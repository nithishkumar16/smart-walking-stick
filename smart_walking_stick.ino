#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}

// Sensor pins

//water lvl original pins
//#define sensorPower 5 //+pin
//#define sensorPin A0 //S pin

//water lvl modified pins
const int sensorPower = 3; //+pin
const int sensorPin = A0; //S pin

//uv sensor 1
const int pingPin1 = A1; // Trigger Pin of Ultrasonic Sensor
const int echoPin1 = 2; // Echo Pin of Ultrasonic Sensor


//uv sensor 2
const int pingPin2 = 9; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 10; // Echo Pin of Ultrasonic Sensor


//uv sensor3
const int pingPin3 = 6; // Trigger Pin of Ultrasonic Sensor
const int echoPin3 = 7; // Echo Pin of Ultrasonic Sensor



// Value for storing water level
int val = 0;
int hour =0;
void setup() {
  //uv sensor
  Serial.begin(9600);
  pinMode(5, OUTPUT);    // sets the digital pin 5 as output

  //water lvl
  // Set D7 as an OUTPUT
  pinMode(sensorPower, OUTPUT);
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);
  
  Serial.begin(9600);
  pinMode(4, OUTPUT);

  //rtc
  pinMode(8, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
    setDS3231time(0,05,1,4,24,03,21);
 //rtc code
  delay(1000); // every second

  
}
//rtc code
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
int displayTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
//  return(hour);
//hour=6;
Serial.println("hour value is:");
Serial.print(hour);

unsigned int AnalogValue;

AnalogValue = analogRead(A3);
//LIGHT SENSOR 
  
  if(hour>=6)
  {

  digitalWrite(8, HIGH);
   //delay(3000);
   //digitalWrite(8, LOW);
  }
  else
  {
  if(AnalogValue>=500)
  {
   digitalWrite(8, HIGH);
   Serial.print("Light Sensor value");
   Serial.println(AnalogValue);
  }
  else
  {
    digitalWrite(8, LOW);
   Serial.print("Light Sensor value");
    Serial.println(AnalogValue);
  }
 
  }
  
}

void loop() {
  //light sensor
   displayTime(); // display the real-time clock data on the Serial Monitor,


  
  //water lvl
  //get the reading from the function below and print it
  int level = readSensor();
  
  
  if(level>300)
  {
    Serial.print("Water level higher: ");
  Serial.println(level);
  
  digitalWrite(4, HIGH);
   delay(3000);
   digitalWrite(4, LOW);
    
  }
  
  delay(1000);
  //uv sensor 1
   long duration1, cm1;
   pinMode(pingPin1, OUTPUT);
   digitalWrite(pingPin1, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin1, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin1, LOW);
   pinMode(echoPin1, INPUT);
   duration1 = pulseIn(echoPin1, HIGH);
  cm1 = microsecondsToCentimeters(duration1);

  //uv sensor 2
   long duration2, cm2;
   pinMode(pingPin2, OUTPUT);
   digitalWrite(pingPin2, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin2, LOW);
   pinMode(echoPin2, INPUT);
   duration2 = pulseIn(echoPin2, HIGH);
  cm2 = microsecondsToCentimeters(duration2);


  //uv sensor 3
   long duration3, cm3;
   pinMode(pingPin3, OUTPUT);
   digitalWrite(pingPin3, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin3, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin3, LOW);
   pinMode(echoPin3, INPUT);
   duration3 = pulseIn(echoPin3, HIGH);
  cm3 = microsecondsToCentimeters(duration3);


if(cm1<25 || cm2<25 ||cm3<25)
{
   Serial.print("Obstacle ahead ");
   //Serial.print("cm");
   Serial.print(cm1);
//   Serial.print(cm2);
//   Serial.print(cm3);
   delay(100);
   digitalWrite(5, HIGH);
   delay(3000);
   digitalWrite(5, LOW);
   
}

}
//water lvl
//This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
  //if(val>0)
}

//uv sensor
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
