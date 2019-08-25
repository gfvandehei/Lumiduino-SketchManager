class DHTSensor(object):
    def __init__(self, pin_number, type_l="DHT11"):
        self.pin = pin_number
        self.type = type_l

    def get_pin(self):
        return self.pin
    
    def get_type(self):
        return self.type