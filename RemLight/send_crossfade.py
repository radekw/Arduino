#!/usr/bin/python

# Copyright, 2011 Radek Wierzbicki
#   
# This file is part of RemLight.
#
# RemLight is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RemLight is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with RemLight.  If not, see <http://www.gnu.org/licenses/>.

import sys
from RemLight import RemLight

#tty = '/dev/tty.usbserial-A900frow'
tty = '/dev/tty.usbserial-A700eCvT'
dest_addr_long = '\x00\x13\xA2\x00\x40\x61\x1A\xC9'

if len(sys.argv) < 4:
    print 'need 3 values from 0 to 100 for red, green, and blue'
    sys.exit(1)

try:
    red = int(sys.argv[1])
    green = int(sys.argv[2])
    blue = int(sys.argv[3])
except:
    print 'error parsing arguments'
    sys.exit(1)

rl = RemLight(tty, dest_addr_long)
rl.crossfade(((red, green, blue),))

