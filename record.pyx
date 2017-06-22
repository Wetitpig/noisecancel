#!/usr/bin/env python

# Compile with: cython record.pyx --embed

import alsaaudio
import audioop
from cpython.exc cimport PyErr_CheckSignals

inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE, alsaaudio.PCM_NONBLOCK, device='default')
inp.setchannels(1)
inp.setrate(44100)
inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)
inp.setperiodsize(64)

oup = alsaaudio.PCM(alsaaudio.PCM_PLAYBACK, alsaaudio.PCM_NONBLOCK, device='default')
oup.setchannels(1)
oup.setperiodsize(64)
oup.setrate(44100)
oup.setformat(alsaaudio.PCM_FORMAT_S16_LE)

while True:
    oup.write(audioop.reverse(inp.read()[1], 2))
    PyErr_CheckSignals()
