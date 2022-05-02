                                                                                                                                                                                                                                                                                                                                               /******************************************************************************

Ethylene voltage logging sketch
******************************************************************************/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <string.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20,16,2); // set the LCD address to 0x20
RTC_DS1307 RTC;
char filename[] = "00000000.CSV";
File myFile; 

const int AO_pin = 0;
int logtime = 1200; //number of seconds between logs 1200 = 5 mins
int timer = -1;
int readtime = 40; //number of readings to average 40 = 10 secs
int counter = 0;
float voltage=0;
float average=0;
float LogAverage=0;
float LogCount=0;

float cal0 = 0.6181; //voltage reading at zero ppm
float cal100 = 1.1397; //voltage reading at 100 ppm

int logger = 1; //logger number

/******************************************************************************/

void setup () {
  lcd.init(); // initialize the lcd
  lcd.backlight(); 
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
voltage = sensorValue * (3.331 / 1023.0);
average = (average + voltage);
LogAverage = (LogAverage + voltage);

//Serial.println(counter);
//Serial.println(voltage,4);
//if(average<0){
//  average=0;
//}

if (counter >= readtime){   
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
                Serial.print("    ");
                Serial.print(average/counter, 4);
                Serial.print("    ");
                float eth = (100 / (cal100 - cal0))*((average/counter) - cal0);
                Serial.print(eth, 2);
                Serial.println(" ppm");
                Serial.print("    ");
                lcd.setCursor(0, 0);
                lcd.print("#");
                lcd.print(logger);
                lcd.print(" ");
                lcd.print(now.hour(), DEC);
                lcd.print(':');
                lcd.print(now.minute(), DEC);
                lcd.setCursor(0, 1);
                lcd.print("Eth: ");
                lcd.print(eth);
                lcd.print(" ppm  ");                  
 		average=0;
                counter=0;
                voltage=0;
}


if (timer >= logtime){
                filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
                filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
                filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
                filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
                filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
                filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
                filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
                filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()
  //Check file name exist?
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
                
                myFile.print(" ");
                myFile.print(LogAverage/LogCount, 4);
                float eth = (100 / (cal100 - cal0))*((LogAverage/LogCount) - cal0);
                myFile.print(" ");
                myFile.print (eth, 2);
                myFile.println(" ppm");
                myFile.close();
                
                LogAverage = 0;
                LogCount = 0;
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
}  
  }
  
timer++;
counter++;
LogCount++;
delay(250);
}
  
/******************************************************************************/
