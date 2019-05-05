

#include <cstdio>
#include <MQTTClient.h>
#include <iostream>
#include "UserMQTTClient.h"
#include "jsoncpp/json/json.h"
#include "wiringPi.h"
#include "pthread.h"

void loop(uint16_t cnt, int ctlPin, int sigPin)
{
    pinMode(ctlPin, OUTPUT);
    pullUpDnControl(ctlPin, PUD_DOWN);
    pinMode(sigPin, INPUT);

    while (cnt --)
    {
        printf("count = %d\n",cnt);
        digitalWrite(ctlPin, 1);
        uint8_t sig = digitalRead(sigPin);
        while(sig)
        {
          //  printf("sig state: %d\n",sig);
            delay(10);
            sig = digitalRead(sigPin);
        }
       // printf("___sig state: %d\n",sig);
        delay(10);
        while(!sig)
        {
         //   printf("sig state: %d\n",sig);
            delay(10);
            sig = digitalRead(sigPin);
        }
        //printf("###sig state: %d\n",sig);
        digitalWrite(ctlPin, 0);
    }
}

volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

    payloadptr = (char *)message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');

    Json::Reader reader;
    Json::Value root;
    int id = 0;
    int count = 0;
    if (reader.parse((char *)message->payload, (char *)message->payload + message->payloadlen, root))
    {
        id = root["id"].asInt();
        count = root["count"].asInt();
    }
    printf("id = %d, count = %d\n", id, count);


    if(id == 0x0001)
    {
        loop(count, 24, 25);

    }
    else if(id == 0x0002)
    {
        loop(count, 24, 25);
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
int main()
{
//    std::string json = "{\"id\" : 123, \"name\" : \"wu\"}";
//
//    Json::Reader reader;
//    Json::Value root;
//    std::string name;
//    int id = 0;
//    if (reader.parse(json, root))
//    {
//        name = root["name"].asString();
//        id = root["id"].asInt();
//    }
//    std::cout << "root: " << id << std::endl;
//    std::cout << "name: " << name << std::endl;
//    Json::Value msg;
//    msg["id"] = "456";
//    msg["count"] = "99";
//    Json::FastWriter writer;
//    std::string jsonstr =  writer.write(msg);
//    std::cout << jsonstr << std::endl;
//
//    return  0;

    if (wiringPiSetup() < 0) return  1;
    UserMQTTClient client;
    char ch;
    client.initialize("kaylor","kaylor","mqtt.kaylordut.com","11111", "123456789", connlost, msgarrvd, delivered);
    client.subscribe("Test", 2);
    printf("Press Q<Enter> to quit\n");
    do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    client.unsubscribe("Test");
    return 0;
}