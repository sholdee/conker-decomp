#!/usr/bin/env python3
"""tcd — toolchain command daemon, run INSIDE the conker-tc container.

`podman exec` serializes on podman's container lock: with a fleet of agents each check
waited 20-40 s just to start a process. tcd listens on a Unix socket in a bind-mounted
directory and runs each request as a subprocess inside the container, concurrently.

  podman exec -d conker-tc python3 REPO/tools/toolchain/tcd.py WS/.tcd.sock   (see run.sh)

Protocol: one JSON line {"cmd": str, "cwd": str, "timeout": s} → one JSON line
{"rc": int, "out": str}. The socket is created mode 0600 (owner only).
"""
import json
import os
import socketserver
import subprocess
import sys


class Handler(socketserver.StreamRequestHandler):
    def handle(self):
        try:
            req = json.loads(self.rfile.readline())
            p = subprocess.run(["bash", "-c", req["cmd"]], cwd=req.get("cwd") or None,
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               text=True, errors="replace", timeout=req.get("timeout", 600))
            resp = {"rc": p.returncode, "out": p.stdout}
        except subprocess.TimeoutExpired as e:
            resp = {"rc": 124, "out": (e.stdout or "") if isinstance(e.stdout, str) else ""}
        except Exception as e:  # report, never die
            resp = {"rc": 125, "out": f"tcd: {e!r}"}
        self.wfile.write((json.dumps(resp) + "\n").encode())


class Server(socketserver.ThreadingMixIn, socketserver.UnixStreamServer):
    daemon_threads = True


def main():
    path = sys.argv[1]
    if os.path.exists(path):
        os.unlink(path)
    old = os.umask(0o177)
    srv = Server(path, Handler)
    os.umask(old)
    srv.serve_forever()


if __name__ == "__main__":
    main()
