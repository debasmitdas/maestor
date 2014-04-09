#!/bin/bash
if [ $(pgrep hubo-daemon) ]; then
    echo "Hubo-Ach daemon is already running. Terminating..."
    hubo-ach killall &> /dev/null
    echo "Hubo-Ach daemon terminated."
fi

xterm -e "roscore" &

export PYTHONPATH="$PYTHONPATH:/usr/lib/python2.7/dist-packages" 
xterm -e "hubo-ach sim openhubo nophysics simtime" &

while [ ! $(pgrep hubo-daemon) ]; do
    sleep 1
done

sudo -E ~/git/git/maestor/bin/maestor sim 

while true; do
    sleep 3
done
