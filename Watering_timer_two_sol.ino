/*
Watering timer
*/

#include <DS3231.h>
#include <Wire.h> 

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);
// Init a Time-data structure
Time  t;
int hour;
int min;
const int onPin = 13;
int onState = 0;

#define RELAY_ON 0
#define RELAY_OFF 1

#define Relay_1 10
#define Relay_2 11

void setup()
{
   digitalWrite(Relay_1, RELAY_OFF);
   digitalWrite(Relay_2, RELAY_OFF);
   pinMode(Relay_1, OUTPUT);   
   pinMode(Relay_2, OUTPUT);
   pinMode(onPin, INPUT);
   delay(4000);
   
   Serial.begin(9600); // Open serial connection to report values to host
   Serial.println("Starting up"); 
  // Initialize the rtc object
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(14, 12, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(31, 10, 2014);   // Set the date to January 1st, 2014
}

void loop()
{
    // Get data from the DS3231
    t = rtc.getTime();
    hour = (t.hour);
    min = (t.min);
    
    onState = digitalRead(onPin);
    
    while(onState == HIGH){
      digitalWrite(Relay_1, RELAY_ON);
      digitalWrite(Relay_2, RELAY_ON); 
      Serial.println(" Water on");
      onState = digitalRead(onPin);  
  }
    
    if (hour == 6) {
      if (min < 15){
      digitalWrite(Relay_1, RELAY_ON);
      digitalWrite(Relay_2, RELAY_ON);
      }
      if (min > 15){
      digitalWrite(Relay_1, RELAY_OFF);
      digitalWrite(Relay_2, RELAY_OFF); 
      }
    
    }
    if (hour == 18) {
      if (min < 15){
      digitalWrite(Relay_1, RELAY_ON);
      digitalWrite(Relay_2, RELAY_ON);
      }
      if (min > 15){
      digitalWrite(Relay_1, RELAY_OFF);
      digitalWrite(Relay_2, RELAY_OFF); 
      }
    }
  // Print the values to the serial port
    // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" ");

  // Send time
  Serial.print(rtc.getTimeStr());
  Serial.println(" ");
  
 
  delay(1000);
}
