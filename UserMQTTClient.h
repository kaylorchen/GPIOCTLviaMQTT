//
// Created by ubuntu on 19-5-5.
//

#ifndef MQTT_USERMQTTCLIENT_H
#define MQTT_USERMQTTCLIENT_H

#include "MQTTClient.h"

class UserMQTTClient {
private:
    MQTTClient mClient;
public:
    ~UserMQTTClient();

    void initialize(const char *username,
                    const char *password,
                    const char *serverIP,
                    const char *port,
                    const char *deviceID,
                    void (*connlost)(void *, char *),
                    int (*msgarrvd)(void *, char *, int, MQTTClient_message *),
                    void (*delivered)(void *, MQTTClient_deliveryToken)
    );
    void publish(const char *topic, const char *payload, int QOS);
    void subscribe(const char *topic, int QOS);
    void unsubscribe(const char *topic);
};


#endif //MQTT_USERMQTTCLIENT_H
