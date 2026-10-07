const fs = require('fs');

const sampleRate = 48000;
const seconds = 15;
const total = sampleRate * seconds;
const pi = Math.PI;
const twoPi = 2 * pi;
const names = [
  '01_original_hypothesis', '02_tight_crossfade', '03_long_resonator', '04_scrub_stretch',
  '05_phase_cloud', '06_ping_pong', '07_dark_bloom', '08_bright_teeth',
  '09_unstable_edge', '10_balanced_matrix'
];
const configs = [
  { r: [0.33,0.50,0.66,1,1.11,1.45,2.22,3.33], ff:0.55, fr:0.435, curve:1, mod:0.045, ap:0.12, apd:0.80, cross:0, pan:0.86, wet:0.86, rev:0, alt:0 },
  { r: [0.25,0.375,0.50,0.667,0.75,1,1.333,1.50], ff:0.58, fr:0.38, curve:1, mod:0.025, ap:0.32, apd:0.70, cross:0.15, pan:0.90, wet:0.84, rev:0, alt:0 },
  { r: [0.50,0.75,1,1.25,1.50,1.875,2.25,2.75], ff:0.62, fr:0.34, curve:1, mod:0.035, ap:0.66, apd:0.72, cross:0.20, pan:0.92, wet:0.86, rev:0, alt:0 },
  { r: [0.33,0.50,0.66,1,1.11,1.45,2.22,3.33], ff:0.54, fr:0.42, curve:0.55, mod:0.14, ap:0.24, apd:0.78, cross:0.10, pan:1.00, wet:0.86, rev:0, alt:0 },
  { r: [0.33,0.50,0.75,0.90,1.20,1.60,2.40,3.10], ff:0.55, fr:0.40, curve:0.80, mod:0.20, ap:0.48, apd:0.92, cross:0.35, pan:1.05, wet:0.88, rev:0, alt:0 },
  { r: [0.33,0.50,0.66,1,1.11,1.45,2.22,3.33], ff:0.56, fr:0.40, curve:1, mod:0.055, ap:0.40, apd:0.78, cross:0.20, pan:1.35, wet:0.88, rev:0, alt:1 },
  { r: [0.40,0.60,0.80,1,1.30,1.80,2.50,3.20], ff:0.64, fr:0.30, curve:1.10, mod:0.018, ap:0.62, apd:0.42, cross:0.20, pan:0.76, wet:0.90, rev:0, alt:0 },
  { r: [0.28,0.44,0.59,0.88,1.07,1.34,2.08,3.00], ff:0.48, fr:0.47, curve:0.92, mod:0.065, ap:0.10, apd:0.88, cross:0.30, pan:1.12, wet:0.84, rev:0, alt:0 },
  { r: [0.33,0.50,0.66,1,1.11,1.45,2.22,3.33], ff:0.70, fr:0.27, curve:1.35, mod:0.09, ap:0.72, apd:0.86, cross:0.42, pan:0.98, wet:0.90, rev:1, alt:0 },
  { r: [0.33,0.50,0.66,1,1.11,1.45,2.22,3.33], ff:0.57, fr:0.41, curve:0.85, mod:0.045, ap:0.36, apd:0.78, cross:0.65, pan:1.00, wet:0.88, rev:0, alt:0 }
];

function loadSource(filePath) {
  const data = fs.readFileSync(filePath);
  if (data.toString('ascii', 0, 4) !== 'RIFF' || data.toString('ascii', 8, 12) !== 'WAVE')
    throw new Error(`${filePath}: expected a RIFF/WAVE file`);
  let channels = 0;
  let bits = 0;
  let format = 0;
  let sampleRateInFile = 0;
  let dataOffset = 12;
  let dataBytes = 0;
  while (dataOffset + 8 <= data.length) {
    const chunk = data.toString('ascii', dataOffset, dataOffset + 4);
    const size = data.readUInt32LE(dataOffset + 4);
    if (chunk === 'fmt ') {
      format = data.readUInt16LE(dataOffset + 8);
      channels = data.readUInt16LE(dataOffset + 10);
      sampleRateInFile = data.readUInt32LE(dataOffset + 12);
      bits = data.readUInt16LE(dataOffset + 22);
    }
    if (chunk === 'data') { dataOffset += 8; dataBytes = size; break; }
    dataOffset += 8 + size + (size & 1);
  }
  if (format !== 1 || bits !== 16 || dataBytes === 0)
    throw new Error(`${filePath}: expected a 16-bit PCM WAV`);
  const frames = Math.floor(dataBytes / (channels * 2));
  const input = new Float32Array(frames);
  for (let frame = 0; frame < frames; frame++) {
    let sum = 0;
    for (let channel = 0; channel < channels; channel++)
    sum += data.readInt16LE(dataOffset + (frame * channels + channel) * 2) / 32768;
    input[frame] = sum / channels;
  }
  const output = new Float32Array(Math.ceil(frames * sampleRate / sampleRateInFile));
  for (let frame = 0; frame < output.length; frame++) {
    const position = frame * sampleRateInFile / sampleRate;
    const first = Math.floor(position);
    const second = Math.min(first + 1, input.length - 1);
    output[frame] = input[first] + (input[second] - input[first]) * (position - first);
  }
  return output;
}

function readDelay(buffer, writeIndex, delaySamples) {
  delaySamples = Math.max(1, Math.min(delaySamples, buffer.length - 2));
  let position = writeIndex - delaySamples;
  while (position < 0) position += buffer.length;
  const i0 = Math.floor(position) % buffer.length;
  const i1 = (i0 + 1) % buffer.length;
  const fraction = position - Math.floor(position);
  return buffer[i0] + fraction * (buffer[i1] - buffer[i0]);
}

function renderVariant(config) {
  const size = Math.ceil(sampleRate * 7.0) + 4;
  const voices = config.r.map((_, index) => ({
    d1: new Float32Array(size), d2: new Float32Array(size),
    damp: 0, ap: 0, dcIn: 0, dcOut: 0, phase: index * 0.73
  }));
  let writeIndex = 0;
  let smoothedScan = 0;
  let smoothedWetLeft = 0;
  let smoothedWetRight = 0;
  const left = new Float32Array(total);
  const right = new Float32Array(total);

  function processSample(input, scan) {
    const character = config.character ?? 0.55;
    const feedback = config.ff + config.fr * 0.99;
    const outputs = [];
    let previous = 0;
    for (let i = 0; i < voices.length; i++) {
      const voice = voices[i];
      voice.phase += (0.012 + 0.028 * 0.75) * (1 + 0.08 * i) / sampleRate;
      if (voice.phase >= twoPi) voice.phase -= twoPi;
      const ratio = config.r[i] * (1 + Math.sin(voice.phase) * config.mod
        * (0.45 + 1.10 * character));
      const time1 = Math.max(1000 / sampleRate, 115 * ratio);
      const time2 = Math.max(1000 / sampleRate, 500 * ratio);
      const tap1 = readDelay(voice.d1, writeIndex, time1 * sampleRate / 1000);
      const tap2 = readDelay(voice.d2, writeIndex, time2 * sampleRate / 1000);
      const raw = 0.5 * (tap1 + tap2);
      const crossMix = Math.max(0, Math.min(0.92, config.cross + 0.24 * (character - 0.5)));
      const comb = raw * (1 - crossMix) + previous * crossMix;
      voice.damp += (0.08 + 0.20 * (1 - 0.90) + 0.04 * character) * (comb - voice.damp);
      const resonant = 0.70 * comb + 0.30 * voice.damp;
      const coeff = Math.max(-0.92, Math.min(0.92, config.ap + config.apd * 0.75
        + 0.16 * (character - 0.5)
        + 0.04 * Math.sin(voice.phase * 1.7)));
      const apOut = -coeff * resonant + voice.ap;
      voice.ap = resonant + coeff * apOut;
      voice.d1[writeIndex] = Math.max(-1.5, Math.min(1.5, input * 0.42
        + feedback * (0.72 * resonant + 0.28 * apOut)));
      voice.d2[writeIndex] = Math.max(-1.5, Math.min(1.5, input * 0.42
        + feedback * (0.52 * resonant + 0.48 * apOut)));
      const rawOut = 0.72 * comb + 0.28 * apOut;
      const output = rawOut - voice.dcIn + 0.995 * voice.dcOut;
      voice.dcIn = rawOut;
      voice.dcOut = output;
      outputs[i] = output;
      previous = output;
    }

    const scanTime = 0.030 - 0.008 * character;
    smoothedScan += (1 - Math.exp(-1 / (scanTime * sampleRate))) * (scan - smoothedScan);
    let position = Math.pow(smoothedScan, config.curve) * 7;
    if (config.rev) position = (1 - smoothedScan) * 7;
    const first = Math.min(7, Math.floor(position));
    const second = (first + 1) % 8;
    const fraction = position - first;
    function panFor(index) {
      let pan = index / 7;
      if (config.alt && index % 2) pan = 1 - pan;
      return Math.max(0, Math.min(1, 0.5 + (pan - 0.5) * config.pan));
    }
    const p0 = panFor(first), p1 = panFor(second);
    const firstWeight = Math.cos(fraction * pi * 0.5);
    const secondWeight = Math.sin(fraction * pi * 0.5);
    const wetLeft = config.wet * (firstWeight * outputs[first] * Math.cos(p0 * pi * 0.5)
      + secondWeight * outputs[second] * Math.cos(p1 * pi * 0.5));
    const wetRight = config.wet * (firstWeight * outputs[first] * Math.sin(p0 * pi * 0.5)
      + secondWeight * outputs[second] * Math.sin(p1 * pi * 0.5));
    const wetSmoothing = 1 - Math.exp(-1 / (0.00008 * sampleRate));
    smoothedWetLeft += wetSmoothing * (wetLeft - smoothedWetLeft);
    smoothedWetRight += wetSmoothing * (wetRight - smoothedWetRight);
    writeIndex = (writeIndex + 1) % size;
    return [smoothedWetLeft, smoothedWetRight];
  }

  for (let sample = 0; sample < total; sample++) {
    const scan = 0.5 - 0.5 * Math.cos(twoPi * (sample / sampleRate) / 6);
    const input = sourceAt(sample);
    const wet = processSample(input, scan);
    left[sample] = Math.tanh(4.5 * (0.12 * input + 0.88 * wet[0]));
    right[sample] = Math.tanh(4.5 * (0.12 * input * 0.94 + 0.88 * wet[1]));
  }
  return { left, right };
}

function writeWav(path, left, right) {
  const data = Buffer.alloc(left.length * 4);
  for (let i = 0; i < left.length; i++) {
    data.writeInt16LE(Math.max(-32768, Math.min(32767, Math.round(left[i] * 32767))), i * 4);
    data.writeInt16LE(Math.max(-32768, Math.min(32767, Math.round(right[i] * 32767))), i * 4 + 2);
  }
  const header = Buffer.alloc(44);
  header.write('RIFF', 0); header.writeUInt32LE(36 + data.length, 4); header.write('WAVE', 8);
  header.write('fmt ', 12); header.writeUInt32LE(16, 16); header.writeUInt16LE(1, 20);
  header.writeUInt16LE(2, 22); header.writeUInt32LE(sampleRate, 24); header.writeUInt32LE(sampleRate * 4, 28);
  header.writeUInt16LE(4, 32); header.writeUInt16LE(16, 34); header.write('data', 36);
  header.writeUInt32LE(data.length, 40);
  fs.writeFileSync(path, Buffer.concat([header, data]));
}

const root = process.argv[2] || '.';
const sourcePath = process.argv[3];
if (!sourcePath)
  throw new Error('Pass a real WAV source: node render_variants.js <output-dir> <source.wav> [start-seconds]');
const source = loadSource(sourcePath);
const sourceStart = Math.max(0, Math.floor(Number(process.argv[4] || 0) * sampleRate));
const characterOverride = process.argv[5] === undefined
  ? null : Math.max(0, Math.min(1, Number(process.argv[5])));
function sourceAt(sample) {
  return source[(sample + sourceStart) % source.length];
}

for (let i = 0; i < configs.length; i++) {
  const config = characterOverride === null
    ? configs[i] : { ...configs[i], character: characterOverride };
  const rendered = renderVariant(config);
  writeWav(`${root}/${names[i]}.wav`, rendered.left, rendered.right);
  console.log(`Rendered ${names[i]}`);
}
