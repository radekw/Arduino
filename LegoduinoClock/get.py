#!/usr/bin/python

import sys, serial, time, datetime, optparse
from xbee import ZigBee

tty = '/dev/tty.usbserial-A700eCvT'
dest_addr_long = '\x00\x13\xA2\x00\x40\x5D\xE4\x3C'
dest_addr='\xFF\xFE'

parser = optparse.OptionParser()
parser.add_option('-d', '--device', action='store', type='string', 
                  dest='device', default=tty)
parser.add_option('-n', '--now', action='store_true', dest='now', 
                  default=False)
(options, args) = parser.parse_args()

serial = serial.Serial(options.device, 9600, timeout=0)
xbee = ZigBee(serial)
xbee._escaped = True

cmd = 'get'
if options.now:
    cmd += ' now'
xbee.send('tx', data=cmd, dest_addr_long=dest_addr_long, dest_addr=dest_addr)

for i in range(0, 5):
    response = xbee.wait_read_frame()
    if response['id'] == 'rx':
        dt_ret = response['rf_data']
        dt = datetime.datetime.strptime(dt_ret, 'T%Y%m%d%H%M%S')
        print dt
        break

sys.exit()

