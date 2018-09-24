#!/bin/sh

set -e

if [ -n "$KUBERNETES_PORT" ] && [ -n "$DNS_IP" ]
then
    sed -i "1 i\nameserver $DNS_IP" /etc/resolv.conf
fi

astaire $@
