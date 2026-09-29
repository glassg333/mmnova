#!/usr/bin/env python3
# dump_map.py — печать карты смещений omega8_map.json в виде таблицы:
# offset | name | kind | conf | note (conf: 3=сверено с редактором/скринами, 1=гипотеза)
import json, sys
m = json.load(open(sys.argv[1] if len(sys.argv) > 1 else '../omega8_map.json'))['map']
rows = [(int(k), v) for k, v in m.items()] if isinstance(m, dict) else list(enumerate(m))
print('| off | name | kind | conf | note |')
print('|----:|------|------|-----:|------|')
for k, v in sorted(rows):
    if isinstance(v, dict):
        print(f"| {k} | {v.get('name','')} | {v.get('kind','')} | {v.get('conf','')} | {v.get('note','')} |")
