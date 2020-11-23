#!/bin/bash
PWD=`pwd`

if [ -d /home/renaud/Documents/Developpement/software/Qt/Projects/Builds/Images ]
then
  rm -rf /home/renaud/Documents/Developpement/software/Qt/Projects/Builds/Images
fi

cp -r /home/renaud/Documents/Developpement/software/Qt/Projects/QTailer/Images /home/renaud/Documents/Developpement/software/Qt/Projects/Builds

#if [ -d /home/renaud/Documents/Developpement/software/Qt/Projects/Builds/Config ]
#then
#  rm -rf /home/renaud/Documents/Developpement/software/Qt/Projects/Builds/Config
#fi

#cp -r /home/renaud/Documents/Developpement/software/Qt/Projects/QTailer/Config /home/renaud/Documents/Developpement/software/Qt/Projects/Builds

echo "Done Images and Config forlder have been copied."
