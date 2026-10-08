// Fallback preview renderer for environments without a C++ compiler.
const fs = require('fs');

const sampleRate = 48000;
const seconds = 12;
const total = sampleRate * seconds;
const pi = Math.PI;
const ratios = [0.33, 0.50, 0.66, 1.00, 1.11, 1.45, 2.22, 3.33];
const size = Math.ceil(sampleRate * 4.0) + 4;
const voices = ratios.map(() => ({
  d1: new Float32Array(size), d2: new Float32Array(size),
  damp: 0, ap: 0, dcIn: 0, dcOut: 0, phase: 0
}));
let writeIndex = 0;

function read(buffer, delaySamples) {
  delaySamples = Math.max(1, Math.min(delaySamples, buffer.length - 2));
  let position = writeIndex - delaySamples;
  while (position < 0) position += buffer.length;
  const i0 = Math.floor(position) % buffer.length;
  const i1 = (i0 + 1) % buffer.length;
  const fraction = position - Math.floor(position);
  return buffer[i0] + fraction * (buffer[i1] - buffer[i0]);
}

function processSample(input, scan) {
  const gain = 0.99;
  const damp = 0.90;
  const phaseAmount = 0.75;
  const delay1 = 115;
  const delay2 = 500;
  const feedback = 0.55 + 0.435 * gain;
  const outputs = [];

  for (let i = 0; i < voices.length; i++) {
    const voice = voices[i];
    voice.phase += (0.015 + 0.025 * phaseAmount) * (1 + 0.08 * i) / sampleRate;
    if (voice.phase >= 2 * pi) voice.phase -= 2 * pi;
    const movement = Math.sin(voice.phase) * (0.015 + 0.04 * phaseAmount);
    const ratio = ratios[i] * (1 + movement);
    const time1 = Math.max(2, delay1 * ratio);
    const time2 = Math.max(time1 + 1, delay2 * ratio);
    const tap1 = read(voice.d1, time1 * sampleRate / 1000);
    const tap2 = read(voice.d2, time2 * sampleRate / 1000);
    const comb = 0.5 * (tap1 + tap2);
    voice.damp += (1 - damp) * (comb - voice.damp);
    const coeff = Math.max(-0.92, Math.min(0.92, 0.12 + 0.80 * phaseAmount
      + 0.04 * Math.sin(voice.phase * 1.7)));
    const apOut = -coeff * voice.damp + voice.ap;
    voice.ap = voice.damp + coeff * apOut;
    voice.d1[writeIndex] = input * 0.18 + feedback * (0.78 * voice.damp + 0.22 * apOut);
    voice.d2[writeIndex] = input * 0.18 + feedback * (0.52 * voice.damp + 0.48 * apOut);
    const dc = 0.36 * comb + 0.64 * apOut - voice.dcIn + 0.995 * voice.dcOut;
    voice.dcIn = 0.36 * comb + 0.64 * apOut;
    voice.dcOut = dc;
    outputs[i] = dc;
  }

  const position = scan * 7;
  const first = Math.floor(position);
  const second = (first + 1) % 8;
  const fraction = position - first;
  const panFirst = first / 7;
  const panSecond = second / 7;
  const left = 0.86 * ((1 - fraction) * outputs[first] * Math.cos(panFirst * pi * 0.5)
    + fraction * outputs[second] * Math.cos(panSecond * pi * 0.5));
  const right = 0.86 * ((1 - fraction) * outputs[first] * Math.sin(panFirst * pi * 0.5)
    + fraction * outputs[second] * Math.sin(panSecond * pi * 0.5));
  writeIndex = (writeIndex + 1) % size;
  return [left, right];
}

const left = new Float32Array(total);
const right = new Float32Array(total);
for (let sample = 0; sample < total; sample++) {
  const time = sample / sampleRate;
  const section = Math.floor(time / 2);
  const local = time - section * 2;
  const envelope = Math.exp(-local * 2.1);
  const source = (0.24 * Math.sin(2 * pi * (146.83 + section * 19) * time)
    + 0.12 * Math.sin(2 * pi * (220 + section * 23) * time)) * envelope
    + (time % 2 < 0.006 ? 0.55 * Math.exp(-local * 20) : 0);
  const scan = (time / seconds) % 1;
  const wet = processSample(source, scan);
  left[sample] = 0.16 * source + 0.84 * wet[0];
  right[sample] = 0.16 * source * 0.94 + 0.84 * wet[1];
}

const data = Buffer.alloc(total * 4);
for (let i = 0; i < total; i++) {
  data.writeInt16LE(Math.max(-32768, Math.min(32767, Math.round(left[i] * 32767))), i * 4);
  data.writeInt16LE(Math.max(-32768, Math.min(32767, Math.round(right[i] * 32767))), i * 4 + 2);
}
const header = Buffer.alloc(44);
header.write('RIFF', 0); header.writeUInt32LE(36 + data.length, 4); header.write('WAVE', 8);
header.write('fmt ', 12); header.writeUInt32LE(16, 16); header.writeUInt16LE(1, 20);
header.writeUInt16LE(2, 22); header.writeUInt32LE(sampleRate, 24); header.writeUInt32LE(sampleRate * 4, 28);
header.writeUInt16LE(4, 32); header.writeUInt16LE(16, 34); header.write('data', 36); header.writeUInt32LE(data.length, 40);
fs.writeFileSync(process.argv[2] || 'comb_scanner_demo.wav', Buffer.concat([header, data]));
