#!/usr/bin/env python3

import sys;
import os;
import subprocess;

originalPicture=sys.argv[1]
iconsetDir=sys.argv[2]

if not os.path.exists(iconsetDir):
	os.mkdir(iconsetDir)


# a class to store icon parameters: its size and scale factor
class IconParameters():
    width = 0
    scale = 1
    def __init__(self,width,scale):
        self.width = width
        self.scale = scale
    def getIconName(self):
        if self.scale != 1:
            return f"icon_{self.width}x{self.width}.png"
        else:        
            return f"icon_{self.width//2}x{self.width//2}@2x.png"

# create a list of all the sizes (5 actual sizes, but 10 files)
ListOfIconParameters = [
    IconParameters(32, 1),
    IconParameters(32, 2),
    IconParameters(64, 1),
    IconParameters(64, 2),
    IconParameters(256, 1),
    IconParameters(256, 2),
    IconParameters(512, 1),
    IconParameters(512, 2),
    IconParameters(1024, 1),
    IconParameters(1024, 2)
]

# generate iconset folder by calling sips utility for each item in the list (so it's 10 times)
for ip in ListOfIconParameters:
    subprocess.call(["sips", "-z", str(ip.width), str(ip.width), originalPicture, "--out", os.path.join(iconsetDir, ip.getIconName())])
