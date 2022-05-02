/* General Arduino script for datalogging
    Leo Board
    Parallel LCD Digital pins 2,3,4,5,6,7
    Sdcard Module SPI
    I2C RTC module   
*/
   
//Import needed libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>

//initialise constants
float temp_c;
char filename[] = "00000000.CSV";
File myFile;
int logtime = 221; //number of seconds between logging events adjust to suit 221=5min
int sensortime = 3; //number of seconds between sensor reads 3 = 3secs
int counter = logtime - 1;
int sensor = sensortime - 1;
RTC_DS1307 RTC;
int sensorPin = 0;

// initialize the lcd library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 8, 11, 12, 13); 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()   /*----( SETUP: RUNS ONCE )----*/
{  
  lcd.begin(16, 2);
  Wire.begin();
  RTC.begin();  
  Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }      
  Serial.print("Initializing SD card...");
  
  
   pinMode(10, OUTPUT); //  Pin ten on Arduino must be free
    if (!SD.begin(SS)) { //  SS used as sdcard constant
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  DateTime now = RTC.now();
    // following line sets the RTC to the date & time this sketch was compiled
    //  RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(filename);
  // if the file opened okay, read from it:
  if (myFile) {
  // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
  // close the file:
    myFile.close();
    Serial.println("completed reading myFile");
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error reading myFile");
  }
  
}/* end setup */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{ 
  if (sensor == sensortime)
    {  
    displaylcd();
    } 
    
  if (counter == logtime)
    {
    writetosdcard();
    }
    
  sensor++;
  counter++;
  delay(1000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void displaylcd()
{
    //lcd.begin(16, 2);
    //lcd.clear();
DateTime now = RTC.now();
    lcd.setCursor(0, 0);
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
      int year=(now.year());
      year = year-2000;
    lcd.print(year);
    lcd.print(' ');
    lcd.print(' ');    
    int hour=(now.hour());
      String hourstring=String(hour);
      if (hour<10){
      hourstring="0"+hourstring;}
    lcd.print(hourstring);
    lcd.print(':');
      int minute=(now.minute());
      String minutestring=String(minute);
      if (minute<10){
      minutestring="0"+minutestring;}
    lcd.print(minutestring);
    lcd.setCursor(0, 1);
    
    float reading = analogRead(sensorPin);
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    float temperatureC = (voltage - 0.5) * 100 ;

    lcd.print((float)temperatureC);
    lcd.print((char)223);
    lcd.print("C  ");
    
sensor = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void writetosdcard()
{
DateTime now = RTC.now();
   
filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()
  
  myFile = SD.open(filename, FILE_WRITE);
  if (myFile){
    
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
  
    float reading = analogRead(sensorPin);
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    float temperatureC = (voltage - 0.5) * 100 ;

    lcd.print((float)temperatureC);
    lcd.print((char)223);
    lcd.print("C  ");
      myFile.print(temperatureC);
      myFile.print((char)223);
      myFile.println("C");
      

   
   // close the file:
      myFile.close();
        
        
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(minutestring);
    Serial.print(':');
    Serial.print(secondstring); 
    Serial.print("\t");
    Serial.print("T: ");
    Serial.print(temperatureC, 2);
    Serial.print("C ");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error writing myFile");
  }
  counter = 0;
}

/* end writetosdcard */

