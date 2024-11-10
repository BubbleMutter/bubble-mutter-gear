#!/bin/bash
xrandr --setprovideroutputsource modesetting NVIDIA-0
xrandr --auto
synclient TapButton1=1
synclient TapButton2=3
