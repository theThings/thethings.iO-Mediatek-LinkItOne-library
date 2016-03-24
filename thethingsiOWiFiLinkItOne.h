#ifndef THETHINGSWIFILINKITONE_H
#define THETHINGSWIFILINKITONE_H

#include "thethingsiOClient.h"

class thethingsiOWiFiLinkItOne : public thethingsiOClient {
public:
    thethingsiOWiFiLinkItOne() : thethingsiOClient(&regular_client, &subscription_client) { }
    thethingsiOWiFiLinkItOne(String token) : thethingsiOClient(&regular_client, &subscription_client, token) { }
private:
    LWiFiClient regular_client;
    LWiFiClient subscription_client;
};

#endif // THETHINGSWIFILINKITONE_H
