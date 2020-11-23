#!/bin/bash

LOOP=0

while [ $LOOP -lt 10 ]; do
  let LOOP=LOOP+1
COUNTER=0
    while [  $COUNTER -lt 202 ]; do
      echo "$COUNTER The counter is $COUNTER or this is the message $COUNTER" >> log-sleep.txt
      let COUNTER=COUNTER+1 
      ./msleep 50
    done
done
