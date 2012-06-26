#!/usr/bin/env python

import ipcalc
import sys

from arprequest import ArpRequest

n = sys.argv[1]
lfile = sys.argv[2]

try:
    iface = sys.argv[3]
except:
    iface = 'eth0'

net = ipcalc.Network(n)

with open(lfile, 'w') as f:
    for ip in net:
        f.write('{} '.format(ip))
        sys.stdout.write('{} '.format(ip))

        req = ArpRequest('{}'.format(ip), iface)
        if not req.request():
            f.write('Not ')
            sys.stdout.write('Not ')
        f.write('Used\n')
        sys.stdout.write('Used\n')
