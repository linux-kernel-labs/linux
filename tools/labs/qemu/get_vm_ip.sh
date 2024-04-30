#!/bin/sh

GUEST_IP=$(arp -n -i tap0 | tail -1 | cut -d ' ' -f 1)
while [ "$GUEST_IP" = "arp:" ]; do
    sleep 1
    GUEST_IP=$(arp -e -n -i tap0 | tail -1 | cut -d ' ' -f 1)
done

echo $GUEST_IP
