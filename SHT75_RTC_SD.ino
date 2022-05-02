// Logging temperature from a Sensirion SHT75

#include <SHT1x.h>
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
RTC_DS1307 RTC;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 8; 

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  3
#define clockPin 2
SHT1x sht1x(dataPin, clockPin);

char filename[] = "00000000.CSV";
File myFile;
int time = 300;//number of seconds between logging events adjust to suit 60=1min 300=5min 900=15min
int sensor_read = 5; //number of seconds between sensor reads =3secs
int sensor = 0; 
int counter = time - 1;

void setup()
{
   Serial.begin(9600); // Open serial connection to report values to host
   Serial.println("Starting up");
   Wire.begin();
   RTC.begin();
   
     if (! RTC.isrunning()) 
 {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
}
  DateTime now = RTC.now();

Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
    pinMode(10, OUTPUT);
   
    if (!SD.begin(8))
{
    Serial.println("initialization failed!");
    return;
      
}
    Serial.println("initialization done.");
      

    
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
 
    
}
void loop()
{  
  delay(1000);
    counter++;
    sensor++;
    
if (sensor == sensor_read)
  {
  readsensor();
  }  
if (counter == time)
  {
  writetosdcard();
  }

}/* --(end main loop )-- */

void readsensor()
{
  float temp_c;
  float humidity; 
  DateTime now = RTC.now();
  
    // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  //temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
  
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
    // Print the values to the serial port
    Serial.print("\t");
    Serial.print("Temperature: ");
    Serial.print(temp_c, 2);
    Serial.print("C / ");
    //Serial.print(temp_f, DEC);
    Serial.print(" Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  
    sensor=0;
}
   void writetosdcard()
{
    DateTime now = RTC.now();
      float temp_c;
      float humidity;
      
  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  //temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
   
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
  
      myFile.print("  Temperature (oC): ");
      myFile.print(temp_c);
    
      myFile.print("  Humidity (%): ");
      myFile.println((float)humidity);  
    	//close the file:
      myFile.close();
  
  } else {
    // if the file didn't open, print an error:
    Serial.println("error writing myFile");
  }
  counter = 0;
}


