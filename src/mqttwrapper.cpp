#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#if ( !defined(_MSC_VER) && (!(defined(_WIN32) || !defined(_WIN64))))
#include <getopt.h>    /* for getopt */
#include <unistd.h>    /* for sleep */
#else
#include "getopt.h"    /* for getopt */
#endif
#include <cstring>     /* for strncpy, strlen */
#include <signal.h>    /* for signal */
#include "mqttwrapper.h"

using namespace std;

static int run = 1;

// Server connection parameters
#define MQTT_HOSTNAME "test.mosquitto.org"	/* mqtt server url for test */
#define MQTT_PORT 1883						/* mqtt server port */
#define MQTT_USERNAME "admin"				/* mqtt username */
#define MQTT_PASSWORD "admin"				/* mqtt password */
#define MQTT_TOPIC "test"					/* mqtt topic */

void handle_signal(int s)
{
	run = 0;
}

static int password_callback(char* buf, int size, int rwflag, void* userdata)
{
	strncpy(buf, "password", size);
	buf[size - 1] = '\0';

	return strlen(buf);
}

MQTTWrapper::MQTTWrapper(const char *_id)
	: mosqpp::mosquittopp(_id)
	, keepalive(120)
{
	mosqpp::lib_init();			// Initialize libmosquitto
}

MQTTWrapper::MQTTWrapper(const char *_id, const char* host, int port)
	: mosqpp::mosquittopp(_id)
	, keepalive(120)
{	
	mosqpp::lib_init();
	this->id = _id;
	this->port = port;
	this->host = host;
	
//	int keepalive = 10;
//	if (username_pw_set("sampleuser", "samplepass") != MOSQ_ERR_SUCCESS) {
//		std::cout << "setting passwd failed" << std::endl;
//	}
	connect_async(this->host.c_str(), this->port, this->keepalive);	// non blocking connection to broker request
	//connect(this->host, port, keepalive);	// non blocking connection to broker request
	
	// Start thread managing connection / publish / subscribe
	if (loop_start() != MOSQ_ERR_SUCCESS) {
		std::cout << "loop_start failed" << std::endl;
	}
}

MQTTWrapper::MQTTWrapper(const char *_id, const char* host, int port, const char* capath, const char *rootca_path, const char *cert_path, const char *privatekey_path)
	: mosqpp::mosquittopp(_id)	
	, keepalive(120)
{
	mosqpp::lib_init();			// Initialize libmosquitto
	this->id = _id;
	this->port = port;
	this->host = host;
	
	tls_opts_set(1, "tlsv1.2", NULL);
	tls_set(rootca_path, capath, cert_path, privatekey_path, NULL);
	connect_async(this->host.c_str(), this->port, this->keepalive);	// non blocking connection to broker request
	//connect(this->host.c_str(), this->port, this->keepalive);	// non blocking connection to broker request
	
	// Start thread managing connection / publish / subscribe
	if (loop_start() != MOSQ_ERR_SUCCESS) {
		std::cout << "loop_start failed" << std::endl;
	}
}

MQTTWrapper::~MQTTWrapper()
{
	std::cout << "1" << std::endl;
	if (loop_stop() != MOSQ_ERR_SUCCESS) {
		std::cout << "loop_stop failed" << std::endl;
	}
	std::cout << "2" << std::endl;
	mosqpp::lib_cleanup();
	std::cout << "3" << std::endl;
}

void MQTTWrapper::on_connect(int rc)
{
	if (rc == 0) {
		std::cout << ">> MQTTWrapper - connected with server" << std::endl;
	}
	else {
		std::cout << ">> MQTTWrapper - Impossible to connect with server(" << rc << ")" << std::endl;
	}

	if (rc) {
		disconnect();
	} else {
//		publish(NULL, "pub/qos2/test", strlen("message"), "message", 2, false);
		subscribe(NULL, "$aws/things/MyCamera/shadow/update", 0);
		printf("subscribe topic: $aws/things/MyCamera/shadow/update\n");
		//subscribe(NULL, "#", 0);
		//printf("subscribe topic: qos0/test\n");
		
//		if (do_check("pub/qos2/test", "pub/qos2/test", false) < 0)
//		{
//			printf("fail to subscribe topic: pub/qos2/test\n");
//		}
//		else
//		{
//			printf("success to subscribe topic: pub/qos2/test\n");
//		}
	}
}

void MQTTWrapper::on_disconnect(int rc)
{
	std::cout << ">> MQTTWrapper - disconnection(" << rc << ")" << std::endl;
	run = rc;
}

void MQTTWrapper::on_publish(int mid)
{
	//disconnect();
	std::cout << "MQTTWrapper - succeed to be published (" << mid << ")" << std::endl;
}

void MQTTWrapper::on_subcribe(int mid, int qos_count, const int *granted_qos)
{
	std::cout << "MQTTWrapper - succeeded to be subscribed (" << mid << ")" << std::endl;
}

int MQTTWrapper::do_check(const char *sub, const char *topic, bool bad_res)
{
	bool match;

	mosqpp::topic_matches_sub(sub, topic, &match);
	
	if (match == bad_res) {
		printf("s: %s t: %s\n", sub, topic);
		return -1;
	}
	
	return 0;
}

void MQTTWrapper::pub(std::string topic, std::string value) {
	int ret = publish(NULL, topic.c_str(), value.size(), value.c_str(), 1, false);
	if (ret != MOSQ_ERR_SUCCESS) {
		std::cout << "Sending failed." << std::endl;
	}
}

void MQTTWrapper::on_message(const struct mosquitto_message *message)
{
	char* pchar = (char*)(message->payload);
	std::string str(pchar);
	
	std::cout << "mid: " << message->mid << std::endl;
	std::cout << "topic: " << message->topic << std::endl;
	std::cout << "qos: " << message->qos << std::endl;
	std::cout << "payload length: " << message->payloadlen << std::endl;
	std::cout << "payload: " << message->payload << std::endl;
	std::cout << "message: " << str.c_str() << std::endl;
	std::cout << "retain: " << message->retain << std::endl;
}

int main(int argc, char *argv[])
{
	int opt, rc = 0;
	/* you have to modify iot url and port number from aws iot console for your aws iot */
	const char* target_host = "a31dsdqskm4t0i.iot.us-east-1.amazonaws.com";
	int target_port = 8883;
	
	/* you have to modify certifiacate path for your aws iot */
	std::string capath("/home/ubuntu/Dev/wr4.0/cert");
	std::string cafile("/home/ubuntu/Dev/wr4.0/cert/VeriSign-Class 3-Public-Primary-Certification-Authority-G5.pem");
	std::string cert("/home/ubuntu/Dev/wr4.0/cert/certificate.pem.crt");
	std::string key("/home/ubuntu/Dev/wr4.0/cert/private.pem.key");
	
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	
	while ((opt = getopt(argc, argv, "h:p:r:c:k:")) != -1) {
		switch (opt) {
		case 'h':
			target_host = optarg;
			break;
		case 'p':
			target_port = atoi(optarg);
			break;
		case 'r':
			cafile = std::string(optarg);
			break;
		case 'c':
			cert = std::string(optarg);
			break;
		case 'k':
			key = std::string(optarg);
			break;
		default:
			//ShowUsage(argv[0]);
			break;
		}
	}
	
	MQTTWrapper * mqttHdl;
	/* if you want to use test mqtt server 
	 * mqttHdl = new MQTTWrapper("connection-test", target_host, target_port); 
	 */
	/* if you want to use aws iot server */
	mqttHdl = new MQTTWrapper("connection-test", target_host, target_port, capath.c_str(), cafile.c_str(), cert.c_str(), key.c_str());
	//mqttHdl = new MQTTWrapper("subscribe-qos0-test");
	//mqttHdl->connect(target_host, target_port, 60);
	
	//mqttHdl->pub("topic/test", "message");
	
/*	if (mqttHdl != NULL) {
		while (run == 1) {
			rc = mqttHdl->loop();						// Keep MQTT connection		
			if (run && rc) {
				printf("connection error!\n");
				sleep(10);
				mqttHdl->reconnect();
			}
			usleep(100);
		}
	}
*/
	delete mqttHdl;

	return run;
	
//	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
//	cout << sz << endl;	//<================= Put a breakpoint here
//	return 0;
}