#!/usr/bin/python

from RemLight import RemLight

rl = RemLight('/dev/tty.usbserial-A700eCvT')
rl.crossfade(((100, 0, 0), (0, 100, 0), (0, 0, 100)))

