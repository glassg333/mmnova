#!/usr/bin/env python3
"""Package MonoJuice sources, factory banks and validation evidence; never include build output/SDK/ROM."""
from pathlib import Path
import argparse
import hashlib
import json
import xml.etree.ElementTree as ET
import zipfile

root = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--output', type=Path, default=root.parent / 'deliverables' / 'MonoJuice_0.8.0.zip')
args = parser.parse_args()
sha = lambda data: hashlib.sha256(data).hexdigest()
fixed = ['CMakeLists.txt', 'CHANGELOG_RU.md', 'PRESETS_RU.md', 'MonoJuice.jucer', 'README_RU.md', 'VALIDATION_RU.md', 'NOTICE_RU.md', 'LICENSE_NEW_CODE.txt', 'package.py', 'BUILD_RU.md']
files = [root / name for name in fixed]
for folder in ['Source', 'tests', 'Analysis', 'Preview', 'TestsResults', 'Licenses', 'Reference']:
    base = root / folder
    if not base.is_dir():
        raise SystemExit(f'Missing required directory: {folder}')
    files.extend(p for p in base.rglob('*') if p.is_file())
for p in files:
    if not p.is_file():
        raise SystemExit(f'Missing required file: {p}')
for name, expected in json.loads((root / 'TestsResults/unchanged-chorus.json').read_text()).items():
    if sha((root / 'Source/dsp' / name).read_bytes()) != expected:
        raise SystemExit(f'Original CHORUS was changed: {name}')
project = ET.parse(root / 'MonoJuice.jucer').getroot()
entries = project.findall('.//FILE')
assert sum(e.get('compile') == '1' for e in entries) == 4
for e in entries:
    assert (root / e.attrib['file']).is_file(), e.attrib['file']
contents = {p.relative_to(root).as_posix(): p.read_bytes() for p in sorted(files)}
manifest = {name: sha(data) for name, data in contents.items()}
contents['MANIFEST_SHA256.json'] = (json.dumps(manifest, indent=2, ensure_ascii=False) + '\n').encode()
args.output.parent.mkdir(parents=True, exist_ok=True)
with zipfile.ZipFile(args.output, 'w', compression=zipfile.ZIP_DEFLATED, compresslevel=9) as z:
    for name, data in sorted(contents.items()):
        info = zipfile.ZipInfo('MonoJuice/' + name, (2026, 9, 17, 0, 0, 0))
        info.compress_type = zipfile.ZIP_DEFLATED
        info.external_attr = 0o100644 << 16
        z.writestr(info, data)
with zipfile.ZipFile(args.output) as z:
    assert z.testzip() is None
    for name, expected in manifest.items():
        assert sha(z.read('MonoJuice/' + name)) == expected
print(json.dumps({'file': str(args.output), 'bytes': args.output.stat().st_size,
                  'entries': len(contents), 'sha256': sha(args.output.read_bytes()),
                  'crc': 'PASS', 'manifest': 'PASS', 'unchanged_chorus': 'PASS'}, indent=2))
