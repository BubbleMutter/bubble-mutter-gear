#!/bin/bash
#
# installation
apt install tcptraceroute
wget http://www.vdberg.org/~richard/tcpping
chmod a+x tcpping
tcpping 127.0.0.1 22