import os
class SketchManager(object):
    
    def __init__(self, f):
        self.sketch_file = f
        self.lines = [line.rstrip('\n') for line in sketch_file]
        self.line_org = {
            "imports": 0,
            "member_variables": 0,
            "setup_leds": 0,
            "setup_dhts": 0,
            
        }
        counter = 0
        print(self.lines)
        for i in self.lines:
            if i.find("//==========") != -1:
                line = i.split(" ")
                part = line[1]
                self.line_org[part] = counter
            counter += 1
        print(self.line_org)

    def setup_leds(self, led_arr):
        if self.line_org['member_variables'] == 0:
            


print(os.path.isfile("data/sketch_template.ino"))
sketch_file = open("data/sketch_template.ino", "r")
SketchManager(sketch_file)


    
