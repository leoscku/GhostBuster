#!/usr/bin/env python3
"""Synthesize placeholder sound effects for GhostBuster using only the stdlib.

Outputs:
  gun.wav        - short punchy gunshot (one-shot, played on fire)
  background.wav - seamless looping ambient drone + wind bed

The originals (gun.mp3 / background.mp3) are gitignored and absent from the repo,
so these stand in. Replace with your own files of the same name if you prefer.
"""
import math
import random
import struct
import wave

SR = 44100


def write_wav(path, samples):
    """samples: list of floats in [-1, 1] -> 16-bit mono PCM."""
    peak = max(1e-9, max(abs(s) for s in samples))
    norm = 0.95 / peak if peak > 0.95 else 1.0
    frames = bytearray()
    for s in samples:
        v = int(max(-1.0, min(1.0, s * norm)) * 32767)
        frames += struct.pack('<h', v)
    with wave.open(path, 'wb') as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(bytes(frames))
    print(f"wrote {path}  ({len(samples)/SR:.2f}s, {len(samples)} frames)")


def gunshot():
    random.seed(1234)
    dur = 0.32
    n = int(SR * dur)
    out = []
    for i in range(n):
        t = i / SR
        # Fast-decaying white-noise crack
        crack = (random.uniform(-1, 1)) * math.exp(-t * 38.0)
        # Low-frequency body "thump" with its own decay
        thump = math.sin(2 * math.pi * 70 * t) * math.exp(-t * 22.0) * 0.9
        # Sharp transient click at the very start
        click = math.exp(-t * 900.0) * (1.0 if i < 40 else 0.0)
        out.append(crack * 0.8 + thump + click)
    return out


def ambience():
    random.seed(99)
    dur = 8.0                     # loop length (seconds)
    n = int(SR * dur)
    xfade = int(SR * 0.6)         # crossfade window for a seamless wind seam

    # Tonal drone: partials snapped to the loop's fundamental grid so each
    # completes a whole number of cycles in `dur` -> the tonal layer is exactly
    # periodic and loops with zero seam.
    base = 1.0 / dur              # 0.125 Hz grid
    partials = [(55, 0.16), (82.5, 0.10), (110, 0.08), (164.8, 0.05)]
    partials = [(round(f / base) * base, a) for f, a in partials]

    # One-pole low-passed white noise -> wind. Generate `n + xfade` samples so we
    # have the natural continuation past the loop point to crossfade against.
    total = n + xfade
    wind = []
    prev = 0.0
    for _ in range(total):
        prev = prev + 0.0025 * (random.uniform(-1, 1) - prev)   # gentle low-pass
        wind.append(prev)

    def sig(i):
        t = i / SR
        drone = 0.0
        lfo = 0.8 + 0.2 * math.sin(2 * math.pi * base * t)       # slow tremolo
        for f, a in partials:
            drone += math.sin(2 * math.pi * f * t) * a * lfo
        swell = 0.55 + 0.45 * math.sin(2 * math.pi * base * t - 1.0)
        return drone + wind[i] * 6.0 * swell

    # Build the loop. The body is sig() directly; the first `xfade` samples blend
    # the head with the signal's natural continuation (indices n..n+xfade) so that
    # sample n-1 -> sample 0 has no discontinuity.
    out = [0.0] * n
    for k in range(xfade):
        a = k / xfade
        out[k] = sig(k) * a + sig(n + k) * (1 - a)
    for k in range(xfade, n):
        out[k] = sig(k)

    return [s * 0.5 for s in out]   # sit under gameplay


if __name__ == '__main__':
    write_wav('gun.wav', gunshot())
    write_wav('background.wav', ambience())
