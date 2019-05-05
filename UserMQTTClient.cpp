//
// Created by ubuntu on 19-5-5.
//

#include <string>
#include <iostream>
#include <cstring>
#include "UserMQTTClient.h"

void UserMQTTClient::initialize(const char *username, const char *password, const char *serverIP, const char *port,
                                const char *deviceID,
                                void (*connlost)(void *, char *),
                                int (*msgarrvd)(void *, char *, int, MQTTClient_message *),
                                void (*delivered)(void *, MQTTClient_deliveryToken)) {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    std::string link = "tcp://" + std::string(serverIP) + ":" + std::string(port);
    std::cout << "Server's address: " << link << std::endl;
    MQTTClient_create(&mClient, link.c_str(), deviceID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = username;
    conn_opts.password = password;
    MQTTClient_setCallbacks(mClient, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(mClient, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

void UserMQTTClient::publish(const char *topic, const char *payload, int QOS) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    pubmsg.payload = (void *)payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(mClient, topic, &pubmsg, &token);
}

void UserMQTTClient::subscribe(const char *topic, int QOS) {
    MQTTClient_subscribe(mClient, topic, QOS);
}

void UserMQTTClient::unsubscribe(const char *topic) {
    MQTTClient_unsubscribe(mClient, topic);
}

UserMQTTClient::~UserMQTTClient() {
    MQTTClient_disconnect(mClient, 10000);
    MQTTClient_destroy(&mClient);
    std::cout<<"exit MQTT service" << std::endl;
}