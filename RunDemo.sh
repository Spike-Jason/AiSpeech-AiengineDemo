#!/bin/bash

##Current directory
DIR_FOLDER=$(cd "$(dirname "$0")";pwd)

#Complie
make all
#Run Auth
cd $DIR_FOLDER/demo/auth/
./auth cfg_auth.json
cd $DIR_FOLDER

#Run ASR
cd $DIR_FOLDER/demo/asr/
./asr cfg_cloud_asr.json param_cloud_asr.json ../../assets/0.wav 44
cd $DIR_FOLDER

#Run Cntts
cd $DIR_FOLDER/demo/cntts/
./cntts cfg_cloud_cntts.json param_cloud_cntts.json
cd $DIR_FOLDER

#Run Wakeup
cd $DIR_FOLDER/demo/wakeup/
./wakeup cfg_native_wakeup.json param_native_wakeup.json ../../assets/0.wav 44
cd $DIR_FOLDER

#Run Gram
cd $DIR_FOLDER/demo/gram/
./gram cfg_native_gram.json param_native_gram.json ../../assets/0.wav 44
cd $DIR_FOLDER