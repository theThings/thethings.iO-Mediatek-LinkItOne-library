#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LWiFiServer.h>
#include <LWiFiUdp.h>
#include <thethingsiOlinkItOne.h> 
#include <LGPS.h>
#include <SPI.h>

#define WIFI_AP "YOUR ESSID"
#define WIFI_PWD "YOUR ESSID"

int millis_gps;
gpsSentenceInfoStruct gpsData;
double latitude;
double longitude;
int hour, minute, second, num;
char buff[256];
int status = -1;

#define TOKEN "YOUR TOKEN HERE"

thethingsiOWiFiLinkItOne thing(TOKEN);

void setup() {
  Serial.begin(9600);
  startWifi();
  millis_gps = millis();
  LGPS.powerOn(GPS_GLONASS);
  send_gps();
}

void loop() {
  if (millis() >= millis_gps + 60000){
    send_gps();
  }
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

void convertPositionToDecimalForm()
{
  double latitudeDecimalPart;
  double longitudeDecimalPart;
  latitude = latitude / 100.0;
  longitude = longitude / 100.0;
  latitudeDecimalPart = modf(latitude, &latitude);
  longitudeDecimalPart = modf(longitude, &longitude);
  latitude = latitude + (latitudeDecimalPart * 100.0 / 60.0);
  longitude = longitude + (longitudeDecimalPart * 100.0 / 60.0);
}

// Helper functions from the GPS.ino example by MediaTek

// Helper function for extracting GPS data from a NMEA string.
// Returns the offset of the character after the n:th comma.
// Returns 0 if the n:th comma is not found.
static unsigned char getComma(unsigned char num, const char *str)
{
  unsigned char i, j = 0;
  int len = strlen(str);
  for (i = 0; i < len; i ++)
  {
    if (str[i] == ',')
      j++;
    if (j == num)
      return i + 1;
  }
  return 0;
}

// Helper function for extracting GPS data from a NMEA string.
// Returns the number that follows the first comma in s.
static double getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;

  i = getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev = atof(buf);
  return rev;
}

// Helper function for extracting GPS data from NMEA string.
// Returns the number that follows the first comma in s.
static double getIntNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;

  i = getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev = atoi(buf);
  return rev;
}

// Helper function for extracting GPS data from NMEA string.
void parseGPGGA(const char* GPGGAstr)
{
  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
   * Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   * Where:
   *  GGA          Global Positioning System Fix Data
   *  123519       Fix taken at 12:35:19 UTC
   *  4807.038,N   Latitude 48 deg 07.038' N
   *  01131.000,E  Longitude 11 deg 31.000' E
   *  1            Fix quality: 0 = invalid
   *                            1 = GPS fix (SPS)
   *                            2 = DGPS fix
   *                            3 = PPS fix
   *                            4 = Real Time Kinematic
   *                            5 = Float RTK
   *                            6 = estimated (dead reckoning) (2.3 feature)
   *                            7 = Manual input mode
   *                            8 = Simulation mode
   *  08           Number of satellites being tracked
   *  0.9          Horizontal dilution of position
   *  545.4,M      Altitude, Meters, above mean sea level
   *  46.9,M       Height of geoid (mean sea level) above WGS84
   *                   ellipsoid
   *  (empty field) time in seconds since last DGPS update
   *  (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
   */

  int tmp;
  if (GPGGAstr[0] == '$')
  {
    tmp = getComma(1, GPGGAstr);
    hour     = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
    minute   = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
    second   = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');

    sprintf(buff, "UTC timer %2d-%2d-%2d", hour, minute, second);
    Serial.println(buff);

    tmp = getComma(2, GPGGAstr);
    latitude = getDoubleNumber(&GPGGAstr[tmp]);
    tmp = getComma(4, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp]);

    // Extract and apply North/South and East/West.
    tmp = getComma(3, GPGGAstr);
    if(GPGGAstr[tmp] == 'N')
    { // do nothing
    }
    else if(GPGGAstr[tmp] == 'S')
    {
      latitude = -latitude;
    }
    else
    {
      sprintf(buff, "Error: found %i(%c). Expected N or S.", GPGGAstr[tmp], GPGGAstr[tmp]);
      Serial.println(buff);
    }

    tmp = getComma(5, GPGGAstr);
    if(GPGGAstr[tmp] == 'E')
    { // do nothing
    }
    else if(GPGGAstr[tmp] == 'W')
    {
      longitude = -longitude;
    }
    else
    {
      sprintf(buff, "Error: found %i(%c). Expected E or W.", GPGGAstr[tmp], GPGGAstr[tmp]);
      Serial.println(buff);
    }

    sprintf(buff, "latitude = %10.4f, longitude = %10.4f", latitude, longitude);
    Serial.println(buff);

    tmp = getComma(7, GPGGAstr);
    num = getIntNumber(&GPGGAstr[tmp]);
    sprintf(buff, "Number of satellites in view = %d", num);
    Serial.println(buff);
    Serial.println("Note: If number of satelites=0, there is no GPS position fix yet \n");
  }
  else
  {
    Serial.println("No Satellites in view ...");
  }
}

void send_gps(){
  LGPS.getData(&gpsData);
  parseGPGGA((const char*)gpsData.GPGGA);
  convertPositionToDecimalForm();
  Serial.print("Longitude: ");
  Serial.println(longitude);
  Serial.print("Latitude: ");
  Serial.println(latitude);
  thing.addValue("GPS", "gps", longitude, latitude);
  thing.send();
  millis_gps = millis();
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
