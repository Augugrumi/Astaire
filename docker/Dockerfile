FROM alpine:latest

RUN apk update && \
    apk upgrade && \
    apk add cmake \
    jsoncpp-dev \
    openjdk8 \
    build-base \
    wget \
    git \
    boost-dev && \
    rm -rf /var/cache/apk/*

ENV JAVA_HOME /usr/lib/jvm/default-jvm

RUN git clone https://github.com/oktal/pistache.git --recursive && \
    cd pistache && \
    mkdir build && \
    cd build && \
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && \
    make && \
    make install

RUN git clone https://github.com/Augugrumi/astaire.git && \
    cd astaire && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Debug -DJNI=False -DTCP=True -DUDP=True .. && \
    make
