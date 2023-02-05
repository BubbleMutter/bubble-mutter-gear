#!/bin/bash
xrandr --setprovideroutputsource modesetting NVIDIA-0
xrandr --auto
(
    # This delay is required. Might be because the X server isn't
    # started yet.
    sleep 10

    # Set up a 5 minute timeout before powering off the display.
    xset dpms 0 0 60
)&
synclient TapButton1=1
synclient TapButton2=3
