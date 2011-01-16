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

import serial, sys, time, datetime, optparse

parser = optparse.OptionParser()
parser.add_option('-d', '--device', action='store', type='string', 
                  dest='device', default='/dev/tty.usbmodem411')
parser.add_option('-s', '--string', action='store', type='string', 
                  dest='string', default=None)
parser.add_option('-n', '--ntp', action='store_true', dest='ntp', 
                  default=False)
parser.add_option('--noreadback', action='store_false', dest='readback', 
                  default=True)
(options, args) = parser.parse_args()

# open serial connection
serial = serial.Serial(options.device, 9600, timeout=0)
# my arduino resets on serial open
# read the buffer and sleep some
serial.read(1000)
time.sleep(3)

if options.ntp:
    print 'Setting time from NTP server'
    import ntplib
    c = ntplib.NTPClient()
    r = c.request('pool.ntp.org', version=3)
    dt = datetime.datetime.fromtimestamp(round(r.tx_time))
elif options.string:
    print 'Setting time from string %s' % options.string
    dt = datetime.datetime.strptime(options.string, '%Y-%m-%d %H:%M:%S')
else:
    print 'Setting time from local time'
    dt = datetime.datetime.now()
    dt = dt.replace(second=int(round(dt.second + (float(dt.microsecond) / 1000000))), microsecond=0)
    
tm = 'T%s' % dt.strftime('%Y%m%d%H%M%S')
serial.write(tm)
print 'Time has been set to %s' % dt.isoformat(' ')

if options.readback:
    print 'Reading time back'
    # read crap from the buffer
    serial.read(1000)
    time.sleep(1)

    # keep reading and displaying time
    for i in range(0, 5):
        sys.stdout.write(serial.readline())
        time.sleep(1)

