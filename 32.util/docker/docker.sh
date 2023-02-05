# install on debian ==========================================================
apt-get remove docker docker-engine docker.io containerd runc
apt-get update
apt-get install  apt-transport-https ca-certificates curl gnupg lsb-release
# Add Docker’s official GPG key:
curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

docker_source="deb"
docker_source=${docker_source}" [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg]"
docker_source=${docker_source}" https://download.docker.com/linux/debian"
docker_source=${docker_source}" $(lsb_release -cs) stable"
echo ${docker_source} | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

apt-get update
apt-get install docker-ce docker-ce-cli containerd.io
# ===========================================================================

docker run hello-world

docker ps

# download images debian (https://hub.docker.com/_/debian)
docker pull debian:stretch
docker pull debian:buster
# stop
docker stop debian9
# start
docker start debian9
# attach container
docker attach debian9
# create container
docker run -idt --net host --privileged --name debian9 -v /root:/root debian:stretch /bin/bash
# create custom network
docker network create --subnet=172.18.0.0/16 docker1
# create container with fixed port and ip
docker run -itd --name container201 --net docker1 --ip 172.18.0.201 -p 17781:80 --security-opt seccomp:unconfined debian:stretch /bin/bash
