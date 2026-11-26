#!/usr/bin/env python3
"""Post-compile pass over each C object (run by conker/Makefile).

Today it (1) removes IDO's 16-byte alignment padding of .text/.rodata/.data/.bss where the yaml splits
one original file into several project files (trim_text_padding, trim_bss_padding), (2) sets the o32
ABI flag, and (3) makes definitions weak where the yaml asks for it (the game's private libultra copies).

It can also point a C object's .rodata references at rodata that is still assembled from
asm/data/*.rodata.s (the linker script then discards that object's .rodata). Every C object owns its
.rodata now, so this part is unused; the text below describes it. So float/double literals, string literals and switch
jump tables that IDO places in the C file's .rodata would never reach the ROM. Writing
them as `extern f32 D_...` instead is NOT equivalent: IDO allocates registers differently
for a literal than for a global load. So write the natural C, and this pass rewrites the
object after compilation: every `%hi/%lo(.rodata+X)` reference is retargeted to an
absolute symbol at the original ROM address.

The address comes from the ROM itself: the file's .text sits at its yaml offset, so the
original instruction pair at the same offset holds the original %hi/%lo — used when the
ROM bytes there equal the literal (or, for a jump table, the case-offset sequence).
Otherwise (function not matching yet) it falls back to searching the ROM's rodata for
the bytes, and finally to address 0 — the object always links, so `mw check` can diff,
and a wrong guess can only make the whole-bin sha1 fail, never pass.

Case labels that the asm tables reference (`.L1502DB54...`) are PROVIDEd by
jtbl_labels.<ver>.txt so they resolve once their function is C. (Unused now: the file is no longer linked.)

usage: rodata_extern.py BASEBIN YAML OBJ
       rodata_extern.py --labels ASM_DATA_DIR OUT     (regenerate jtbl_labels.<ver>.txt)
No-op (and fast) for objects that don't reference .rodata.
"""
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent / "asm-processor"))
from asm_processor import ElfFile, Symbol, SHT_REL  # noqa: E402

R_MIPS_32, R_MIPS_HI16, R_MIPS_LO16 = 2, 5, 6
STT_SECTION, STB_GLOBAL, STB_WEAK, SHN_ABS = 3, 1, 2, 0xFFF1
OP_LUI, OP_LDC1, OP_SDC1 = 0x0F, 0x35, 0x3D


_FLAT = {}


def flat_segments(yaml_path):
    """[(rom_start, type, name, vram - rom)] for every segment/subsegment, sorted.

    Cached in-process and on disk (build/.flat_segments.json, keyed by the yaml's size and mtime):
    this runs once per object of every build, and parsing the yaml dominated its cost."""
    import json, os
    yp = Path(yaml_path).resolve()
    st = yp.stat()
    key = f"{yp}:{st.st_size}:{st.st_mtime_ns}"
    if key in _FLAT:
        return _FLAT[key]
    cache = yp.parent / "build" / ".flat_segments.json"
    try:
        c = json.loads(cache.read_text())
        if c.get("key") == key:
            _FLAT[key] = [tuple(t) for t in c["flat"]]
            return _FLAT[key]
    except (OSError, ValueError):
        pass
    flat = _flat_segments_parse(yp)
    try:
        tmp = cache.with_suffix(f".{os.getpid()}.tmp")
        tmp.write_text(json.dumps({"key": key, "flat": flat}))
        os.replace(tmp, cache)
    except OSError:
        pass
    _FLAT[key] = flat
    return flat


def _flat_segments_parse(yaml_path):
    import yaml
    loader = getattr(yaml, "CSafeLoader", yaml.SafeLoader)
    cfg = yaml.load(Path(yaml_path).read_text(), Loader=loader)
    flat = []
    for seg in cfg["segments"]:
        if isinstance(seg, list):
            flat.append((seg[0], None, None, 0))
            continue
        delta = seg.get("vram", seg["start"]) - seg["start"]
        flat.append((seg["start"], None if seg.get("subsegments") else seg.get("type"), None, delta))
        for sub in seg.get("subsegments") or []:
            if isinstance(sub, list) and sub:
                flat.append((sub[0], sub[1] if len(sub) > 1 else None,
                             sub[2] if len(sub) > 2 else None, delta))
            elif isinstance(sub, dict):
                flat.append((sub["start"], sub.get("type"), sub.get("name"), delta))
    flat.sort(key=lambda t: t[0])
    return flat


def obj_name(obj):
    return re.sub(r"^.*?build/src/|\.c\.o$", "", obj)


class Rom:
    def __init__(self, path, yaml_path):
        self.data = Path(path).read_bytes()
        self.flat = flat_segments(yaml_path)
        self.rodata = []  # (rom_start, rom_end, delta)
        for (start, typ, _, delta), (end, _, _, _) in zip(self.flat, self.flat[1:]):
            if typ and "rodata" in typ and end > start:
                self.rodata.append((start, end, delta))

    def text_vram(self, name):
        for start, typ, nm, delta in self.flat:
            if typ == "c" and nm == name:
                return start, start + delta
        return None

    def at_vram(self, vram, n):
        for start, end, delta in self.rodata:
            if start + delta <= vram and vram + n <= end + delta:
                off = vram - delta
                return self.data[off:off + n]
        return None

    def word(self, rom_off):
        return struct.unpack(">I", self.data[rom_off:rom_off + 4])[0]

    def search(self, needle):
        """vram of every 4-aligned occurrence of needle in rodata."""
        hits = []
        for start, end, delta in self.rodata:
            seg = self.data[start:end]
            i = seg.find(needle)
            while i != -1:
                if i % 4 == 0:
                    hits.append(start + i + delta)
                i = seg.find(needle, i + 1)
        return hits

    def find_table(self, offs, base=None):
        """vram of every table holding [B + o for o in offs] (B = .text base; fixed if given)."""
        hits = []
        n = len(offs)
        d = [o - offs[0] for o in offs]
        for start, end, delta in self.rodata:
            cnt = (end - start) // 4
            words = struct.unpack(f">{cnt}I", self.data[start:start + 4 * cnt])
            for p in range(cnt - n + 1):
                w0 = words[p]
                if w0 >> 28 != 1 or (base is not None and w0 != base + offs[0]):
                    continue
                if all(words[p + i] - w0 == d[i] for i in range(1, n)):
                    hits.append(start + 4 * p + delta)
        return hits


def owns_rodata(yaml_path, obj):
    """True if the yaml links this object's own .rodata (a `.rodata, <name>` subsegment)."""
    return re.search(rf"\[0x[0-9A-Fa-f]+,\s*\.rodata,\s*{re.escape(obj_name(obj))}\s*\]",
                     Path(yaml_path).read_text()) is not None


def write_labels(asm_data_dir, out_path):
    """PROVIDE every code label an asm jump table references (the label name holds its vram)."""
    labels = set()
    for f in sorted(Path(asm_data_dir).glob("*.s")):
        labels |= set(re.findall(r"\.word (\.L([0-9A-F]{8})\w*)", f.read_text()))
    lines = [f"PROVIDE({name} = 0x{addr});" for name, addr in sorted(labels)]
    Path(out_path).write_text("/* generated by tools/rodata_extern.py --labels; jump-table case labels */\n"
                              + "\n".join(lines) + "\n")


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


DATA_SECTIONS = (".data", ".bss", ".sdata", ".sbss")


def owns_section(yaml_path, obj, kind):
    """True if the yaml links this object's own section `kind`: a list-form subsegment
    `[0xROM, .data, <name>]`, or a dict-form one `{start: ..., type: .bss, vram: ..., name: <name>}`
    (the form bss needs, since its vram cannot be derived from a ROM offset)."""
    text = Path(yaml_path).read_text()
    k, n = re.escape(kind.lstrip(".")), re.escape(obj_name(obj))
    if re.search(rf"\[0x[0-9A-Fa-f]+,\s*\.?{k},\s*{n}\s*\]", text):
        return True
    for d in re.findall(r"\{[^{}]*\}", text):
        if re.search(rf"\btype:\s*\.{k}\b", d) and re.search(rf"\bname:\s*{n}\s*(?:[,}}]|$)", d):
            return True
    return False


def main():
    if sys.argv[1] == "--labels":
        return write_labels(sys.argv[2], sys.argv[3])
    basebin, yaml_path, obj = sys.argv[1:4]
    weak_text = "--weak-text" in sys.argv[4:]
    Path(obj + ".notes").unlink(missing_ok=True)
    set_o32_abi(obj)
    trim_text_padding(obj, yaml_path)
    if owns_rodata(yaml_path, obj):
        trim_text_padding(obj, yaml_path, ".rodata", b".rodata")
    if owns_section(yaml_path, obj, ".data"):
        trim_text_padding(obj, yaml_path, ".data", b".data")
    if owns_section(yaml_path, obj, ".bss"):
        trim_bss_padding(obj, yaml_path)
    elf = ElfFile(Path(obj).read_bytes())
    text = elf.find_section(".text")
    if text is None:
        return
    notes = []
    rom = []  # loaded lazily (the whole bin)

    def get_rom():
        if not rom:
            rom.append(Rom(basebin, yaml_path))
        return rom[0]
    changed = retarget_data(elf, text, get_rom, yaml_path, obj, notes)
    rodata = elf.find_section(".rodata")
    if rodata is not None and not owns_rodata(yaml_path, obj):
        changed |= retarget_rodata(elf, text, rodata, get_rom, obj, notes)
    changed |= weaken_data_defs(elf, yaml_path, obj)
    if weak_text:
        changed |= weaken_section_defs(elf, {text.index})
    if changed:
        rel_tx = [s for s in text.relocated_by if s.sh_type == SHT_REL]
        for rs in rel_tx:
            rs.data = b"".join(r.to_bin() for r in rs.relocations)
        elf.write(obj)
    if notes:
        Path(obj + ".notes").write_text("\n".join(notes) + "\n")


def trim_text_padding(obj, yaml_path, typ_wanted="c", section=b".text"):
    """IDO pads .text (and .rodata) to a multiple of 16. When the object's yaml slice ends on a 4-byte
    boundary (hand-written asm or another file's constant data next to it), cut that zero padding so the
    next object links at its ROM address."""
    name = obj_name(obj)
    flat = flat_segments(yaml_path)
    seglen = None
    for (start, typ, nm, _), (end, _, _, _) in zip(flat, flat[1:]):
        if typ == typ_wanted and nm == name:
            seglen = end - start
            break
    if not seglen or seglen % 16 == 0:
        return
    with open(obj, "r+b") as f:
        data = f.read()
        shoff, = struct.unpack(">I", data[32:36])
        shentsize, shnum, shstrndx = struct.unpack(">HHH", data[46:52])
        def sh(i):
            return shoff + i * shentsize
        stroff = struct.unpack(">I", data[sh(shstrndx) + 16:sh(shstrndx) + 20])[0]
        for i in range(shnum):
            nameoff, = struct.unpack(">I", data[sh(i):sh(i) + 4])
            secname = data[stroff + nameoff:data.index(b"\0", stroff + nameoff)]
            if secname != section:
                continue
            off, size = struct.unpack(">II", data[sh(i) + 16:sh(i) + 24])
            if size > seglen and size - seglen < 16 and not any(data[off + seglen:off + size]):
                f.seek(sh(i) + 20)
                f.write(struct.pack(">I", seglen))
            return


def trim_bss_padding(obj, yaml_path):
    """Like trim_text_padding for an owned .bss: bss has no ROM bytes, so the slice length comes from
    the vram of the next bss entry in the yaml (dict form: {start, type: .bss|bss, vram, name})."""
    name = obj_name(obj)
    ents = []
    for d in re.findall(r"\{[^{}]*\}", Path(yaml_path).read_text()):
        t = re.search(r"\btype:\s*(\.?bss)\b", d)
        v = re.search(r"\bvram:\s*(0x[0-9A-Fa-f]+)", d)
        n = re.search(r"\bname:\s*([\w/]+)", d)
        if t and v:
            ents.append((int(v.group(1), 16), t.group(1), n.group(1) if n else None))
    ents.sort()
    seglen = None
    for (v, t, n), (v2, _, _) in zip(ents, ents[1:]):
        if t == ".bss" and n == name:
            seglen = v2 - v
            break
    if not seglen or seglen % 16 == 0:
        return
    with open(obj, "r+b") as f:
        data = f.read()
        shoff, = struct.unpack(">I", data[32:36])
        shentsize, shnum, shstrndx = struct.unpack(">HHH", data[46:52])
        sh = lambda i: shoff + i * shentsize
        stroff = struct.unpack(">I", data[sh(shstrndx) + 16:sh(shstrndx) + 20])[0]
        for i in range(shnum):
            nameoff, = struct.unpack(">I", data[sh(i):sh(i) + 4])
            if data[stroff + nameoff:data.index(b"\0", stroff + nameoff)] != b".bss":
                continue
            size, = struct.unpack(">I", data[sh(i) + 20:sh(i) + 24])
            if size > seglen and size - seglen < 16:
                f.seek(sh(i) + 20)
                f.write(struct.pack(">I", seglen))
            return


def set_o32_abi(obj):
    """A -mips3 -32 object (the SDK's libc/ll.c) is flagged 64-bit; mark it EF_MIPS_ABI_O32 so a
    modern ld links it with the -mips2 objects (ultralib's tools/set_o32abi_bit.py does the same)."""
    with open(obj, "r+b") as f:
        f.seek(36)
        flags = struct.unpack(">I", f.read(4))[0]
        if flags >> 28 == 2 and not flags & 0x0000F000:  # EF_MIPS_ARCH_3, no ABI bits
            f.seek(36)
            f.write(struct.pack(">I", flags | 0x00001000))


def weaken_section_defs(elf, idx):
    changed = False
    for s in elf.symbol_entries[1:]:
        if s.st_shndx in idx and s.st_bind == STB_GLOBAL:
            s.st_bind = STB_WEAK
            changed = True
    if changed:
        elf.symtab.data = b"".join(s.to_bin() for s in elf.symbol_entries)
    return changed


def weaken_data_defs(elf, yaml_path, obj):
    """Globals this object defines in data sections the link discards (its .data/.bss/.rodata
    are assembled from asm/data instead) become weak, so the asm's definition wins instead of
    clashing: ported library code (ultralib) defines its own globals, e.g. osViClock."""
    idx = set()
    for name in DATA_SECTIONS + (".rodata",):
        sec = elf.find_section(name)
        owned = owns_rodata(yaml_path, obj) if name == ".rodata" else owns_section(yaml_path, obj, name)
        if sec is not None and not owned:
            idx.add(sec.index)
    return weaken_section_defs(elf, idx)


def pair_refs(text, rel_tx, wanted, ins_at):
    """[(hi or None, lo, X)] for %hi/%lo pairs against symbols in `wanted` {sym_index: base offset}."""
    refs = []
    for rs in rel_tx:
        last_hi = None
        for r in rs.relocations:
            if r.sym_index not in wanted:
                continue
            if r.rel_type == R_MIPS_HI16:
                last_hi = r
            elif r.rel_type == R_MIPS_LO16:
                hi = last_hi if last_hi is not None and last_hi.sym_index == r.sym_index else None
                himm = ins_at(hi.r_offset) & 0xFFFF if hi else 0
                x = wanted[r.sym_index] + (himm << 16) + sext16(ins_at(r.r_offset) & 0xFFFF)
                refs.append((hi, r, x))
    return refs


def add_abs_symbols(elf, prefix, addrs):
    index = {}
    for addr in sorted(set(addrs)):
        name = f"{prefix}.{addr:08X}"  # dotted: decomp-permuter treats it as a symbol
        sym = Symbol.from_parts(elf.fmt, elf.sym_strtab.add_str(name), addr, 0,
                                STB_GLOBAL << 4, 0, SHN_ABS, elf.sym_strtab, name)
        index[addr] = len(elf.symbol_entries)
        elf.symbol_entries.append(sym)
        elf.symtab.data += sym.to_bin()
    return index


def rewrite_refs(text, tdata, refs, resolved_of, index):
    done_hi = set()
    for hi, lo, key in refs:
        for r in (hi, lo):
            if r is None or id(r) in done_hi:
                continue
            if r is hi:
                done_hi.add(id(r))  # a shared %hi keeps its first partner's symbol
            r.sym_index = index[resolved_of(key)]
            w = struct.unpack(">I", tdata[r.r_offset:r.r_offset + 4])[0]
            tdata[r.r_offset:r.r_offset + 4] = struct.pack(">I", w & 0xFFFF0000)
    text.data = bytes(tdata)


_SYMADDR = {}


def static_addr(name, yaml_path):
    """Address of a file-local static: from a D_<ADDR> name, or (after the wave-3 renames) from the
    symbol_addrs file next to the yaml."""
    m = re.fullmatch(r"D_([0-9A-Fa-f]{8})(?:\.\w+)?", name or "")
    if m:
        return int(m.group(1), 16)
    if not name:
        return None
    if not _SYMADDR:
        f = Path(yaml_path).resolve().parent / "symbol_addrs.us.txt"
        for line in f.read_text().splitlines() if f.exists() else []:
            mm = re.match(r"\s*([A-Za-z_]\w*)\s*=\s*0x([0-9A-Fa-f]+)", line)
            if mm:
                _SYMADDR[mm.group(1)] = int(mm.group(2), 16)
        _SYMADDR.setdefault("", 0)
    base = re.sub(r"\.\w+$", "", name)
    return _SYMADDR.get(base)


def retarget_data(elf, text, get_rom, yaml_path, obj, notes):
    """Point %hi/%lo references into this object's own .data/.bss (function-local statics,
    file statics) at the original address in the game's data segment. The linker script keeps
    only .text of game objects, and the original values are already assembled from asm/data.
    Address: a static named D_XXXXXXXX gives it directly (plus the offset into it); otherwise
    the original instruction pair at the same .text offset; otherwise 0 (links; never matches)."""
    secs = [elf.find_section(n) for n in DATA_SECTIONS]
    secs = [s for s in secs if s is not None and not owns_section(yaml_path, obj, s.name)]
    if not secs:
        return False
    idx = {s.index for s in secs}
    syms = elf.symbol_entries
    wanted = {i: (0 if s.st_type == STT_SECTION else s.st_value)
              for i, s in enumerate(syms) if i and s.st_shndx in idx}
    rel_tx = [s for s in text.relocated_by if s.sh_type == SHT_REL]
    tdata = bytearray(text.data)

    def ins_at(off):
        return struct.unpack(">I", tdata[off:off + 4])[0]
    refs = pair_refs(text, rel_tx, wanted, ins_at)
    if not refs:
        return False
    named = [(s.st_shndx, s.st_value, max(s.st_size, 1), a)
             for s in syms if s.st_shndx in idx and s.st_type != STT_SECTION
             for a in [static_addr(s.name, yaml_path)] if a is not None]
    rom = get_rom()
    tv = rom.text_vram(obj_name(obj))
    resolved = {}
    keyed = []
    for hi, lo, x in refs:
        shndx = syms[lo.sym_index].st_shndx
        key = (shndx, x)
        keyed.append((hi, lo, key))
        if key in resolved:
            continue
        addr = next((a + (x - v) for sh, v, n, a in named if sh == shndx and v <= x < v + n), None)
        if addr is None and tv and hi is not None:
            rh, rl = rom.word(tv[0] + hi.r_offset), rom.word(tv[0] + lo.r_offset)
            if rh >> 26 == OP_LUI and rl >> 26 == ins_at(lo.r_offset) >> 26:
                addr = ((rh & 0xFFFF) << 16) + sext16(rl & 0xFFFF)
        if addr is None:
            addr = 0
            notes.append(f"static data at {syms[lo.sym_index].name or 'section'}+0x{x:X} (.text+0x{lo.r_offset:X}) "
                         f"has no known address: name the static D_<ADDR> to place it")
        resolved[key] = addr
    tag = re.sub(r"\W", "_", obj_name(obj))
    index = add_abs_symbols(elf, f".da.{tag}", resolved.values())
    rewrite_refs(text, tdata, keyed, resolved.get, index)
    return True


def retarget_rodata(elf, text, rodata, get_rom, obj, notes):
    syms = elf.symbol_entries
    ro_syms = {i: (0 if s.st_type == STT_SECTION else s.st_value)
               for i, s in enumerate(syms) if s.st_shndx == rodata.index and i}
    rel_tx = [s for s in text.relocated_by if s.sh_type == SHT_REL]
    tdata = bytearray(text.data)

    def ins_at(off):
        return struct.unpack(">I", tdata[off:off + 4])[0]

    # (hi or None, lo, X): every reference into .rodata, X = offset within it.
    refs = []
    for rs in rel_tx:
        last_hi = None
        for r in rs.relocations:
            if r.sym_index not in ro_syms:
                continue
            if r.rel_type == R_MIPS_HI16:
                last_hi = r
            elif r.rel_type == R_MIPS_LO16:
                hi = last_hi if last_hi is not None and last_hi.sym_index == r.sym_index else None
                himm = ins_at(hi.r_offset) & 0xFFFF if hi else 0
                x = ro_syms[r.sym_index] + (himm << 16) + sext16(ins_at(r.r_offset) & 0xFFFF)
                refs.append((hi, r, x))
                last_hi = None if hi is None else last_hi
    if not refs:
        return False

    # Jump-table words: R_MIPS_32 in .rel.rodata against .text.
    entries = {}
    for rs in rodata.relocated_by:
        for r in rs.relocations:
            s = syms[r.sym_index]
            if r.rel_type == R_MIPS_32 and s.st_shndx == text.index:
                (w,) = struct.unpack(">I", rodata.data[r.r_offset:r.r_offset + 4])
                entries[r.r_offset] = w + (0 if s.st_type == STT_SECTION else s.st_value)
    ref_offs = sorted({x for _, _, x in refs})
    tables = {}
    for x in ref_offs:
        if x in entries:
            offs, k = [], x
            while k in entries and (k == x or k not in ref_offs):
                offs.append(entries[k]); k += 4
            tables[x] = offs

    LOAD_WIDTH = {0x20: 1, 0x24: 1, 0x21: 2, 0x25: 2, 0x23: 4, 0x31: 4, 0x39: 4,
                  OP_LDC1: 8, OP_SDC1: 8}  # lb lbu lh lhu lw lwc1 swc1 ldc1 sdc1

    def literal(x, lo):
        """Bytes of the literal at X as the referencing instruction uses it: a load reads its
        own width; an address (addiu) may be a string (through its NUL) or data (one word)."""
        nxt = min([o for o in ref_offs if o > x] + [len(rodata.data)])
        b = rodata.data[x:min(nxt, x + 256)]
        width = LOAD_WIDTH.get(ins_at(lo.r_offset) >> 26)
        if width:
            return b[:width]
        if b"\0" in b and b.index(b"\0") >= 1 and all(32 <= c < 127 or c in (9, 10) for c in b[:b.index(b"\0")]):
            return b[:b.index(b"\0") + 1]
        return b[:4]

    rom = get_rom()
    tv = rom.text_vram(obj_name(obj))
    base = tv[1] if tv else None

    resolved = {}
    for hi, lo, x in refs:
        if x in resolved:
            continue
        addr = None
        if tv and hi is not None:  # the original %hi/%lo at the same offset
            rh, rl = rom.word(tv[0] + hi.r_offset), rom.word(tv[0] + lo.r_offset)
            if rh >> 26 == OP_LUI and rl >> 26 == ins_at(lo.r_offset) >> 26:
                a = ((rh & 0xFFFF) << 16) + sext16(rl & 0xFFFF)
                if x in tables:
                    ok = a in rom.find_table(tables[x], base)
                else:
                    lit = literal(x, lo)
                    have = rom.at_vram(a, len(lit))
                    ok = have == lit
                    if not ok and have is not None:
                        notes.append(f"literal at file .text+0x{lo.r_offset:X} is {show(lit)} but the "
                                     f"original rodata at 0x{a:08X} holds {show(have)}")
                if ok:
                    addr = a
        if addr is None:
            hits = rom.find_table(tables[x], base) if x in tables else rom.search(literal(x, lo) or b"\0\0\0\0")
            if hits:
                near = [v for v in resolved.values() if v]
                addr = min(hits, key=lambda h: min((abs(h - v) for v in near), default=0))
            else:
                addr = 0
                if x not in tables:
                    lit = literal(x, lo)
                    notes.append(f"literal at file .text+0x{lo.r_offset:X} is {show(lit)}, which appears "
                                 f"nowhere in the ROM rodata — wrong value/precision?")
        resolved[x] = addr

    tag = re.sub(r"\W", "_", obj_name(obj))
    index = {}
    for addr in set(resolved.values()):
        name = f".ro.{tag}.{addr:08X}"  # dotted: decomp-permuter treats it as a symbol
        sym = Symbol.from_parts(elf.fmt, elf.sym_strtab.add_str(name), addr, 0,
                                STB_GLOBAL << 4, 0, SHN_ABS, elf.sym_strtab, name)
        index[addr] = len(elf.symbol_entries)
        elf.symbol_entries.append(sym)
        elf.symtab.data += sym.to_bin()
    done_hi = set()
    for hi, lo, x in refs:
        for r in (hi, lo):
            if r is None or id(r) in done_hi:
                continue
            if r is hi:
                done_hi.add(id(r))  # a shared %hi keeps its first partner's symbol
            r.sym_index = index[resolved[x]]
            tdata[r.r_offset:r.r_offset + 4] = struct.pack(">I", ins_at(r.r_offset) & 0xFFFF0000)
    text.data = bytes(tdata)
    return True


def show(b):
    """Human form of a rodata literal: float/double value, else the string or hex."""
    if len(b) == 4:
        return f"0x{b.hex().upper()} ({struct.unpack('>f', b)[0]!r}f)"
    if len(b) == 8:
        return f"0x{b.hex().upper()} ({struct.unpack('>d', b)[0]!r})"
    if b.endswith(b"\0"):
        return repr(b[:-1].decode("latin1"))
    return "0x" + b.hex().upper()


if __name__ == "__main__":
    main()
