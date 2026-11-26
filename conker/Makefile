BASENAME := conker

BUILD_DIR = build
VERSION   := us

### Split configuration

OFFSETS_LENGTH      := 2048
TEXT_SECTION_LENGTH := 1335000
DATA_SECTION_LENGTH := 92000

### Version differences

ifeq ($(VERSION),us)
SRC_DIR  := src
else
ifeq ($(VERSION),eu)
SRC_DIR := src_eu
else
ifeq ($(VERSION),ects)
SRC_DIR := src_ects
else
SRC_DIR := src_debug
OFFSETS_LENGTH      := 2112
TEXT_SECTION_LENGTH := 1380000
COMPRESS_DIR_ARGS   := --num-offsets 528 --total-size 1380000
endif
endif
endif

### Directories

ASM_DIRS  = asm \
            asm/libultra asm/libultra/audio asm/libultra/gu asm/libultra/io asm/libultra/libc asm/libultra/os \
			asm/data asm/data/init asm/data/init/data asm/data/game asm/data/libultra

BIN_DIRS  = assets

SPLIT_DIR := $(BUILD_DIR)/split

# Every directory under src/: source files may live in nested directories (src/game/<Area>/<Name>/).
SRC_DIRS := $(shell find $(SRC_DIR) -type d)


S_FILES   = $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s)) \
            $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))   # hand-written asm (hasm) sources
C_FILES   = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
H_FILES   = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))
BIN_FILES = $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))

O_FILES := $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.s.o)) \
           $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.c.o)) \
           $(foreach file,$(BIN_FILES),$(BUILD_DIR)/$(file:.bin=.bin.o))

TARGET = $(BUILD_DIR)/$(BASENAME).$(VERSION)

LD_SCRIPT = $(BASENAME).ld

# IDO 5.3 (static recomp); the toolchain image installs it here
IDO    ?= /opt/ido/5.3
CC     := $(IDO)/cc

CROSS   = mips-linux-gnu-
AS      = $(CROSS)as
CPP     = cpp
LD      = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
PYTHON  = python3

RZIP    := $(PYTHON) ../tools/rarezip.py
RZIPDIR := $(PYTHON) ../tools/compress_dir.py
SPLIT := split -b 4096 -d --additional-suffix=.bin --suffix-length=4

OPT_FLAGS := -O2 -g3
MIPSBIT := -mips2 -o32

ASM_PROCESSOR_DIR := ../tools/asm-processor
RODATA_EXTERN = $(PYTHON) ../tools/rodata_extern.py $(BASENAME).$(VERSION).bin $(BASENAME).$(VERSION).yaml

# Target version differences

ifeq ($(VERSION),ects)
TARGET_GAME_BIN = $(TARGET).game.bin
TARGET_ROOT_GAME_BIN = ../assets/game.$(VERSION).bin
else
TARGET_GAME_BIN = $(TARGET).game.rzip.bin
TARGET_ROOT_GAME_BIN = ../assets/game.$(VERSION).rzip.bin
endif

# Files requiring pre/post-processing
GREP := grep -rl
GLOBAL_ASM_C_FILES := $(shell $(GREP) GLOBAL_ASM $(SRC_DIR) </dev/null)
GLOBAL_ASM_O_FILES := $(foreach file,$(GLOBAL_ASM_C_FILES),$(BUILD_DIR)/$(file:.c=.c.o))

INCLUDE_CFLAGS := -I . -I include -I include/2.0L -I include/2.0L/PR -I include/libc -I src/libultra/os -I src/libultra/audio -I src/libultra/io

ASFLAGS = -EB -mtune=vr4300 -march=vr4300 -mabi=32 -no-pad-sections -I include

CFLAGS := -G 0 -Xfullwarn -Xcpluscomm -signed -nostdinc -non_shared -Wab,-r4300_mul
CFLAGS += -D_LANGUAGE_C -D_FINALROM -DF3DEX_GBI_2 -D_MIPS_SZLONG=32
# ignore compiler warnings about anonymous structs
CFLAGS += -woff 649,838
CFLAGS += $(INCLUDE_CFLAGS)

LDFLAGS = -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(TARGET).map -T undefined_syms.$(VERSION).txt -T undefined_funcs.$(VERSION).txt -T undefined_syms_auto.txt -T undefined_funcs_auto.txt -T jtbl_labels.$(VERSION).txt --no-check-sections

### Overrides

$(BUILD_DIR)/$(SRC_DIR)/libultra/audio/%.o: OPT_FLAGS := -g

# TODO: blindly move all of this into libultra/audio
$(BUILD_DIR)/$(SRC_DIR)/init_17F10.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_17F80.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_19B50.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_1AAE0.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_1C060.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_1E480.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_1E4A0.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_1E530.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_1FB40.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_20000.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_214F0.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_22040.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/init_22460.c.o: OPT_FLAGS := -g


$(BUILD_DIR)/$(SRC_DIR)/libultra/io/ai.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/io/aigetstat.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/io/aisetfreq.c.o: OPT_FLAGS := -O1

$(BUILD_DIR)/$(SRC_DIR)/libultra/os/getthreadpri.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/os/initialize.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/io/crc.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/io/crc2.c.o: OPT_FLAGS := -O1
# The game was built with a raised optimizer size limit: functions over IDO's default -Olimit
# (uopt ~1000 blocks, as1 500) come out unoptimized otherwise (func_1504CB98, func_150265CC,
# func_1502460C, func_15065A5C). The whole bin is unchanged with it (2026-11-16).
$(BUILD_DIR)/$(SRC_DIR)/game_%.c.o: OLIMIT := -Olimit 3000

$(BUILD_DIR)/$(SRC_DIR)/init_3920.c.o: OPT_FLAGS := -O2

# Per-file flags (proven by the sha1 gate). libultra shipped without debug info, and the game
# links its own private libultra copy at 0x151Dxxxx-0x151Fxxxx built at plain -O1/-O2.
$(BUILD_DIR)/$(SRC_DIR)/init_210C0.c.o: OPT_FLAGS := -g
# Rare's compressed-audio bitstream decoder: the whole TU is unoptimised -g output.
$(BUILD_DIR)/$(SRC_DIR)/game/Audio/Mp3Decoder/mp3decoder.c.o: OPT_FLAGS := -g
$(BUILD_DIR)/$(SRC_DIR)/libultra/io/pfsisplug2.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/io/pfschecker2.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/gu/guNormalize.c.o: OPT_FLAGS := -O2
$(BUILD_DIR)/$(SRC_DIR)/game_21C4F0.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/game_21C540.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/game_21CAF0.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/game_21D420.c.o: OPT_FLAGS := -O2
$(BUILD_DIR)/$(SRC_DIR)/game_21D5F0.c.o: OPT_FLAGS := -O3

# $(BUILD_DIR)/$(SRC_DIR)/init_B1B0.o: OPT_FLAGS := -O2

# $(BUILD_DIR)/$(SRC_DIR)/game_14FF90.o: OPT_FLAGS := -O3

# libultra 2.0G from the ultralib decompilation (lib/ultralib/include is its header tree), built
# with ultralib's own flags for that version: os/ and io/ at -O1, libc/ and gu/ at -O3.
ULTRA_CFLAGS := -G 0 -nostdinc -non_shared -Xcpluscomm -fullwarn -woff 516,649,838,712 -Wab,-r4300_mul \
                -D_MIPS_SZLONG=32 -D_FINALROM -DNDEBUG -DBUILD_VERSION=VERSION_G -DBUILD_VERSION_STRING=\"2.0G\" \
                -I lib/ultralib/include -I lib/ultralib/include/compiler/ido -I lib/ultralib/include/PR \
                -I lib/ultralib/src/gu -I lib/ultralib/src/libc -I lib/ultralib/src/os
ULTRALIB_SRCS :=
include ultralib.mk
$(foreach s,$(ULTRALIB_SRCS),$(eval $(BUILD_DIR)/$(SRC_DIR)/libultra/$(s).c.o: CFLAGS := $(ULTRA_CFLAGS)))
$(foreach s,$(filter os/% io/%,$(ULTRALIB_SRCS)),$(eval $(BUILD_DIR)/$(SRC_DIR)/libultra/$(s).c.o: OPT_FLAGS := -O1))
$(foreach s,$(filter libc/% gu/%,$(ULTRALIB_SRCS)),$(eval $(BUILD_DIR)/$(SRC_DIR)/libultra/$(s).c.o: OPT_FLAGS := -O3))
# The game links its own copy of some libultra files (stems ending in 2) whose calls - even to
# functions of the same file - bind to the init copy: their own definitions are weak, and the
# game names of their entry points are address aliases in undefined_syms.
$(foreach s,$(filter %2,$(ULTRALIB_SRCS)),$(eval $(BUILD_DIR)/$(SRC_DIR)/libultra/$(s).c.o: RODATA_EXTERN_FLAGS := --weak-text))
# Game copies that game code calls by its own names (and that call each other): definitions
# renamed onto the game names, sources untouched.
$(BUILD_DIR)/$(SRC_DIR)/libultra/gu/mtxutil.c.o: CFLAGS += -DguMtxF2L=guMtxF2L2 -DguMtxIdentF=func_151EFE00 -DguMtxIdent=func_151EFE88
$(BUILD_DIR)/$(SRC_DIR)/libultra/gu/ortho.c.o: CFLAGS += -DguMtxF2L=guMtxF2L2 -DguMtxIdentF=func_151EFE00 -DguOrthoF=func_151EF800 -DguOrtho=func_151EF954
# io/vimgr.c was rebuilt with the game's own flags
$(BUILD_DIR)/$(SRC_DIR)/libultra/io/vimgr.c.o: OPT_FLAGS := -O2 -g3
# libc/ll.c is the SDK's 64-bit helper file: -O1 -mips3 -32
$(BUILD_DIR)/$(SRC_DIR)/libultra/libc/ll.c.o: OPT_FLAGS := -O1
$(BUILD_DIR)/$(SRC_DIR)/libultra/libc/ll.c.o: MIPSBIT := -mips3 -32

### Non-matching
ifeq ($(NON_MATCHING),1)
VERIFY :=
else
VERIFY := verify
endif

### Targets

default: all

all: dirs $(TARGET).bin $(VERIFY)

dirs:
	$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(BIN_DIRS) ,$(shell mkdir -p build/$(dir)))

clean:
	rm -rf build

really-clean: clean
	rm -rf asm
	rm -rf bin
	rm -rf assets
	rm -rf $(BASENAME).*.ok
	rm -f *auto.txt
	rm -rf conker.us.bin conker.eu.bin conker.ects.bin conker.debug.bin

extract: $(BUILD_DIR)/splat

# sanity check base bin
check: $(BASENAME).$(VERSION).ok

# check built bin
verify: $(TARGET).ok

# replace original binaries
replace: $(VERIFY) $(TARGET).header.bin $(TARGET).boot.bin $(TARGET).init.bin $(TARGET_GAME_BIN) $(TARGET).debugger.bin
	cp $(TARGET).header.bin ../assets/header.$(VERSION).bin
	cp $(TARGET).boot.bin ../assets/boot.$(VERSION).bin
	cp $(TARGET).init.bin ../assets/init.$(VERSION).bin
	cp $(TARGET).debugger.bin ../assets/debugger.$(VERSION).bin
	cp $(TARGET_GAME_BIN) $(TARGET_ROOT_GAME_BIN)


### Recipes

$(BUILD_DIR)/$(LD_SCRIPT): $(LD_SCRIPT)
	$(CPP) -P -DBUILD_DIR=$(BUILD_DIR) -o $@ $<

# filter_auto_syms --linked: drop splat's absolute auto entry for every global a linked section defines
$(TARGET).elf: $(O_FILES) $(BUILD_DIR)/$(LD_SCRIPT) $(GLOBAL_ASM_O_FILES)
	@$(PYTHON) ../tools/filter_auto_syms.py . --linked >/dev/null
	$(LD) $(LDFLAGS) -o $@

ifndef PERMUTER
$(GLOBAL_ASM_O_FILES): $(BUILD_DIR)/%.c.o: %.c include/variables.h include/structs.h include/functions.h
	@cpp -MM -MP -MT $@ -MF $@.d -nostdinc $(filter -I% -D% -U%,$(subst -I ,-I,$(CFLAGS))) -D_LANGUAGE_C -D__sgi $< 2>/dev/null || rm -f $@.d
	$(PYTHON) $(ASM_PROCESSOR_DIR)/asm_processor.py $(OPT_FLAGS) $< > $(BUILD_DIR)/$<
	$(CC) -c -32 $(CFLAGS) $(OPT_FLAGS) $(OLIMIT) $(LOOP_UNROLL) $(MIPSBIT) -o $@ $(BUILD_DIR)/$<
	$(PYTHON) $(ASM_PROCESSOR_DIR)/asm_processor.py $(OPT_FLAGS) $< --post-process $@ \
		--assembler "$(AS) $(ASFLAGS)" --asm-prelude $(ASM_PROCESSOR_DIR)/prelude.inc
	$(RODATA_EXTERN) $@ $(RODATA_EXTERN_FLAGS)
endif

$(BUILD_DIR)/%.c.o: %.c
	@cpp -MM -MP -MT $@ -MF $@.d -nostdinc $(filter -I% -D% -U%,$(subst -I ,-I,$(CFLAGS))) -D_LANGUAGE_C -D__sgi $< 2>/dev/null || rm -f $@.d
	$(CC) -c -32 $(CFLAGS) $(OPT_FLAGS) $(OLIMIT) $(MIPSBIT) -o $@ $<
	$(RODATA_EXTERN) $@ $(RODATA_EXTERN_FLAGS)

$(BUILD_DIR)/%.s.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.bin.o: %.bin
	$(LD) -r -b binary -o $@ $<

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) $(OBJCOPYFLAGS) -O binary $< $@

# extract
$(BUILD_DIR)/splat: check $(BASENAME).$(VERSION).yaml
	$(PYTHON) ../tools/n64splat/split.py $(BASENAME).$(VERSION).yaml
	$(PYTHON) ../tools/filter_auto_syms.py

%.ok: %.bin
	@echo "$$(cat $(BASENAME).$(VERSION).sha1)  $<" | sha1sum --check
	@touch $@

conker.$(VERSION).bin:
	make extract -C ..

# dump sections
$(TARGET).header.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .header $< $@

$(TARGET).boot.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .boot $< $@

$(TARGET).init.bin: $(TARGET).init.code.bin $(TARGET).init.data.bin
	cat $^ > $@

$(TARGET).init.code.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .init $< $@

$(TARGET).init.data.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .init_data $< $@

#ects only
$(TARGET).game.bin: $(TARGET).game.code.bin $(TARGET).game.data.bin
	cat $^ > $@

$(TARGET).game.code.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .game $< $@

$(TARGET).game.data.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .game_data $< $@

$(TARGET).debugger.bin: $(TARGET).debugger.code.bin $(TARGET).debugger.data.bin
	cat $^ > $@

$(TARGET).debugger.code.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .debugger $< $@

$(TARGET).debugger.data.bin: $(TARGET).elf
	$(OBJCOPY) -O binary --only-section .debugger_data $< $@

# split code section
$(SPLIT_DIR)/0000.bin: $(TARGET).game.code.bin
	mkdir -p $(SPLIT_DIR)
	$(SPLIT) $< $(SPLIT_DIR)/

# create offsets file
$(SPLIT_DIR)/offsets.bin: $(SPLIT_DIR)/0000.bin
	$(RZIPDIR) $(SPLIT_DIR) $(SPLIT_DIR) --offsets-file $@ $(COMPRESS_DIR_ARGS)

# concatenate compressed code blocks
$(TARGET).game.code.rzip.bin: $(TARGET).game.code.bin $(SPLIT_DIR)/offsets.bin
	cat $(SPLIT_DIR)/*.gz > $@

# compressed data section
$(TARGET).game.data.rzip.bin: $(TARGET).game.data.bin
	$(RZIP) $< $@

# create padding
# The game segment has a fixed size (offsets + code = TEXT_SECTION_LENGTH, data = DATA_SECTION_LENGTH),
# so everything after it keeps its ROM address. pad_to.py fails if a part outgrows its reservation.
$(TARGET).game.code.padding.bin: $(SPLIT_DIR)/offsets.bin $(TARGET).game.code.rzip.bin
	$(PYTHON) ../tools/pad_to.py ../assets/game/code_padding.bin $$(($(TEXT_SECTION_LENGTH)-$$(wc -c <$(SPLIT_DIR)/offsets.bin)-$$(wc -c <$(TARGET).game.code.rzip.bin))) $@

$(TARGET).game.data.padding.bin: $(TARGET).game.data.rzip.bin
	$(PYTHON) ../tools/pad_to.py /dev/null $$(($(DATA_SECTION_LENGTH)-$$(wc -c <$<))) $@

$(TARGET).game.rzip.bin: $(SPLIT_DIR)/offsets.bin $(TARGET).game.code.rzip.bin $(TARGET).game.code.padding.bin $(TARGET).game.data.rzip.bin $(TARGET).game.data.padding.bin
	cat $^ > $@


# settings
.PHONY: all clean default
SHELL = /bin/bash -e -o pipefail

# Header dependencies (generated by the compile rules above with GNU cpp -MM). Without them a header
# change left stale objects that still passed the sha gate.
-include $(addsuffix .d,$(filter %.c.o,$(O_FILES)))
