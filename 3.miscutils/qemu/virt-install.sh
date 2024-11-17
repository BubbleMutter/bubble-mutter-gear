#!/bin/bash
if [ -z "${NET_BRIDGE}" ]; then
	NET_BRIDGE=virbr0
fi

if [ -z "${IMAGE_NAME}" ]; then
	echo env:IMAGE_NAME is missing.
fi

if [ -z "${IMAGE_FILE}" ]; then
	echo env:IMAGE_FILE is missing.
fi

if [ ! -f "${IMAGE_FILE}" ]; then
	echo ${IMAGE_FILE} file is not found.
fi

echo env:NET_BRIDGE=[${NET_BRIDGE}]
echo env:IMAGE_FILE=[${IMAGE_FILE}]
echo env:IMAGE_NAME=[${IMAGE_NAME}]

virt-install --name "${IMAGE_NAME}" \
	--connect qemu:///system \
	--virt-type kvm\
	--memory 4096\
	--vcpus 4 \
	--disk=${IMAGE_FILE},format=qcow2,bus=virtio \
	--boot=hd \
	--network bridge=${NET_BRIDGE},model=virtio \
	--boot loader=/usr/share/OVMF/OVMF_CODE.fd \
	--graphics none \
	--noautoconsole
