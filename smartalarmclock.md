# Building a Smart Alarm Clock

This tutorial makes the assumption that you have played around with the Arduino, Understand the basics of the program structure and have at a minimum used LEDs and buttons.

__ TODO: better explain the pins on each sensor, ... __

## Just Your Basic Clock

To get started, we are going to build a alarm clock that is not connected to the internet. The code developed in this section will act as part of the framework for building an alarm clock connected to the internet further down.

### Displaying Digits
The first part of the challenge of building a clock is to learn how to display numbers using a common anode 7 segment 4 digit display. The 7 segment display is built up of 7 LEDs arranged as shown in the image below. Each of the LEDs is considered a segment, which can be used in conjunction to draw numeric values on the display. While we call this a 7 segment display, technically there are 8 LED segments that make up the display, with the last one being for a decimal point. The data sheet for this component can be found [here](https://e-radionica.com/productdata/LD3361BS.pdf).

![<4-digit_7-segment_display image>](/img/DI-0017_2.jpg)

7 Segment displays come in two varieties - common anode and common cathode. Both display types contain the word _common_ in front of them. This simply refers to the fact that for all the LED segments on a digit are linked by at one end. The anode display links the LEDs at the VCC line, which means that if you wish to turn an LED on, you need to change it to a ```LOW``` value. The cathode display is the opposite to the anode, where the common end of the LEDs is connected to the GND line, and the ```HIGH``` value is used to turn a LED segment on. Ultimately, there is no difference in functionality of the two kinds, you just need to invert the logic for your program if the LED segments do the opposite of what you expect. As such, for the remainder of this project we are going to assume when talking about any 7 segment display that it is of the common anode variety.

The 7 segment display works by assigning different lettering to each segment, as shown in the image below. What this means is that if you set the pin attached to ```A``` to ```LOW``` then it will turn on.

![<7-segment_display lettering image>](/img/7-segment display.png)

Different combinations of different segments lighting up make different shapes. The table below shows the segments required for drawing a zero on the display. Have a go filling out the rest of the table for the other 9 digits.

| Digit | A | B | C | D | E | F | G | H |
| ----- |:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0     | ```LOW``` | ```LOW``` | ```LOW``` | ```LOW``` | ```LOW``` | ```LOW``` | ```HIGH``` | ```HIGH``` |
| 1     | ||||||||
| 2     | ||||||||
| 3     | ||||||||
| 4     | ||||||||
| 5     | ||||||||
| 6     | ||||||||
| 7     | ||||||||
| 8     | ||||||||
| 9     | ||||||||

Now that you have a basic understanding of how a 7-segment display works, we are going to have a go building a simple circuit containing one, and program it to display the number zero on all four digits. To start off we are going to build the circuit as displayed in the image below. Once you finish building the circuit it is then time to move on to the programming.

![<4-digit_7-segment_display circuit image>](/img/7segment4digitDisplay_bb.png)

First off we need to define our pin values in the code. The large advantage of doing so is to allow us to reuse our code later on. If we decide later to use a different set of pins to control this display, all we have to do is change one value rather than multiple throughout our code.

```c++
#define A 11
#define B 7
#define C 4
#define D 2
#define E 1
#define F 10
#define G 5
#define DP 3

#define firstDigit 12
#define secondDigit 9
#define thirdDigit 8
#define fourthDigit 6
```
Now that we have defined all our pins, we need to set them up so the Arduino knows that they are all output pins...

```c++
void setup() {
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(F, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(DP, OUTPUT);

    pinMode(firstDigit, OUTPUT);
    pinMode(secondDigit, OUTPUT);
    pinMode(thirdDigit, OUTPUT);
    pinMode(fourthDigit, OUTPUT);
}
```

To get this ball rolling, we are first going to start by making our display show four 0's on screen. To do this, we need to put the following code in the control loop. The code below turns on the pins for all four digits, and then to turn on the LED segments for the shape zero.

```c++
    // Turns the pins that you want to display a number on
    digitalWrite(firstDigit, HIGH);
    digitalWrite(secondDigit, HIGH);
    digitalWrite(thirdDigit, HIGH);
    digitalWrite(fourthDigit, HIGH);

    // Form the number 0 on every digit pin that is set to High
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    digitalWrite(DP, HIGH);
```

Once you have finished typing up your program, download it to the arduino and see what happens. Provided that everything goes smoothly, you should see all four digits displaying the number zero.

**Exercise 1**: Write 9 helper functions which display a particular number. In other words, you should have 9 functions structured similarly to the code below. _Hint: Remember that these functions are to draw the number, not to control which digit you are drawing them at._

```c++
void displayZero(){
    // put code to display zero here.
}
```

**Exercise 2**: Make your display increment the number of all digits by 1 every second. When you reach 9, jump back down to 0 to continue incrementing.


### Shift Registers

So you've probably noticed now that you have had a play with the display that it uses a lot of pins to control it. In fact, it uses 12 of the 14 available digital pins on your Arduino Uno. Realistically you have two choices if you want to continue adding in sensors to your project:

1. Upgrade your Arduino Uno board to a board with a larger amount of pin mounts, such as the Arduino Mega.
2. Introduce a shift register into the circuit

The choice of which is better is up to you, but in terms of size and cost, for this project a shift register is a better choice. The shift register we will be using looks similar to the one featured in the image below. The data sheet can be found [here](https://cdn.shopify.com/s/files/1/0045/8932/files/EXPAND_ShiftRegisterModule.pdf?100739).

![<shift_register image>](/img/index.jpeg)

Shift registers are a chip which allows for additional inputs into the circuit. Shift registers have two different functionalities. The first of these functionalities is to use serial communcication to collect information from sensors. The second functionality is to use parallel communication to allow for multi-pin output control. It is the later that we are going to experiment with below.

In this part of the tutorial we are going to introduce the shift register into the circuit and attempt to complete the same task as in the previous section, drawing zeros on the display. The circuit to construct should look similar to the one below.

![<ShiftRegister7segment4digitDisplay image>](/img/ShiftRegister7segment4digitDisplay_bb.png)

As you can see from your now complete circuit, we have moved the connections from A-G and DP from the arduino and wired them directly into the shift register. As such, our program needs to reflect this. Add the following to our defines, making sure to remove the letters A-G and DP from the previous program.

```c++
#define dataPin 5
#define latchPin 6
#define clockPin 7
```
Similar to above, we also need to modify the setup function, making sure to remove the letters A-G and DP from the previous program.

```c++
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
```

To output data to the shift register, there is a simple one-line command that we need to learn - ```shiftOut(dataPin, clockPin, MSBFIRST, output_value)```. What this command does is tell the shift register to tell the output values to turn on or off accordingly. The ```output_value``` is a number between 0 and 255, which maps the binary values to the different segments. For example, the ```output_value``` for drawing zero would look like ```0b00000011```, which represents in a binary format the combination drawn in the table earlier in the tutorial. By adding the binary value for each number as a constant at the top of your program, you can modify the helper function to look like the following.

```c++
    const int zero  = 0b00000011;

    void displayZero(){
        shiftOut(dataPin, clockPin, MSBFIRST, zero);
    }
```

Similar to before, download the finished program onto your Arduino, and admire your lovely display showing four zeros for the world to see.

**Exercise 3**: Modify your helper functions above to use the binary structure and shift register to display the incrementing numbers, in the same pattern as exercise 2.

### 0000 to 9999

Up until now, we have been displaying the same number on all four digits of our display. It is not as trivial to display different numbers on each digit. This is because the LED segments connected to segment A are the same pin for all four digits (and the same for every other joining segment). As such, we need to develop a way so that we can write unique numbers to each of the different digits.

To start off with, we are going to modify our program to contain the following helper function. The idea of this helper function is so that we can turn a integer value into the functions that we wrote earlier in this tutorial. The code should look something similar to below.

```c++
void showdigit (int digit) {
  switch (digit) {
      case 0: displayZero();
          break;
      case 1: displayOne();
          break;
      case 2: displayTwo();
          break;
      case 3: displayThree();
          break;
      case 4: displayFour();
          break;
      case 5: displayFive();
          break;
      case 6: displaySix();
          break;
      case 7: displaySeven();
          break;
      case 8: displayEight();
          break;
      case 9: displayNine();
          break;
      default: break;
  }
}
```

With this helper function implemented, we are going to write a function called ```showdigits``` which takes in a number and displays this number on screen. As it is four digits, the easiest way to gain the individual digits to display is through division. If we divide the input number by 1000, we get the first digit. If we divide the input number by 100 we get the second digit, and so on. Using this idea, remainders when dividing and recursion, we can write the function below to display a four digit number.

```c++
// showing 4 digits
void showdigits (int number) {
    showdigit(number / 1000); // segments are set to display "1"
    digitalWrite(firstDigit, HIGH);
    digitalWrite(secondDigit, LOW);
    digitalWrite(thirdDigit, LOW);
    digitalWrite(fourthDigit, LOW);
    delay (1);

    number = number % 1000;
    digitalWrite(firstDigit, LOW);
    showdigit(number / 100);
    digitalWrite(secondDigit, HIGH);
    delay (1);

    number = number % 100;
    digitalWrite(secondDigit, LOW);
    showdigit(number / 10);
    digitalWrite(thirdDigit, HIGH);
    delay (1);

    number = number % 10;
    digitalWrite(thirdDigit, LOW);
    showdigit(number);
    digitalWrite(fourthDigit, HIGH);
    delay (1);
}
```

Now that we have the two helper functions in place to help us write unique 4 digit numbers, have a go modifying your main control loop so that it displays the number ```1234```.

**Exercise 4**: Using the code above, modify your existing code to write a counter starting at 0000 and counting up to 9999, incrementing the number by 1 each second. For example, your counter should start at 0000, then display 0001, followed by 0002 and so on.

### My Display is Flickering??

Now it is more than likely for the exercise above you have written code similar to what is below in your main control loop.

```c++
for(int i = 0; i < 10000; i++){
    showdigits(i);
    delay(1000);
}
```

For those who did, you may notice that your display isn't showing the number correctly. As it is, your display will be flickering and looking all kinds of funny. Not to worry - the problem here is the structure of the main loop. In the code below, we have written a code sample that is identical to the one above, however in not using the delay and using ```millis()```, we remove the flickering.


```c++
for (int i = 0; i < 10000; i++) {
    long currenttime = millis();
    while (currenttime + 1000 > millis()) {
        showdigits(i);
    }
}
```

Now that you have solved the flickering problem, it is time to move onto adding the clock to the circuit.

### Real Time Clock Module

So now is the time you have all been waiting for - putting a real time clock into the circuit. But first, we are going to just play with this module alone. A real time clock module looks similar to the sensor shown below. The datasheet for the sensor is available [here](https://datasheets.maximintegrated.com/en/ds/DS1307.pdf).

![<tinyRTC image>](/img/sku_161172_1.jpg)

Looking at the sensor you will notice that there are two groups of input/output pins, one on the left and the other on the right, which control this sensor. These groups are exactly the same with one minor difference. The larger group has additional pins which can be used for advanced usage of this sensor. As such, we will be using the smaller of the two groups in this tutorial to start off.

This is a sensor that using the I2C ports can provide the time and calendar date. There are several libraries available online to interface with this sensor, for example [here](https://github.com/adafruit/RTClib/archive/master.zip) or  [here](https://www.elecrow.com/wiki/index.php?title=File:RTC.zip). However for the purposes of learning about the sensor, we are going to use the I2C pins on our Arduino and get the information manually.

This sensor by itself is very easy to wire into the Arduino. The first circuit we are going to build consequently is the one below.  

![<TinyRTC circuit image>](/img/tiny-rtc_bb.png)

This clock module has two different functionalities we are going to use, set and get.


```c++
#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68  // the I2C address of Tiny RTC
```

```c++
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
```

```c++
byte decToBcd(byte val){   return ( (val/10*16) + (val%10) );  }
byte bcdToDec(byte val){   return ( (val/16*10) + (val%16) );  }
```

To begin with, we are going to write the function to set the time of the clock.

```c++
// Function to set the current time, change the second&minute&hour to the right time
void setClock(int second, int minute, int hour, int dayOfWeek, int dayOfMonth, int month, int year) {
    this.second = second;
    this.minute = minute;
    this.hour   = hour;
    this.dayOfWeek = dayOfWeek;
    this.dayOfMonth = dayOfMonth;
    this.month = month;
    this.year = year;

    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write(decToBcd(0));
    Wire.write(decToBcd(second));    
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(hour));      
    Wire.write(decToBcd(dayOfWeek));
    Wire.write(decToBcd(dayOfMonth));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.endTransmission();
}
```
```c++
void getTime(){
     Wire.beginTransmission(DS1307_I2C_ADDRESS);
     Wire.write(decToBcd(0));
     Wire.endTransmission();
     Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
     second     = bcdToDec(Wire.read() & 0x7f);
     minute     = bcdToDec(Wire.read());
     hour       = bcdToDec(Wire.read() & 0x3f);
     dayOfWeek  = bcdToDec(Wire.read());
     dayOfMonth = bcdToDec(Wire.read());
     month      = bcdToDec(Wire.read());
     year       = bcdToDec(Wire.read());
}
```

```c++
void printTime(){
    Serial.print(hour, DEC);
    Serial.print(":");
    Serial.print(minute, DEC);
    Serial.print(":");
    Serial.print(second, DEC);
    Serial.print("  ");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(year,DEC);
    Serial.print("  ");
    Serial.println();
}
```

TODO talk about battery and requirement for setting the time

TODO talk about reading serial needing to be on a different frequency

### Building A Simple Clock

Now that we understand how the clock module works, we are going to modify the circuit to include the display. Instead of printing the time to serial, we are going to display the time on our displays. To begin with you need to build the circuit featured below.

![<Simple Clock Circuit Image>](/img/simple-clock_bb.png)

**Exercise 5**: Using the circuit above, join your clock code and display code to make the display read what the time is.

### Measuring Temperature

The data sheet can be found [here](https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf)

![<Simple Alarm Clock Circuit Image>](/img/bmp180.jpeg)

There are two ways we can choose to program a sensor by Adafruit, from scratch using the I2C data, or using the [Adafruit Sensor Library](https://github.com/adafruit/Adafruit_Sensor). This seems like a perfect time in the tutorial to introduce using external Arduino libraries. Installing external libraries is a very simple process which involves downloading the packaged code from the source, and placing it in a correct folder. If you have never installed a library before, the Arduino Guide has an excellent explanation of how you can install different libraries which you can find [here](https://www.arduino.cc/en/Guide/Libraries).

**Exercise 6**: Using the links in the paragraph above, install the Adafuit Sensor Library. Once you have finished, restart your Arduino IDE.

Now that you have all the correct libraries installed, it is time to build this sensor into our circuit! The circuit you need to build is featured below.

![](/img/temperatureClock_bb.png)

To include the libraries in what we have done so far, we need to add the following include lines to the base code.

```c++
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
```

Using this library means we can treat the sensor values similar to an object. As a consequence though, we need to add the following global declarations into our base code. These allow us to store and access the sensor data later in the program.

```c++
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); //barometer sensor
sensors_event_t event;
float temperature;
```

Actually accessing the sensor values that are measured is super easy. To make our lives easier, we are going to write a function called ```displayTemperature``` which gets the temperature and displays it to screen for us. The code should look like below.

```c++
void displayTemperature(){
    bmp.getTemperature(&temperature);
    temperature*=100;
    showdigits((int)temperature);
}
```

This code does three tasks. The first task gets the temperature values and stores them in our temperature variable. The second one multiples the value by 100 so that it is a number we could display on screen with the decimal point in the correct location. The last step utilises your code from earlier to write the temperature on screen.

**Exercise 7**: Using the above code, write a new function that displays the temperature using 3 digits and draws a C or F to represent what temperature type you are measuring on the final digit.

**Exercise 8**: Modify your code such that it can deal with negative temperatures being displayed.

**Exercise 9**: Modify your code such that if the button is pressed, the display will show the temperature, and if the button is not pressed, it will display the time. 

### Extra Functionalities
Congratulations, you have now built a simple alarm clock which has the capability of measuring temperature. By increasing your circuit to contain more LEDs or buttons, you can now complete the following exercises. Additionally, you may want to consider using a piezo sensor to simulate the alarm sound, which can be wired into your circuit similar to the image below. Piezo sensors are programmed identically to LEDs, where ```HIGH``` will make the sensor emit a noise, and ```LOW``` will turn the sound off.

![](/img/piezoCircuit_bb.png)

**Exercise 10**: Using the buttons in your circuit, write a function to allow you to change and set the time manually.

**Exercise 11**: Introduce an alarm system into your clock, such that you can input the time an alarm should go off, and it will sound an alarm.

**Exercise 12**: Introduce a timer into your clock, such that it can start a timer for a number of minutes, and set an alarm off when the timer ends.

## Making Your Clock Smart
TODO

## Final Remarks
TODO
