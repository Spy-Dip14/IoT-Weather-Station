#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

int t;
int h;
//int p;
int p0;
double T,P,a;

//----------------------------------------Include the NodeMCU ESP8266 Library
//----------------------------------------see here: https://www.youtube.com/watch?v=8jMr94B8iN0 to add NodeMCU ESP12E ESP8266 library and board (ESP8266 Core SDK)

//----------------------------------------
//----------------------------------------Include the DHT Library
#include "DHT.h"
#include <SFE_BMP180.h>
//----------------------------------------

#define DHTTYPE DHT11 //--> Defines the type of DHT sensor used (DHT11, DHT21, and DHT22), in this project the sensor used is DHT11.

const int DHTPin = 14; //--> The pin used for the DHT11 sensor is Pin D1 = GPIO5
DHT dht(DHTPin, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

//----------- Enter you Wi-Fi Details---------//
char ssid[] = "ECE project"; //SSID
char pass[] = "stcet@ece"; // Password
//-------------------------------------------//



//-------------------------------------------//


//------------------//

//----------- Channel Details -------------//
unsigned long Channel_ID = 1756108; // Channel ID
const int Field_number1 = 1;// To which field to write data (don't change)
const int Field_number2 = 2;
const int Field_number3 = 3; 
const char * WriteAPIKey = "YQEYWZ2HX6W36VCP"; // Your write API Key
// ----------------------------------------//



WiFiClient  client;

SFE_BMP180 pressure;
#define ALTITUDE 1655


void setup()
{
  Serial.begin(115200);
  dht.begin();  //--> Start reading DHT11 sensors
  
  //delay(500);
  pressure.begin();
  //delay(500);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
   pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On 
  
  if(!bmp.begin())
   {
     Serial.println("Could not find a valid BMP085 sensor,check wiring!");
    while(1){}
   }
  
}
void loop()
{
  internet();
 
  measure();
 
  upload();
}
void upload()
{
  internet();
   ThingSpeak.writeField(Channel_ID, Field_number1,t, WriteAPIKey);
   ThingSpeak.writeField(Channel_ID, Field_number2,h, WriteAPIKey);
   ThingSpeak.writeField(Channel_ID, Field_number3,int(p0), WriteAPIKey);
}

void measure()
{
 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor !");
    delay(500);
    return;
  }
  //String Temp = "Temperature : " + String(t) + " °C";
  //String Humi = "Humidity : " + String(h) + " %";
  Serial.println();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C");
  Serial.println();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %");

//char status;
          //double T,P,p0,a;
          //status = pressure.startTemperature();
          //if (status != 0)
          //{
            //delay(status);
            //status = pressure.getTemperature(T);
            //if (status != 0)
            //{
             
             // status = pressure.startPressure(3);
             // if (status != 0)
             // {
                // Wait for the measurement to complete:
               // delay(status);
        
                //status = pressure.getPressure(P,T);
               // if (status != 0)
                //{
         
                  //p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
                  //a = pressure.altitude(P,p0);
                //}
                //else Serial.println("error retrieving pressure measurement\n");
             // }
              //else Serial.println("error starting pressure measurement\n");
            //}
           // else Serial.println("error retrieving temperature measurement\n");
          //}
          
          //p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          p0=bmp.readPressure();
          Serial.println();
          //Serial.print("Relative (sea-level) Pressure: ");
          //Serial.print(p0,2);
          //Serial.print(" mb, ");
          //Serial.print(p0*0.0295333727,2);
          //Serial.print(" inHg");
          //Serial.println();//
          //Serial.print("Altitude: ");
          //Serial.print(a);
          //Serial.print("m");
          Serial.print("Pressure: ");
          Serial.print(p0);
          Serial.print(" Pa");

          Serial.println();
          delay(500);

 // delay(5000);  // Pause for 5 seconds.
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}
