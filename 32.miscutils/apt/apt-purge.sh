# apt purge
for d in $(dpkg -l | awk '/^rc/{print $2}'); do sudo apt remove --purge -y $d; done

# apt trace all out-of-repo package
for d in $(dpkg -l | awk '/^ii/{print $2}'); do [ 0 -ne $(apt policy $d 2>/dev/null| grep -w '\*\*\*' -A1 | grep -cw /var/lib/dpkg/status) ] && echo $d ; done
