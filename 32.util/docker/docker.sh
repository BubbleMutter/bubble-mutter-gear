installation_debian() {
    # install on debian ==========================================================
    apt-get remove docker docker-engine docker.io containerd runc
    apt-get update
    apt-get install  apt-transport-https ca-certificates curl gnupg lsb-release
    curl -fsSL http://mirrors.ustc.edu.cn/docker-ce/linux/debian/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
    sudo echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] http://mirrors.ustc.edu.cn/docker-ce/linux/debian buster stable"  > /etc/apt/sources.list.d/docker.list
    apt-get update
    apt-get install docker-ce docker-ce-cli containerd.io
    docker run hello-world
}

mamagement() {
    # download images debian (https://hub.docker.com/_/debian)
    docker pull debian:stretch
    docker pull debian:buster
    docker ps -a          # dump
    docker stop debian9   # stop
    docker start debian9  # start
    docker attach debian9 # attach container
    # create container
    docker run -idt --net host --privileged --name debian9 -v /root:/root debian:stretch /bin/bash
    # create custom network
    docker network create --subnet=172.18.0.0/16 docker1
    # create container with fixed port and ip
    docker run -itd --name container201 --net docker1 --ip 172.18.0.201 -p 17781:80 --security-opt seccomp:unconfined debian:stretch /bin/bash
    # spawn a new process
    docker exec -it debian9 /bin/bash
}

# inspect container's volumes
docker inspect dpu | jq '.[].Mounts[] | .Source, .Destination' | sed 'N; s|\n| |'

# edit config must wait docker is stopped, or the file will be restored
systemctl stop docker.service
vim /var/lib/docker/containers/$container_id/hostconfig.json
systemctl start docker.service