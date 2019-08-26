import os
from LumiduinoSketchManager.models.Lightstrip import LightStrip
class SketchManager(object):
    
    def __init__(self, filename):
        f = open("data/sketch_template.ino", "r")
        self.sketch_file = f
        self.lines = [line.rstrip('\n') for line in self.sketch_file]
        self.line_org = {
            "imports": 0,
            "member_variables": 0,
            "setup": 0
        }
        self.sections_text = {
            "imports": "",
            "member_variables": "",
            "setup": ""
        }
        counter = 0
        for i in self.lines:
            if i.find("//==========") != -1:
                i = i.lstrip('\t')
                line = i.split(" ")
                print(line)
                index = line.index("//==========")
                part = line[index+1]
                self.line_org[part] = counter
            counter += 1
        print(self.line_org)

    def setup_leds(self, led_arr: [LightStrip]):
        if self.line_org['member_variables'] == 0:
            return
        strip_amt = len(led_arr)
        self.sections_text['member_variables'] += "int strip_amt = "+str(strip_amt)+";\n"
        led_arr.sort(key=lambda x: x.length)
        names = ""
        lengths = ""
        for i, strip in enumerate(led_arr):
            self.sections_text['member_variables'] += "CRGB strip{}[{}];\n".format(i, strip.get_length())
            names += "strip{},".format(i)
            lengths += "{},".format(strip.get_length())
            self.sections_text['setup'] += "FastLED.addLeds<{}, {}>(strip{}, {});\n".format(
                strip.get_type(),
                strip.get_pin(),
                i,
                strip.get_length()
            )
        names.rstrip(",")
        lengths.rstrip(",")
        print(names)
        self.sections_text['member_variables'] += "CRGB* strips[] = {"+names+"};\n"
        self.sections_text['member_variables'] += "int lengths[] = {"+lengths+"};\n"
    
    def setup_dhts(self, dht_arr):
        pass

    def retrieve_lines(self):
        self.lines[self.line_org['imports']] = self.sections_text['imports']
        self.lines[self.line_org['member_variables']] = self.sections_text['member_variables']
        self.lines[self.line_org['setup']] = self.sections_text['setup']
        return self.lines


    
