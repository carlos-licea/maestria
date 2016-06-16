#!/usr/local/bin/python

import os
import shutil
import subprocess
from decimal import *

cost = 5.0
type = "vpg"

for i in ['0.1', '0.2', '0.3', '0.4', '0.5', '0.6', '0.7', '0.8', '0.9', '1.0']:
    os.mkdir(i)
    shutil.copy("lattice", i)
    os.chdir(i)
    subprocess.run(['../../generate_realization', i, str(cost), type])
    os.chdir("..")