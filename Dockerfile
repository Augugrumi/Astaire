FROM augugrumi/astaire-dockerbuilds:alpine as builder
WORKDIR /tmpbuilding/
COPY . .
RUN builder -DCMAKE_BUILD_TYPE=Release -DJNI=True -DTCP=False -DUDP=True

FROM alpine:3.8
LABEL maintainer="poloniodavide@gmail.com"

RUN apk --update --no-cache add \
    openjdk8 \
    jsoncpp \
    boost-system \
    boost-thread && \
    mkdir -p /data/

VOLUME /data/

EXPOSE 8767
EXPOSE 8767/udp

# FIXME this will broke one day
COPY --from=builder /tmpbuilding/build/src/astaire /usr/bin/astaire

ENTRYPOINT ["astaire"]