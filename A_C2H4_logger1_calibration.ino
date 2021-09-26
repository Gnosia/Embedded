                                                                                                                                                                                                                                                                                                                                               /******************************************************************************
Ethylene Logger Calibration Sketch
******************************************************************************/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <string.h>
RTC_DS1307 RTC;
char filename[] = "00000000.CSV";
File myFile; 

const int AO_pin = 0;
int logtime = 240; //number of seconds between logs 240 = 1min
int timer = -1;
long counter = 1;
float voltage;
float average;
float sum;

int logger = 1; //logger number

/******************************************************************************/

void setup () {
  		Serial.begin(9600);
  		//while (!Serial);
                delay(3000);
                // replaced while with delay for DC jack operation
   analogReference(EXTERNAL); // use AREF for reference voltage              
   Wire.begin();
   RTC.begin();
      if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    }
    else{
    // following lines sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    //Serial.println("Setting logger time to system time"); 
      
      Serial.print("Initializing SD card...");
      pinMode(10, OUTPUT);
      if (!SD.begin(10,11,12,13)) {
            Serial.println("initialization failed.");
            return;
    }
      else{
      Serial.println(" OK");
    }				
  }
}  	
  	
/******************************************************************************/
void loop() {
DateTime now = RTC.now();

int sensorValue = analogRead(A0);
voltage = sensorValue * (3.332 / 1023.0);
sum = (sum + voltage);
average = sum / counter;

   Serial.println();
   Serial.print("#");
   Serial.print(logger);
   Serial.print(" ");
   Serial.print(now.year(), DEC);
   Serial.print('/');
   Serial.print(now.month(), DEC);
   Serial.print('/');
   Serial.print(now.day(), DEC);
   Serial.print(' ');
   Serial.print(now.hour(), DEC);
   Serial.print(':');
   Serial.print(now.minute(), DEC);
   Serial.print(':');
   Serial.print(now.second(), DEC);
  Serial.print("\t");               
  Serial.print(counter);
  Serial.print("\t");
  Serial.print(voltage, 6);
  Serial.print("\t");
  Serial.print(sum);
  Serial.print("\t");
  Serial.println(average, 6);

if (timer >= logtime){
  filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
  filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
  filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
  filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
  filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
  filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
  filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
  filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()
 
   if (SD.exists(filename)) {  
      myFile = SD.open(filename, FILE_WRITE);
      if (myFile){
                myFile.print("#");
                myFile.print(logger);
                myFile.print(" ");
                myFile.print(now.year(), DEC);
                myFile.print('/');
                myFile.print(now.month(), DEC);
                myFile.print('/');
                myFile.print(now.day(), DEC);
                myFile.print(' ');
                int hour=(now.hour());
                String hourstring=String(hour);
                if (hour<10){
                hourstring="0"+hourstring;}
                myFile.print(hourstring);
                myFile.print(':');
                int minute=(now.minute());
                String minutestring=String(minute);
                if (minute<10){
                minutestring="0"+minutestring;}
                myFile.print(minutestring);
                myFile.print(':');
                int second=(now.second());
                String secondstring=String(second);
                if (second<10){
                secondstring="0"+secondstring;}
                myFile.print(secondstring);
                myFile.print(' ');               
                myFile.print(counter);
                 myFile.print(' ');
                myFile.print(voltage, 6);
                 myFile.print(' ');
                myFile.print(sum);
                 myFile.print(' ');
                myFile.println(average, 6);                         
                myFile.close();
                timer = 0;
               
 }
   }
else {
Serial.print(filename);
Serial.println(" doesn't exist.");
Serial.print("Creating new file ");
Serial.println(filename);
Serial.println();
myFile = SD.open(filename, FILE_WRITE);
myFile.close();
counter = 1;
sum=0;
average=0;
}
}
delay(250);
counter++;
timer++;
}
  
   
    
	










/******************************************************************************/
