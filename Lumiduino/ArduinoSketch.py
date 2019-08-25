from models.Lightstrip import LightStrip
from models.DHTSensor import DHTSensor
import os

class ArduinoSketch(object):
    def __init__(self, build_dir: "../"):
        self.build_dir = build_dir
        self.sketch_file = None
        self.check_build_dir()
        self.create_sketch()
        option_dict = self.ask_setupoptions()


    def check_build_dir(self):
        if not os.path.isdir(self.build_dir):
            return False
        
        if not os.path.isdir(self.build_dir+"/build"):
            os.makedirs(self.build_dir+"/build")

        if not os.path.isdir(self.build_dir+"/build/sketch"):
            os.makedirs(self.build_dir+"/build/sketch")

        return True
    
    def create_sketch(self):
        self.sketch_file = open(self.build_dir+"/build/sketch/sketch.ino", "w+")
        return self.sketch_file

    @staticmethod
    def ask_resp_bool(message):
        resp = input(message+" [y/n]")
        resp = resp.lower()
        if resp.find("y"):
            return True
        else:
            return False
    
    @staticmethod
    def ask_resp_int(message):
        resp = input(message+" [#]")
        good = False
        while not good:
            try:
                resp = int(resp)
                good = True
            except:
                print("Im sorry that is not a number try again")

        return resp

    def ask_res_options(self, message, options):
        options_desc = "("
        for i in range(len(options)):
            options_desc += str(i)+": "+options[i]+", "
        options_desc +=")" 
        resp = input(message+" "+options_desc+" [#]")
        good = False
        while not good:
            try:
                resp = options[resp]
                good = True
            except:
                print("Im sorry that is not an option try again")

        return resp


    def ask_setupoptions(self):
        option_dict = {
            "imports": {
                "neopixels": self.ask_resp_bool("Include Lighting control?"),
                "dht sensors": self.ask_resp_bool("Include DHT Sensor control?"),
            },
            "neopixel_strips": [],
            "dht_sensors": [],
        }

        if option_dict['imports']['neopixels'] is True:
            strip_number = self.ask_resp_int("How many lighting strips will there be?")
            for i in range(strip_number):
                pin = self.ask_resp_int("What pin is strip "+str(i)+" on")
                length = self.ask_resp_int("How long is strip "+str(i))
                option_dict['neopixel_strips'].append(LightStrip(pin, length))

        if option_dict['imports']['dht sensors'] is True:
            sensor_number = self.ask_resp_int("How many DHT Sensors are there?")
            for i in range(sensor_number):
                pin = self.ask_resp_int("What pin is DHT "+str(i)+" on")
                typel = self.ask_res_options("What type is DHT "+str(i)+"?", ["DHT11", "DHT12"])
                option_dict['dht_sensors'].append(DHTSensor(pin, typel))
        return option_dict

    def write_arduino_sketch(self, f, options_dict):
        if options_dict['import']['neopixels']:
            f.write('#include "FastLed.h"\n')
        if options_dict['import']['dht sensors']:
            f.write('#include "DHT.h"\n')

        




