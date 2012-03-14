#!/usr/bin/python

import sys, serial, time, datetime, optparse
from xbee import ZigBee

tty = '/dev/tty.usbserial-A700eCvT'
dest_addr_long = '\x00\x13\xA2\x00\x40\x5D\xE4\x3C'
dest_addr='\xFF\xFE'

parser = optparse.OptionParser()
parser.add_option('-d', '--device', action='store', type='string', 
                  dest='device', default=tty)
parser.add_option('-s', '--string', action='store', type='string', 
                  dest='string', default=None)
parser.add_option('-n', '--ntp', action='store_true', dest='ntp', 
                  default=False)
parser.add_option('--noreadback', action='store_false', dest='readback', 
                  default=True)
(options, args) = parser.parse_args()

s = serial.Serial(options.device, 9600, timeout=0)
xbee = ZigBee(s)
xbee._escaped = True

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

tm = 'set T%s' % dt.strftime('%Y%m%d%H%M%S')
print dt
xbee.send('tx', data=tm, dest_addr_long=dest_addr_long, dest_addr=dest_addr)

response = xbee.wait_read_frame()

if options.readback:
    print 'Reading time'
    cmd = 'get'
    xbee.send('tx', data=cmd, dest_addr_long=dest_addr_long, dest_addr=dest_addr)

    for i in range(0, 5):
        response = xbee.wait_read_frame()
        if response['id'] == 'rx':
            dt_ret = response['rf_data']
            dt = datetime.datetime.strptime(dt_ret, 'T%Y%m%d%H%M%S')
            print dt
            break

sys.exit()

