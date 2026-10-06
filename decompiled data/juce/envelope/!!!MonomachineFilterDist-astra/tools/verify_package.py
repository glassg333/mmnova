"""Static/source verification only. Never invokes a C++ compiler."""
import hashlib,json,pathlib,re,ast
import generate_port as gen
ROOT=pathlib.Path(__file__).resolve().parent.parent

def verify():
    meta=json.loads((ROOT/'evidence/provenance.json').read_text())
    mem=[{int(k):v for k,v in d.items()} for d in json.loads((ROOT/'evidence/memory.json').read_text())]
    chosen=[pc for _,a,b in gen.regions for pc in gen.prog if a<=pc<b]
    assert len(chosen)==meta['instruction_count']==786
    assert sum(len(gen.prog[p][1]) for p in chosen)==meta['machine_words_verified']==900
    for p in chosen:
        for i,w in enumerate(gen.prog[p][1]):assert mem[0][p+i]==w
    lo,hi=int(meta['table_start'],16),int(meta['table_end_exclusive'],16)
    tables=[mem[0][a] for a in range(lo,hi)]
    assert hi-lo==meta['table_words']==12486
    assert hashlib.sha256(b''.join(w.to_bytes(3,'big') for w in tables)).hexdigest()==meta['table_sha256_be24']

    for name,text in gen.render().items():
        assert (ROOT/name).read_text()==text,name

    generated=(ROOT/'include/FirmwareFilterDist.hpp').read_text()
    arithmetic=(ROOT/'include/DspArithmetic.hpp').read_text()
    gotos=re.findall(r'goto (L[0-9a-f]+);',generated)
    labels=re.findall(r'^(L[0-9a-f]+):;',generated,re.M)
    assert set(gotos)==set(labels) and len(labels)==len(set(labels))
    for method in set(re.findall(r'\bs\.(\w+)\(',generated)):
        assert re.search(r'\b'+method+r'\([^;{}]*\) (?:const )?noexcept',arithmetic),method
    registers=set(re.search(r'enum class R \{([^}]+)',arithmetic,re.S)[1].replace('\n','').replace(' ','').split(','))
    assert set(re.findall(r'R::(\w+)',generated))<=registers
    for h in (ROOT/'include').glob('*.hpp'):
        t=h.read_text()
        # Lexical guard only; not a C++ parser/type checker.
        t=re.sub(r'//[^\n]*|/\*.*?\*/','',t,flags=re.S)
        assert t.count('{')==t.count('}'),h.name
        for local in re.findall(r'#include "([^"]+)"',t):assert (h.parent/local).is_file(),local
    for script in (ROOT/'tools').glob('*.py'):ast.parse(script.read_text(),filename=script.name)
    manifest=ROOT/'SHA256SUMS'
    if manifest.exists():
        for line in manifest.read_text().splitlines():
            digest,name=line.split('  ',1)
            assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest,name
    print('PASS: words/tables, reproducible C++, local includes, registers, labels, Python syntax'+
          (', SHA256 manifest.' if manifest.exists() else '.'))
    print('No C++ compiler, C++ type checking or independent DSP/audio oracle used.')

if __name__=='__main__':verify()
