# mosquitto_client_with_aws_iot

## Before starting this tutorial, you will need:

 *1. Ubuntu Linux
 *2. Mosquitto client and dev package on Unbuntu
 *3. AWS IoT account and registry Things and certification files(root_ca.pem, certificate.crt.pem, privatekey.pem)

Ubuntu 16.04 has a fairly recent version of Mosquitto in its default software repository. Log in with your non-root user and install Mosquitto with apt-get.

```bash
$ sudo apt-get install mosquitto mosquitto-clients
$ sudo apt-get install libmosquitto-dev libmosquittopp-dev
```

Log in to 'test.mosquitto.org' server a second time, so you have two terminals side-by-side. In the new terminal, use mosquitto_sub to subscribe to the test topic:

```bash
$ mosquitto_sub -h test.mosquitto.org -t iot/hello/test
```

-h is used to specify the hostname of the MQTT server, and -t is the topic name. You'll see no output after hitting ENTER because mosquitto_sub is waiting for messages to arrive. Switch back to your other terminal and publish a message:

```bash
$ mosquitto_pub -h test.mosquitto.org -t iot/hello/test -m "hello world"
```

The options for mosquitto_pub are the same as mosquitto_sub, though this time we use the additional -m option to specify our message. Hit ENTER, and you should see hello world pop up in the other terminal. You've sent your first MQTT message!

and you have to find the mosquitto library file from your development linux

```bash
$ find /usr/lib -name libmosquitto*
/usr/lib/x86_64-linux-gnu/libmosquittopp.so.1
/usr/lib/x86_64-linux-gnu/libmosquittopp.so
/usr/lib/x86_64-linux-gnu/libmosquitto.so
/usr/lib/x86_64-linux-gnu/libmosquitto.so.1
```

You have to change the include and lib path on debug.mak and release.mak

## cross compile for Windows environment
You have to prepare windows cmake command, and you have to prepare toolchain using vcpkg on windows.
You can reference from [Microsoft Vcpkg](https://docs.microsoft.com/en-us/cpp/vcpkg) and, you can download
from [vcpkg github](https://github.com/Microsoft/vcpkg)

First, you have to build mosquitto library for this project on vcpkg.
```
.\vcpkg install mosquitto
```

And, if you vcpkg path is D:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake, you can build this command on this project directory
```
cmake . -Bstatic -G "Visual Studio 15 2017" -DVCPKG_TARGET_TRIPLET=x86-windows -DCMAKE_TOOLCHAIN_FILE="D:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build static --config Release
```

## remote build environment for embedded linux on Windows using Visual Studio
If you install the [VisualGDB](https://visualgdb.com/) cross compile environment for Visual Studio, you can compile on windows for remote embedded environment using ssh.
You can download VisualGDB for Visual Studio. and set the target device environment for your embedded device.

If you are ready for remote environment, open it for visual studio solution file.

## Source Code
First, Mosquitto test server
If you want to test this application with test.mosquitto.org, you should open this line on this source code.
```
/* if you want to use test mqtt server 
 * mqttHdl = new MQTTWrapper("connection-test", target_host, target_port); 
 */
``` 
you can test it with mosquitto test server.

Second, AWS IoT service platform
If you want to test with AWS IoT platform, you have to create aws iot certification files on AWS IoT console. [ref. 1](https://docs.aws.amazon.com/iot/latest/developerguide/create-device-certificate.html)
and, you have to download crt and key files from AWS IoT console. otherwise, you can possible to generate certification file with openssl. [ref. 2](https://docs.aws.amazon.com/iot/latest/developerguide/device-certs-your-own.html)

If you are complete certification download from AWS IoT, you can modify certification path from this line on this source code.
```
std::string capath("/home/ubuntu/Dev/wr4.0/cert");
std::string cafile("/home/ubuntu/Dev/wr4.0/cert/VeriSign-Class 3-Public-Primary-Certification-Authority-G5.pem");
std::string cert("/home/ubuntu/Dev/wr4.0/cert/certificate.pem.crt");
std::string key("/home/ubuntu/Dev/wr4.0/cert/private.pem.key");
```


## Finally!
1. You modify the root ca file path, certification file path, private key file from mqttwrapper.cpp.
2. You modify the hostname on HTTPS Endpoint Page for Things from AWS IoT Console.
3. You modify the iot topic on Interact Page for Things from AWS IoT Console.

Good Lock to you!