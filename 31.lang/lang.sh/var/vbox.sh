#!/bin/bash
vboxModule=(vboxnetflt vboxnetadp vboxpci vboxdrv)
/sbin/modprobe ${vboxModule[@]}    # load
/sbin/modprobe -r ${vboxModule[@]} # unload