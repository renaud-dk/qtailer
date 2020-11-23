#!/bin/bash

COUNTER=0
    while [  $COUNTER -lt 500000 ]; do
      echo "The counter is $COUNTER or this is the message $COUNTER" >> log.txt
      let COUNTER=COUNTER+1 
    done
