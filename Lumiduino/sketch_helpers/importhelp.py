import json
optionsmapping = {
    "N": '"FastLED.h"', #include NEOPixels
    "D": '"DHT.h"',
}

class SetupParser(object):

    def __init__(self, setupjson_path):
        self.setupjson = json.load(setupjson_path) 

    def verify_imports(self):
        if self.setupjson.get('imports') is None:
            print("Setup json is required to have an imports area")
            exit(1)
def get_imports_from_string(setupstring):
    setuparr = setupstring.split(",")
    importsarr = []
    for i in setuparr:
        if optionsmapping.get(i) is not None:
            importsarr.append(optionsmapping.get(i))
        else:
            print("Could not determin mapping of import parameter")