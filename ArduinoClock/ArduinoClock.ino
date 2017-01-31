#include "Wire.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#define RTC_Address 0x68   // the I2C Address of the Tiny RTC

#define firstDigit  2
#define secondDigit 3
#define thirdDigit  4
#define fourthDigit 5
#define dataPin     6
#define latchPin    7
#define clockPin    8
#define button1     9
#define button2    10
#define button3    11
#define button4    12
#define button5    13
#define LEDred     A0
#define LEDyellow  A1
#define LEDgreen   A2
#define piezo      A3


/**********************************************************************************************
 **  DRAWING NUMBERS ON A SEGMENTED DISPLAY
 **********************************************************************************************/
const int zero  = 0b00000011; 
const int one   = 0b10011111;
const int two   = 0b00100101;
const int three = 0b00001101;
const int four  = 0b10011001;
const int five  = 0b01001001;
const int six   = 0b01000001;
const int seven = 0b00011111;
const int eight = 0b00000001;
const int nine  = 0b00001001;
const int dp    = 0b11111110;


void showDigits(int number){
    digitalWrite(firstDigit,  LOW); 
    digitalWrite(secondDigit, LOW); 
    digitalWrite(thirdDigit,  LOW); 
    digitalWrite(fourthDigit, LOW); 
    
    convertDigit(number/1000); 
    digitalWrite(firstDigit,  HIGH); 
    delay(1); 
    
    number = number%1000; 
    digitalWrite(firstDigit,  LOW); 
    delay(1); 
    convertDigit(number/100); 
    digitalWrite(secondDigit, HIGH); 
    delay(1); 
    
    convertDigit(dp); 
    delay(1); 
        
    number = number%100; 
    digitalWrite(secondDigit,  LOW); 
    delay(1); 
    convertDigit(number/10); 
    digitalWrite(thirdDigit, HIGH); 
    delay(1); 
    
    number = number%10; 
    digitalWrite(thirdDigit,  LOW); 
    delay(1); 
    convertDigit(number); 
    digitalWrite(fourthDigit, HIGH); 
    delay(1); 
    
    digitalWrite(fourthDigit, LOW); 
    delay(1); 
}

void convertDigit(int digit){
    switch (digit) {
        case 0: displayValue(zero);  break; 
        case 1: displayValue(one);   break; 
        case 2: displayValue(two);   break; 
        case 3: displayValue(three); break; 
        case 4: displayValue(four);  break; 
        case 5: displayValue(five);  break; 
        case 6: displayValue(six);   break; 
        case 7: displayValue(seven); break; 
        case 8: displayValue(eight); break; 
        case 9: displayValue(nine);  break; 
        default: displayValue(dp);   break;   
    }
}

void displayValue(byte value){
    digitalWrite(latchPin, LOW); 
    shiftOut(dataPin, clockPin, MSBFIRST, value);
    digitalWrite(latchPin, HIGH); 
}

/**********************************************************************************************
 **  REAL TIME CLOCK FUNCTIONS
 **********************************************************************************************/

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; 

byte decToBcd(byte value){  return ( (value/10*16) + (value%10) );  }
byte bcdToDec(byte value){  return ( (value/16*10) + (value%16) );  }

void setClock(int s, int m, int h, int dow, int dom, int mon, int y){
    Wire.beginTransmission(RTC_Address); 
    Wire.write(decToBcd(0)); 
    Wire.write(decToBcd(s)); 
    Wire.write(decToBcd(m)); 
    Wire.write(decToBcd(h)); 
    Wire.write(decToBcd(dow)); 
    Wire.write(decToBcd(dom)); 
    Wire.write(decToBcd(mon)); 
    Wire.write(decToBcd(y)); 
    Wire.endTransmission();   
}

void getTime(){
    Wire.beginTransmission(RTC_Address); 
    Wire.write(decToBcd(0)); 
    Wire.endTransmission(); 
    
    Wire.requestFrom(RTC_Address, 7);
    second    = bcdToDec(Wire.read() & 0x7f); 
    minute    = bcdToDec(Wire.read()); 
    hour      = bcdToDec(Wire.read() & 0x3f);  
    dayOfWeek = bcdToDec(Wire.read()); 
    dayOfMonth= bcdToDec(Wire.read());
    month     = bcdToDec(Wire.read());
    year      = bcdToDec(Wire.read());
}

void displayTime(){
    getTime(); 
    int number = 100* hour + minute; 
    showDigits(number);   
}

void printTime(){
    Serial.print(hour, DEC);
    Serial.print(":"); 
    Serial.print(minute, DEC);
    Serial.print(":"); 
    Serial.print(second, DEC);
    Serial.print(" "); 
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/"); 
    Serial.print(year, DEC);
    Serial.println(" ");    
}

/**********************************************************************************************
 **  TEMPERATURE FUNCTIONS
 **********************************************************************************************/

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); 
float temperature; 

void serialTemperature(){
    bmp.getTemperature(&temperature); 
    Serial.print(temperature);
    Serial.println("*C");    
}

void displayTemperature(){
    bmp.getTemperature(&temperature); 
    temperature *= 100; 
    showDigits((int)temperature);  
}

/**********************************************************************************************
 **  CONTROL LOOPS
 **********************************************************************************************/
void setup(){
    pinMode(firstDigit, OUTPUT); 
    pinMode(secondDigit, OUTPUT); 
    pinMode(thirdDigit, OUTPUT); 
    pinMode(fourthDigit, OUTPUT); 
    digitalWrite(firstDigit,  LOW); 
    digitalWrite(secondDigit, LOW); 
    digitalWrite(thirdDigit,  LOW); 
    digitalWrite(fourthDigit, LOW); 

    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT); 
    pinMode(clockPin, OUTPUT); 
    
    pinMode(button1, INPUT); 
    pinMode(button2, INPUT); 
    pinMode(button3, INPUT); 
    pinMode(button4, INPUT); 
    pinMode(button5, INPUT); 

    pinMode(LEDred, OUTPUT);
    pinMode(LEDyellow, OUTPUT);
    pinMode(LEDgreen, OUTPUT);
    digitalWrite(LEDred,  LOW); 
    digitalWrite(LEDyellow, LOW); 
    digitalWrite(LEDgreen,  LOW); 
    
    pinMode(piezo, OUTPUT);
    digitalWrite(piezo, LOW); 

    Wire.begin(); 
    bmp.begin();
    
    Serial.begin(9600); 
}

void loop(){
    if(digitalRead(button1) == HIGH){
        displayTemperature(); 
        serialTemperature(); 
        digitalWrite(LEDred, HIGH); 
     }else{
        displayTime(); 
        digitalWrite(LEDred, LOW);
     }
}






