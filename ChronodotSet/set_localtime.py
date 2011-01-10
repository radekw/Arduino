#!/usr/bin/python

# Copyright, 2011 Radek Wierzbicki
#   
# This file is part of ChronodotSet.
#
# ChronodotSet is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ChronodotSet is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ChronodotSet.  If not, see <http://www.gnu.org/licenses/>.

import serial, sys, time

# get TZ offset from command line param
tzoffset = 0
if len(sys.argv) > 0:
    try:
        tzoffset = int(sys.argv[1])
    except:
        tzoffset = 0

if tzoffset:
    print 'TZ offset: %s' % tzoffset

# open serial connection
serial = serial.Serial('/dev/tty.usbmodem411', 9600, timeout=0)
# my arduino resets on serial open
# read the buffer and sleep some
serial.read(1000)
time.sleep(2)

# get the timestamp and write to serial
ts = int(time.time()) + (tzoffset * 60 * 60)
tm = 'T%s' % ts
serial.write(tm)

# read crap from the buffer
serial.read(1000)
print 'Set time to %s' % tm

# keep reading and displaying time
while(True):
    sys.stdout.write(serial.readline())

