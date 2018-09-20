FROM augugrumi/astaire-dockerbuilds:alpine as builder

COPY . /build/

RUN builder -DCMAKE_BUILD_TYPE=Release -DJNI=True -DTCP=True -DUDP=True

FROM alpine:3.8

LABEL maintainer="poloniodavide@gmail.com"

COPY --from=builder /build/build/ /tmpbuild/

RUN apk --update --no-cache add \
    openjdk-8-jdk \
    libjsoncpp \
    boost-system \
    boost-thread && \
    apk --update --no-cache --virtual add \
    make && \
    cd /tmpbuild/ && \
    make install/strip && \
    cd - && \
    rm -Rf /tmpbuild/

RUN mkdir -p /data/
VOLUME /data/

ENTRYPOINT ["astaire"]