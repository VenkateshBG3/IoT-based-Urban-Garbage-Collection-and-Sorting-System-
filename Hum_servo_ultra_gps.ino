#include<ESP8266WiFi.h>
#include "DHT.h"
#include <Servo.h>
#include<SoftwareSerial.h>

Servo servo;

#define DHTPIN 4     // D2

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 

int cm = 0;

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial ss(10,9); //SD3, SD2 GPIO10, GPIO9 TX RX

void ConnectTowifi(void); // Function decleration

long readUltrasonicDistance(int triggerPin, int echoPin)
{

  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Sets the trigger pin to HIGH state for 10 microseconds
 
 digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  
// Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() {
  Serial.begin(9600); 

  ConnectTowifi();
 
  dht.begin();

  ss.begin(9600);

  servo.attach(2); //D4

  servo.write(90);

  delay(2000);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  while(ss.available()> 0) {
     byte gpsData = ss.read();
     Serial.write(gpsData);
  }
   float a = analogRead(A0);
   if (a >= 500)
   {
    Serial.println("PICK ME UP!   TOXIC GASES");
   } 
   else
   {
   Serial.println("STATUS NORMAL");
   }
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  cm = 0.01723 * readUltrasonicDistance(12, 12);  //D6 pin of NodeMCU

  Serial.print("Humidity: "); 
  Serial.println(h);

  if (h<=63){
    Serial.println("Dry Wastage");
    servo.write(0);
    delay(1000);
    servo.write(90);
    delay(1000);
  }
  else {
    Serial.println("Wet Wastage");
    servo.write(180);
    delay(1000);
    servo.write(90);
    delay(1000);
  }
  Serial.print("Disance: ");
  Serial.print(cm);
  Serial.println("cm");
  delay(1000); // Wait for 100 millisecond(s)

  if (cm>=180)
  {
    Serial.println("Dustbin is 100%");
  }
  else if (cm<180 && cm>100)
  {
      Serial.println("Dustbin is 50% full");
  } 
   else if (cm<100 && cm>10)
  {
      Serial.println("Dustbin is 25% full");
  }  
  else
  {
        Serial.println("Dustbin is empty");
  }
  
}

void ConnectTowifi()   //function definiton
{
  WiFi.mode(WIFI_STA);  //nodemcu as station
  WiFi.begin("realme 7","1234567890"); //WiFi.begin(ssid,password);
  Serial.print("Connecting to WIFI");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.'); //print . until is connected
    delay(200);
  }
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP()); //To print IP address
  Serial.print("MacAddress:");
  Serial.println(WiFi.macAddress()); //To print MAC address 
}
