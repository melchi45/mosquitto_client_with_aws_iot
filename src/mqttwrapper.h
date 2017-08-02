#pragma once

#include <iostream>        /* for string */
#include <mosquittopp.h>

class MQTTWrapper : public mosqpp::mosquittopp
{
public:
	MQTTWrapper(const char *_id);
	MQTTWrapper(const char *_id, const char* host, int port);
	MQTTWrapper(const char *_id, const char* host, int port, const char* capath, const char *rootca_path, const char *cert_path, const char *privatekey_path);
	~MQTTWrapper();
	
	void pub(std::string topic, std::string value);
	
protected:
	void on_connect(int rc);
	void on_disconnect(int rc);
	void on_message(const struct mosquitto_message *message);
	void on_publish(int mid);
	void on_subcribe(int mid, int qos_count, const int *granted_qos);
	int do_check(const char *sub, const char *topic, bool bad_res);
	
private:
	std::string	host;
	const char	*id;
	std::string	topic;
	int			port;
	int			keepalive;
};