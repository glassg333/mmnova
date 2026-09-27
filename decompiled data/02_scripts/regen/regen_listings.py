#!/usr/bin/env python3
"""Regenerate the 16 missing machine listings from dsp1_machines_fx.txt
by reusing the repo's own extract_missing_machines.py with corrected paths."""
import os, sys, importlib.util

REPO = "/home/z/my-project/repo_mmnova/decompiled data/!decryptor 56300 - agent can make chorus and fm etc from original"
DISASM = os.path.join(REPO, "memory_images/dsp1_machines_fx.txt")
DISPATCH = os.path.join(REPO, "memory_images/dsp1_dispatch.txt")
OUT = "/home/z/my-project/scripts/out_listings"
os.makedirs(OUT, exist_ok=True)

src_path = os.path.join(REPO, "scripts/extract_missing_machines.py")
src = open(src_path, encoding="utf-8").read()
# neutralize hardcoded constants from the previous session
src = src.replace('ROOT = "/home/z/my-project/work/mmnova/decompiled data"',
                  'ROOT = r"%s"' % REPO)
src = src.replace('DISASM = os.path.join(ROOT, "dsp1_machines_fx.txt")',
                  'DISASM = r"%s"' % DISASM)
src = src.replace('DISPATCH = os.path.join(ROOT, "dsp1_dispatch.txt")',
                  'DISPATCH = r"%s"' % DISPATCH)
src = src.replace('OUT = "/home/z/my-project/work/extract/listings"',
                  'OUT = r"%s"' % OUT)
src = src.replace('if __name__ == "__main__":\n    main()', '')

spec = importlib.util.spec_from_loader("emm", loader=None)
emm = importlib.util.module_from_spec(spec)
exec(compile(src, src_path, "exec"), emm.__dict__)
emm.main()
print("DONE ->", OUT)
