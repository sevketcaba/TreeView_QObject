FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections && \
apt-get update && apt-get -y -q install \
    apt-utils \
    build-essential \
    cmake \
    qt5-default \
    cppcheck

WORKDIR /project
COPY ./CMakeLists.txt ./CMakeLists.txt
COPY ./common ./common
COPY ./resources ./resources
COPY ./components ./components
COPY ./src ./src
RUN mkdir build

WORKDIR /project/build

RUN cmake ..
RUN make

CMD ./bin/TreeView_QObject
