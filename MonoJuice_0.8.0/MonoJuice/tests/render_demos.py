#!/usr/bin/env python3
"""Run FactoryTests with an output prefix first; convert its float demos to RMS-matched WAVs.
Requires Python + numpy for this optional export only, not for the C++ plugin.
Usage (project root): build/native/FactoryTests audit/presets && python tests/render_demos.py
"""
from pathlib import Path
import hashlib
import json
import wave
import numpy as np
root=Path(__file__).resolve().parents[1]
sr=44100
meta={'version':'0.8.0','sample_rate':sr,'pcm_bits':24,'synthetic_input':'testInput in tests/PatchSettings.h; 2 seconds excitation + 2 seconds tail per segment','notes':'Constant gain per segment, RMS matched. No compression/limiting in WAV export. Compares timbre, not original output level. Native Slot only: master loop/DC/clipper excluded. Slot ON amount starts fully wet for every segment.','files':{}}
for kind,name in [('COMB_AB','MonoJuice_COMB_07_vs_08'),('CLASSIC','MonoJuice_SPECTRE_CLASSIC_1-10')]:
    path=root/'audit'/f'presets_{kind}.raw'
    a=np.fromfile(path,dtype='<f4').reshape(-1,2).astype(np.float64)
    assert np.isfinite(a).all()
    b=a.copy();segments=[]
    for n in range(len(a)//(4*sr)):
        v=a[n*4*sr:(n+1)*4*sr]
        rms=float(np.sqrt(np.mean(v*v)));gain=10**(-24/20)/max(1e-12,rms)
        b[n*4*sr:(n+1)*4*sr]*=gain
        segments.append({'start_seconds':n*4,'preset':n//2+1 if kind=='COMB_AB' else n+1,'version':('0.7' if n%2==0 else '0.8') if kind=='COMB_AB' else 'CLASSIC 0.5','gain_db_before_common_attenuation':20*np.log10(gain),'raw_peak':float(np.max(np.abs(v)))})
    common=min(1.,10**(-3/20)/float(np.max(np.abs(b))));b*=common
    ints=np.rint(b*(2**23-1)).astype('<i4').reshape(-1)
    data=np.column_stack((ints&255,(ints>>8)&255,(ints>>16)&255)).astype('uint8').tobytes()
    output=root/'Preview'/f'{name}.wav';output.parent.mkdir(exist_ok=True)
    with wave.open(str(output),'wb') as w:
        w.setnchannels(2);w.setsampwidth(3);w.setframerate(sr);w.writeframes(data)
    meta['files'][output.name]={'seconds':len(a)/sr,'raw_sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'wav_sha256':hashlib.sha256(output.read_bytes()).hexdigest(),'common_attenuation_db':20*np.log10(common),'peak_dbfs':20*np.log10(np.max(np.abs(b))),'segments':segments}
(root/'audit/preset-demos.json').write_text(json.dumps(meta,indent=2)+'\n')
print('WAV export PASS; source raw files may now be removed.')
