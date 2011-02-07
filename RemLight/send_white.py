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

from RemLight import RemLight

rl = RemLight('/dev/tty.usbserial-A700eCvT')
#rl = RemLight('/dev/tty.usbserial-A900frow')
rl.setColor(100, 100, 100)

