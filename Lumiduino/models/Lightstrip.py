class LightStrip(object):
    def __init__(self, pin_number, length, type_l="NEOPIXEL"):
        self.pin = pin_number
        self.length = length
        self.type = type_l

    def get_pin(self):
        return self.pin
    
    def get_length(self):
        return self.length
    
    def get_type(self):
        return self.type