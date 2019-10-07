#!/bin/bash

set -e

if [[ ! -d build ]]; then
    mkdir build
fi

cd build

mockPath="../Mocks/";
options="-c -g -std=gnu++14"
srcPath="../../src"
libPath="../../libs"
testPath="../"

mocksOptions="-I $mockPath"
srcOptions="-I $srcPath"
libOptions="-I $libPath/arduinolibs/libraries/Crypto -I $libPath/arduinolibs/libraries/CryptoLW/src -I $libPath/CRC32/src"
testOptions="-I $libPath/Catch2/single_include/catch2 -I $libPath/FakeIt/include -I $libPath/FakeIt/config/catch"
fullOptions="$mocksOptions $srcOptions $testOptions $libOptions"

g++ $mocksOptions $options $mockPath/Arduino.cpp
g++ $mocksOptions $options $mockPath/fake_serial.cpp

if [[ ! -f main.o ]]; then
    g++ $testOptions $options $testPath/main.cpp
fi

g++ $mocksOptions $libOptions $srcOptions $options $srcPath/MqttModule/SubscriberList.cpp
g++ $mocksOptions $srcOptions $libOptions $options $srcPath/RadioEncrypted/Encryption.cpp
g++ $mocksOptions $srcOptions $libOptions $options $srcPath/MqttModule/MeshMqttClient.cpp

g++ $mocksOptions $libOptions $options $libPath/arduinolibs/libraries/Crypto/AuthenticatedCipher.cpp
g++ $mocksOptions $libOptions $options $libPath/arduinolibs/libraries/Crypto/Cipher.cpp
g++ $mocksOptions $libOptions $options $libPath/arduinolibs/libraries/CryptoLW/src/Acorn128.cpp
g++ $mocksOptions $libOptions $options $libPath/arduinolibs/libraries/Crypto/Crypto.cpp
g++ $mocksOptions $libOptions $options $libPath/CRC32/src/CRC32.cpp


# compile test
g++ $fullOptions $options $testPath/MqttModule/SubscriberListTest.cpp
g++ $fullOptions $options $testPath/RadioEncrypted/EncryptionTest.cpp
g++ $fullOptions $options $testPath/MqttModule/MeshMqttClientTest.cpp

if [[ -f test ]]; then
    rm test
fi

g++ $(ls | tr '\n' ' ') -o test

./test $1
