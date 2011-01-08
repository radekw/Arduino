import serial

CMD_START              = 250
CMD_SET_INTENSITY      = 10
CMD_SET_COLOR          = 20
CMD_CROSSFADE          = 30
CMD_SET_CROSSFADE_WAIT = 31
CMD_SET_CROSSFADE_HOLD = 32

class RemLight:
    def __init__(self, dev):
        self._serial = serial.Serial(dev, 9600)

    def _sendByteArray(self, ba):
        return self._serial.write(bytearray(ba))

    def _constrain(self, color, max=100):
        if color < 0:
            color = 0
        elif color > max:
            color = max
        return color

    def setIntensity(self, intensity):
        cmd = [CMD_START, 2, CMD_SET_INTENSITY, self._constrain(intensity)]
        self._sendByteArray(cmd)

    def setColor(self, red, green, blue):
        cmd = [CMD_START, 4, CMD_SET_COLOR, 
               self._constrain(red), 
               self._constrain(green), 
               self._constrain(blue)]
        self._sendByteArray(cmd)

    def crossfade(self, colors):
        size = 0
        data = []
        for c in colors:
            data.append(self._constrain(c[0]))
            data.append(self._constrain(c[1]))
            data.append(self._constrain(c[2]))
            size += 3
        cmd = [CMD_START, size + 1, CMD_CROSSFADE] + data
        self._sendByteArray(cmd)

    def setCrossfadeWait(self, wait):
        cmd = [CMD_START, 2, CMD_SET_CROSSFADE_WAIT, 
               self._constrain(wait, max=255)]
        self._sendByteArray(cmd)

    def setCrossfadeHold(self, hold):
        cmd = [CMD_START, 2, CMD_SET_CROSSFADE_HOLD, 
               self._constrain(hold, max=255)]
        self._sendByteArray(cmd)

