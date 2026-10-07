// Fallback preview renderer for environments without a C++ compiler.
const fs = require('fs');

const sampleRate = 48000;
const seconds = 12;
const total = sampleRate * seconds;
const pi = Math.PI;
const base = [43, 67, 97, 139];
const delaySize = Math.ceil(sampleRate * 3.0) + 4;
const delays = base.map(() => new Float32Array(delaySize));
const allpassSizes = [17, 31, 53, 79].map(ms => Math.ceil(sampleRate * ms / 1000) + 4);
const allpasses = allpassSizes.map(size => new Float32Array(size));
const allpassFeedback = [0.58, 0.63, 0.69, 0.74];
let writeIndex = 0;
let allpassIndex = [0, 0, 0, 0];
let dampingState = [0, 0, 0, 0];
let phase = [0, 1.7, 3.1, 4.8];
let randomState = 0.1234567;
let rotatePhase = 0;

function noise() {
  randomState = (randomState * 3.9898 + 0.2113) % 1;
  if (randomState < 0) randomState += 1;
  return randomState * 2 - 1;
}

function read(buffer, delaySamples) {
  delaySamples = Math.max(1, Math.min(delaySamples, buffer.length - 2));
  let position = writeIndex - delaySamples;
  while (position < 0) position += buffer.length;
  const i0 = Math.floor(position) % buffer.length;
  const i1 = (i0 + 1) % buffer.length;
  const f = position - Math.floor(position);
  return buffer[i0] + f * (buffer[i1] - buffer[i0]);
}

function processSample(input) {
  const decay = 29.74 / 127;
  const rt60 = 0.25 + 8 * Math.pow(decay, 0.78);
  const damping = 0.10;
  const rotation = 0.25;
  const delayed = [];
  rotatePhase += (0.0025 + 0.015 * rotation) / sampleRate;
  if (rotatePhase >= 1) rotatePhase -= 1;

  for (let i = 0; i < 4; i++) {
    phase[i] += (0.017 + 0.003 * i) * (0.35 + rotation) / sampleRate;
    if (phase[i] >= 2 * pi) phase[i] -= 2 * pi;
    const delayMs = Math.max(2, Math.min(2990, base[i] * (0.78 + 0.22 * Math.sin(phase[i]))));
    let value = read(delays[i], delayMs * sampleRate / 1000);
    dampingState[i] += (1 - damping) * (value - dampingState[i]);
    delayed[i] = dampingState[i];
  }

  const h0 = 0.5 * (delayed[0] + delayed[1] + delayed[2] + delayed[3]);
  const h1 = 0.5 * (delayed[0] - delayed[1] + delayed[2] - delayed[3]);
  const h2 = 0.5 * (delayed[0] + delayed[1] - delayed[2] - delayed[3]);
  const h3 = 0.5 * (delayed[0] - delayed[1] - delayed[2] + delayed[3]);
  const angle = rotation * pi * 0.5 + rotatePhase * 2 * pi * 0.07;
  const s = Math.sin(angle), c = Math.cos(angle);
  const mixed = [h0 * c - h1 * s, h0 * s + h1 * c, h2 * c - h3 * s, h2 * s + h3 * c];
  const diffused = [];

  for (let i = 0; i < 4; i++) {
    const loopGain = Math.pow(10, -3 * (base[i] / 1000) / rt60);
    const weight = 0.88 + 0.12 * Math.sin(phase[i] * (1 + 0.17 * i));
    delays[i][writeIndex] = input * 0.52 + mixed[i] * loopGain * weight;
    const index = allpassIndex[i];
    const delayedAllpass = allpasses[i][index];
    const inner = delayed[i] - allpassFeedback[i] * delayedAllpass;
    diffused[i] = delayedAllpass + allpassFeedback[i] * inner;
    allpasses[i][index] = inner;
    allpassIndex[i] = (index + 1) % allpasses[i].length;
  }
  writeIndex = (writeIndex + 1) % delaySize;
  return [0.5 * (diffused[0] + diffused[2]), 0.5 * (diffused[1] + diffused[3])];
}

const left = new Float32Array(total);
const right = new Float32Array(total);
for (let sample = 0; sample < total; sample++) {
  const time = sample / sampleRate;
  const section = Math.floor(time / 2);
  const sectionTime = time - section * 2;
  const envelope = Math.exp(-sectionTime * 1.8);
  const source = (0.22 * Math.sin(2 * pi * (110 + section * 27) * time)
    + 0.15 * Math.sin(2 * pi * (164.81 + section * 31) * time)
    + 0.11 * Math.sin(2 * pi * (220 + section * 41) * time)) * envelope
    + (time % 2 < 0.006 ? 0.45 * Math.exp(-sectionTime * 18) : 0);
  const wet = processSample(source);
  left[sample] = 0.18 * source * 0.95 + 0.82 * wet[0];
  right[sample] = 0.18 * source * 0.90 + 0.82 * wet[1];
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
fs.writeFileSync(process.argv[2] || 'zigzag_demo.wav', Buffer.concat([header, data]));
