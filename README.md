# mosquitto_client_with_aws_iot

### Before starting this tutorial, you will need:

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

### Finally!
1. You modify the root ca file path, certification file path, private key file from mqttwrapper.cpp.
2. You modify the hostname on HTTPS Endpoint Page for Things from AWS IoT Console.
3. You modify the iot topic on Interact Page for Things from AWS IoT Console.

Good Lock to you!