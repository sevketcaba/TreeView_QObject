#!/bin/bash
set -e
build=0
while getopts "b" flag
do
case "${flag}" in
    b) build=1;;
    esac
done

if [ ${build} -eq 1 ]
then
source build-docker.sh
fi

xhost local:docker
docker run -it \
-e DISPLAY=${DISPLAY} \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    --network host \
    treeviewqobject:latest
