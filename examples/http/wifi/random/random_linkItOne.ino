#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LWiFiServer.h>
#include <LWiFiUdp.h>
#include <SPI.h>
#include <thethingsiOlinkItOne.h> 

#define WIFI_AP "YOUR ESSID"
#define WIFI_PWD "YOUR PASSWORD"

int status = -1;
int r;

#define TOKEN "YOUR TOKEN HERE"

thethingsiOWiFiLinkItOne thing(TOKEN);

void setup() {
  Serial.begin(9600);
  startWifi();
}

void loop() {
    r = random(101);
    Serial.println(r);
    thing.addValue("number",r);
  thing.send();
}

void startWifi(){
  Serial.println("Connecting LinkIt One to network...");
  LWiFi.begin();
   // attempt to connect to Wifi network:
  while ( status != LWIFI_STATUS_CONNECTED ) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    LWiFi.connectWPA(WIFI_AP, WIFI_PWD);
    // wait 10 seconds for connection:
    delay(10000);
    status = LWiFi.status();
  }
   
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = LWiFi.localIP();
    Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
 
  // print your MAC address:
  byte mac[6];  
  LWiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
 
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(LWiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  LWiFi.BSSID(bssid);    
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = LWiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}
