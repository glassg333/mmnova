#!/usr/bin/env python3
"""Fail if a source directory or .tar/.tar.gz payload includes an OS firmware file."""

from __future__ import annotations

import argparse
import tarfile
from pathlib import Path

FORBIDDEN_SUFFIXES = (".syx", ".srec", ".s19", ".hex")
FORBIDDEN_NAMES = ("elektron_sfx6-60_os1.32b",)


def forbidden(name: str) -> bool:
    lower = name.lower()
    return lower.endswith(FORBIDDEN_SUFFIXES) or any(token in lower for token in FORBIDDEN_NAMES)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("path", type=Path)
    args = parser.parse_args()
    path = args.path
    if path.is_dir():
        names = [item.relative_to(path).as_posix() for item in path.rglob("*") if item.is_file()]
    elif path.is_file() and tarfile.is_tarfile(path):
        with tarfile.open(path, "r:*") as archive:
            names = [item.name for item in archive.getmembers() if item.isfile()]
    else:
        parser.error("path must be a directory or a readable tar archive")
    bad = [name for name in names if forbidden(name)]
    if bad:
        print("FIRMWARE BOUNDARY FAIL")
        for name in bad:
            print("-", name)
        return 1
    print(f"FIRMWARE BOUNDARY PASS ({len(names)} regular files scanned; no OS firmware payload)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
