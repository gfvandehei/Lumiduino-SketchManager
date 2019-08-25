import serial
import serial.tools.list_ports

class ArduinoSerial(object):
    connected_arduino = {}
    arduino_serial = {}
    options = {}
    
    def list_ports(self):
        comports = list(comport for comport in serial.tools.list_ports.comports())
        valid_ports = []
        for i in comports:
            if str(i.manufacturer).find("Arduino") != -1:
                valid_ports.append(i)
                self.options[i.device] = i
        return valid_ports
    
print(ArduinoSerial().list_ports())