"""Read-only firmware extraction; no compiling. Format follows Monomodule Firmware.cpp."""
import hashlib,json,pathlib,re
ROOT=pathlib.Path(__file__).resolve().parent.parent
def unpack_syx(data):
    out=bytearray(); base=None; expect=None
    for msg in re.findall(b'\xf0[^\xf7]*\xf7',data):
        if not msg.startswith(bytes.fromhex('f0 00 20 3c 03 00 7e')) or len(msg)<=15:continue
        addr=0
        for c in msg[9:15]:addr=(addr<<4)|(c&15)
        if base is None:base=expect=addr
        assert addr==expect and (len(msg)-16)%3==0
        for i in range(15,len(msg)-1,3):
            w=(msg[i]<<14)|(msg[i+1]<<7)|msg[i+2]
            assert w<=65535
            out.extend(w.to_bytes(2,'big'))
        expect=base+len(out)
    return base,bytes(out)
def depack(data):
    i=0; left=0; tag=0; last=1; out=bytearray()
    def byte():
        nonlocal i
        r=data[i];i+=1;return r
    def bit():
        nonlocal left,tag
        if left==0:tag=byte();left=8
        left-=1;return(tag>>left)&1
    def gamma():
        v=1
        while True:
            v=v*2+bit()
            if bit():return v
    try:
        while True:
            if bit():out.append(byte());continue
            g=gamma()
            if g==2:off=last
            else:
                raw=g*256+byte()
                if raw==767:break
                off=raw-767;last=off
            sl=bit()*2+bit();length=sl if sl else gamma()+2
            if off>3328:length+=1
            assert 0<off<=len(out)
            for _ in range(length+1):out.append(out[-off])
    except IndexError:
        pass # Firmware.cpp terminates on exhausted stream, including residual tag bits.
    return bytes(out)
def records(data):
    assert len(data)%3==0
    w=[int.from_bytes(data[i:i+3],'little') for i in range(0,len(data),3)]
    p=0;out=[]
    while p<len(w):
        t=w[p]
        if t==3:p+=2;continue
        assert t in (0,1,2)
        t,a,n=w[p:p+3];assert p+3+n<=len(w)
        out.append((t,a,w[p+3:p+3+n]));p+=3+n
    return out
def verify(bin_path,syx_path):
    evidence=ROOT/'evidence'
    expected=json.loads((evidence/'extraction_report.json').read_text())
    flash=pathlib.Path(bin_path).read_bytes();syx=pathlib.Path(syx_path).read_bytes()
    assert hashlib.sha256(flash).hexdigest()==expected['bin_sha256'],'Wrong BIN version'
    assert hashlib.sha256(syx).hexdigest()==expected['syx_sha256'],'Wrong SYX version'
    base,raw=unpack_syx(syx)
    assert base==expected['container_syx_address']
    offset=expected['container_bin_offset']
    assert len(raw)==expected['container_bytes'] and flash[offset:offset+len(raw)]==raw
    sections=[]
    for entry in expected['sections']:
        p=entry['offset']-offset
        n=int.from_bytes(raw[p:p+4],'big');checksum=int.from_bytes(raw[p+4:p+8],'big')
        stream=raw[p+8:p+8+n]
        assert n==entry['compressed'] and sum(stream)==checksum
        data=depack(stream)
        assert len(data)==entry['expanded'] and hashlib.sha256(data).hexdigest()==entry['sha256']
        sections.append(data)
    maps=[{},{},{}]
    for section in (sections[1],sections[3]):
        for t,a,words in records(section):maps[t].update({a+i:v for i,v in enumerate(words)})
    packaged=json.loads((evidence/'memory.json').read_text())
    for space,words in enumerate(packaged):
        for a,v in words.items():assert maps[space][int(a)]==v,hex(int(a))
    print('PASS: original BIN = SYX container; all packaged instructions and tables match firmware.')

if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description='Verify package against user-supplied original OS 1.32B files; no compiling.')
    p.add_argument('--bin',required=True);p.add_argument('--syx',required=True)
    a=p.parse_args();verify(a.bin,a.syx)
