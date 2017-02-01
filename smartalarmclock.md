# Building a Smart Alarm Clock

__TODO fix fritzing diagrams to have the correct pins__

If you are anything like me you hate waking up in the mornings and your alarm clock is your worst nightmare. The dreaded _"beep! beep! beep!"_ in the morning is enough to drive anyone insane. But how does it all work? Is there a way we can make it smarter to maximise the amount of sleep you can get every morning?

This tutorial aims to answer all your questions about how a smart alarm clock works and what you can do to maximise your sleeping time. To accomplish this, in this tutorial we are going to build a smart alarm clock from the ground up! We are going to start with simple electronic components and first build a _dumb_ alarm clock. Once we complete this challenge, we are then going to take what we originally built and connect it to the Internet to make it _smart_.

Smart Alarm clocks have a lot more functionality that a normal alarm clock would never be able to match. For example, what if you never had to set an alarm again?  Instead, your clock automatically looked at your calendar and chose the best possible time to wake you up! What if your clock could account for road closures and traffic and still get you up in time for work? While only some of these solutions will be introduced in this tutorial, you will come to see that all are possible with a bit of programming.

This tutorial does assume some things:

- You have some basic knowledge with the Arduino framework and IDE
- You have spent some time familiarising yourself with an Arduino board and building circuits
- You have a basic knowledge of programming in C++, understand the basic Arduino program structure, and can comfortably program both LEDs and buttons.

With these assumptions in place, the tutorial will walk you through an introduction of each new component as it's added to the circuit, basic examples of how to use it and exercises to complete yourself using your new sensor. If you don't feel quite ready to start yet, Adafruit has a really good introduction to Arduino tutorial which you can find [here](https://learn.adafruit.com/lesson-0-getting-started/overview).

## Tutorial Structure

This tutorial is designed to walk you through everything you need to know to build a simple smart clock.

__TODO finish__ 

## What you are Going to Need

For this tutorial you will need a few electronic components:

- 1 x Arduino Uno Board
- 1 x Raspberry Pi (version 2 or 3)
- 1 x [AlaMode Board](http://wyolum.com/projects/alamode/)
- 1 x 4 Digit 7 Segment Anode Display (5461BS)
- 1 x Shift Register (74HC595)
- 1 x Tiny RTC Module (DS1307)
- 1 x Barometer or Temperature Sensor (BMP180)
- 3 x LEDs (different colours)
- 5 x buttons
- 1 x Piezo Speaker
- 5 x 1Mohm resisters
- 8 x 330ohm resisters
- 3 x 220ohm resisters
- Breadboard + Wires

## Just Your Basic Clock

To get started, we are going to build a alarm clock that is not connected to the Internet. The code developed in this section will act as part of the framework for building an alarm clock connected to the Internet later on.

### Displaying Digits
The first challenge is displaying numbers using a 7 segment, 4 digit display. A 7 segment display is built up of 7 LEDs arranged as shown in the image below. Each of the LEDs is considered a segment, which can be used in conjunction to draw numeric values on the display.

While we call this a 7 segment display, technically there are 8 LED segments that make up the display (with the last one being for a decimal point). The data sheet for this component can be found [here](https://e-radionica.com/productdata/LD3361BS.pdf).

![4-digit_7-segment_display image](/img/DI-0017_2.jpg)

7 Segment displays come in two varieties - common _anode_ and common _cathode_. Both display types contain the word '_common_' in front of them. This simply refers to the fact that for all the LED segments on each digit are linked together at one end. The common _anode_ variant links the LEDs at the ```VCC/VDD``` (+V) line rather than at ```VEE/VSS/GND``` (0V), which means that if you wish to represent a digit, you need to power all of the LED segments, and only ground the ones you want to light up.

The cathode is the opposite to the anode, where the common connection is at the GND line, and a ```HIGH``` value is used to turn on each individual segment. Ultimately, there is no difference in functionality of the two kinds, you just need to invert the logic for your program if the LED segments do the opposite of what you expect! For the remainder of this project we are going to assume when talking about any 7 segment display that it is of the common _anode_ variety.

A 7 segment display works by assigning different lettering to each segment, as shown in the image below. What this means is that if you power the digit and set the pin attached to ```A``` to ```LOW``` then it will turn on.

![7-segment_display lettering image](/img/7-segment display.png)

Different combinations of different segments lighting up make different shapes. The table below shows the segments required for drawing a zero on the display (note the inverted logic!). Have a go filling out the rest of the table for the other 9 digits.

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

#### Build the circuit!

Now that you have a basic understanding of how a 7-segment display works, we are going to have a go building a simple circuit containing one, and program it to display the number zero on all four digits. Build the circuit as displayed in the image below. Once you finish building the circuit, move on to the programming!

![4-digit_7-segment_display circuit image](/img/7segment4digitDisplay_bb.png)

#### Time to for control

First off we need to define which pins are connected to each part of the display. The major advantage of doing so is to allow us to reuse code later on. If we decide later to use a different set of pins to control this display, all we have to do is change one value rather than lots of meaningless integers throughout our code!

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
Now that we have defined all our pins, we need to set them up so the Arduino configures them as output pins:

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

For now, we are going to show four 0's on screen. To do this, we need to put the following code in the main control loop. The code below turns on the pins for all four digits, and then grounds the LED segments for the shape 'zero'.

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

Once you have finished typing up your program, download it to the Arduino and see what happens. Provided that everything goes smoothly, you should see all four digits displaying the number zero!

**Exercise 1**: Write 9 helper functions which display a particular number. In other words, you should have 9 functions structured similarly to the code below. _Hint: Remember that these functions are to draw the number, not to control which digit you are drawing them at! Keep it simple._

```c++
void displayZero(){
    // put code to display zero here.
}
```

**Exercise 2**: Make your display increment the number of all digits by 1 every second. When you reach 9, jump back down to 0 to continue incrementing.


### Shift Registers

You've probably noticed that controlling the display this way uses a lot of pins... In fact, it uses 12 of the 14 digital pins on your Arduino board! Realistically you have two choices if you want to continue adding in sensors to your project:

1. Upgrade your Arduino Uno board to a board with a larger amount of pin mounts, such as the Arduino Mega.
2. Introduce a shift register into the circuit

The choice of which is better is up to you, but in terms of size and cost for this project, a shift register is a better choice. The shift register we will be using looks similar to the one featured in the image below. The data sheet can be found [here](https://cdn.shopify.com/s/files/1/0045/8932/files/EXPAND_ShiftRegisterModule.pdf?100739).

![shift_register image](/img/index.jpeg)

Shift registers are a chip which allows for additional inputs into the circuit. Shift registers have two different functionalities. The first of these is to use serial communication to collect information from sensors. The second functionality is to use parallel communication to allow for multi-pin output control. It is the latter function that we are going to use to drive the display.

#### Build the circuit!

For this part of the tutorial we are going to introduce the shift register into the circuit and attempt to complete the same task as in the previous section, drawing zeros on the display. The circuit to construct should look similar to the one below.

![ShiftRegister7segment4digitDisplay image](/img/ShiftRegister7segment4digitDisplay_bb.png)

#### Time to for control

As you can see from your new circuit, we have moved the connections from A-G and DP from the Arduino and wired them directly into the shift register. As such, the program needs to reflect this. Add the following to your ```#define```s, making sure to remove the letters A-G and DP from the previous program:

```c++
#define firstDigit  2
#define secondDigit 3
#define thirdDigit  4
#define fourthDigit 5
#define dataPin  6
#define latchPin 7
#define clockPin 8
```
Similar to above, modify the setup function, making sure to remove the letters A-G and DP from the previous program.

```c++
pinMode(dataPin, OUTPUT);
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
```

To output data to the shift register, there is a simple one-line command that we need to learn - ```shiftOut(dataPin, clockPin, MSBFIRST, output_value)```. What this does is clock out the output values to the shift register, which in turn enables or disables its outputs accordingly. The ```output_value``` is an 8-bit binary value, which maps to the different segments. For example, the ```output_value``` for drawing zero would look like ```0b00000011```, which represents in a binary format the combination drawn in the table earlier in the tutorial (again, notice the inverted logic to what you'd expect!). By adding the binary value for each number as a constant at the top of your program, you can modify the helper function to look like the following:

```c++
    const int zero  = 0b00000011;

    void displayValue(byte value){
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, value);
        digitalWrite(latchPin, HIGH);
    }
```

Similar to before, download the finished program onto your Arduino, and admire your display showing four zeros for the world to see!

**Exercise 3**: Modify your program from exercise 2 to use binary structures and the function described above.

### 0000 to 9999

Up until now, we have been displaying the same number on all four digits of our display. It is not as easy to display different numbers on each digit. This is because the wire connected to segment A on the first digit is connected to each segment A on the rest (and similarly for all the other segments)! As such, we need to develop a way so that we can write unique numbers to each of the different digits.

To start off, we are going to modify our program to contain the following helper function. The idea of this helper function is so that we can turn a integer value into the functions that we wrote earlier in this tutorial. The code should look something similar to below.

```c++
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
```

With this helper function implemented, we are going to write a function called ```showdigits``` which takes in a number and displays this number on screen. Since it is four digits, the easiest way to gain the individual digits to display is through division. If we divide the input number by 1000, we get the first digit. If we divide the input number by 100 we get the second digit, and so on. Using this idea, remainders when dividing and recursion, we can write the function below to display a four digit number.

```c++
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
```

Now that we have these two functions in place to help us write unique 4 digit numbers, have a go modifying your main control loop so that it displays the number ```1234```.

**Exercise 4**: Using the code above, modify your existing code to write a counter starting at 0000 and counting up to 9999, incrementing the number by 1 each second. For example, your counter should start at 0000, then display 0001, followed by 0002 and so on.

### My Display is Flickering??

It is likely for the exercise above you have written code similar to what is below in your main control loop.

```c++
for(int i = 0; i < 10000; i++){
    showdigits(i);
    delay(1000);
}
```

For those who did, you may notice that your display isn't showing the number correctly. Your display will be flickering briefly and then turning off!. Not to worry - the problem here is the structure of the main loop. In the code below, we have written a code sample that is identical to the one above, however in not using the delay and using ```millis()```, we remove the flickering since the numbers are being constantly refreshed with power at the correct segments!


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

Now is the time (ha ha) you have all been waiting for - putting a real time clock into the circuit. First, we are going to just play with this module alone, then add it into the rest (don't disconnect anything yet though!). A real time clock module looks similar to the sensor shown below. The datasheet for the sensor is available [here](https://datasheets.maximintegrated.com/en/ds/DS1307.pdf).

![tinyRTC image](/img/sku_161172_1.jpg)

Looking at the sensor you will notice that there are two groups of input/output pins, one on the left and the other on the right, which control this sensor. These groups are exactly the same with one minor difference. The larger group has additional pins which can be used for advanced functionality. For this project, we will be using the smaller group.

This is a module that provides time and calendar date through what's called the I2C protocol. There are several libraries available online to interface with this sensor, for example [here](https://github.com/adafruit/RTClib/archive/master.zip) or  [here](https://www.elecrow.com/wiki/index.php?title=File:RTC.zip). For the purposes of learning about the sensor however, we are going to use the I2C pins on our Arduino and get the information manually.

#### Build the circuit!

By itself is very easy to wire into the Arduino. The first circuit we are going to build consequently is the one below (you can leave the rest of your wiring connected as long as it doesn't interfere with this circuit):

![TinyRTC circuit image](/img/tiny-rtc_bb.png)

#### Time to for control

This clock module has two different functionalities we are going to use, setting the time, and reading the time. But before we start writing functions to access these functionalities, we need to add our includes and declarations. To utilise our real time clock functionalities we need to include the ```Wire.h``` library. The [Wire Library](https://www.arduino.cc/en/Reference/Wire) is a standard Arduino library which allows you to communicate with I2C modules. It is more than likely that if you have used an I2C sensor before that you have used it. Because the real time clock uses I2C as its means for passing through data, instead of declaring the pins it is on, we need to tell the program what the address is through a defines method. Finally, we need to declare the variables we are going to use for our program. This section of the program should look similar to what is below.

```c++
#include "Wire.h"

#define RTC_Module 0x68  // the I2C address of Tiny RTC

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
```

Additionally, when using the Wire library, we need to make sure we start it in the setup code  by writing ```Wire.begin();```.

You may have gathered that the data from the real time clock is a stored in a byte from the code above. However, when we need to use the data from the clock, we need to be able to use a number in the base 10 number system (decimal). To do this we are going to do two helper functions that convert between decimal numbers and binary coded decimals (bytes).

```c++
byte decToBcd(byte val){   return ( (val/10*16) + (val%10) );  }
byte bcdToDec(byte val){   return ( (val/16*10) + (val%16) );  }
```

Now that we have all the structure in place, we are going to write a function to set the time of the clock. Setting the clock is very straight forward because of our conversion helper functions we wrote above. All we need to do is write it all the information is expects in the write order over the I2C bus.  

```c++
void setClock(int second, int minute, int hour, int dayOfWeek, int dayOfMonth, int month, int year) {
    Wire.beginTransmission(RTC_Module);
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

If you look closely at your Tiny RTC module, you can see that it has a battery on board. This battery allows the module to remember the time you set for when you power your device again. Provided that the battery is in the sensor when you set the time, you won't need to set it again until the battery runs flat. However, there will be times to use this function you wrote, so where would you use it? The answer to this question is in your initial setup code. To set the time, type the current time into the input parameters for this function when you call it and you are good to go.

Getting the time from the real time clock is similar to the way you set the clock. Rather than writing all the data to the clock through wire, we write a single byte with value 0 to the clock and then request the information by reading the wire. The function to read the time looks like below.

```c++
void getTime(){
     Wire.beginTransmission(RTC_Module);
     Wire.write(decToBcd(0));
     Wire.endTransmission();
     Wire.requestFrom(RTC_Module, 7);
     second     = bcdToDec(Wire.read() & 0x7f);
     minute     = bcdToDec(Wire.read());
     hour       = bcdToDec(Wire.read() & 0x3f);
     dayOfWeek  = bcdToDec(Wire.read());
     dayOfMonth = bcdToDec(Wire.read());
     month      = bcdToDec(Wire.read());
     year       = bcdToDec(Wire.read());
}
```

Up to now, we haven't printed anything to the serial port, but that is about to change. Because we are working on a simple circuit without any display, we are just going to print it to the [Serial](https://www.arduino.cc/en/reference/serial). You may recall using serial from your first ```Hello World``` program on Arduino, but if you don't remember, you need to start your serial connection in your setup code by typing ```Serial.begin(19200);```. Once you have the serial running, we are going to write a function to print the time and date to serial, which looks like the one featured below.

```c++
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
```

Once you have everything together, compile and download the program to your Arduino. With any luck your program should be printing the time and date via the serial console. If it is, congratulations! You are now ready to add the segment display back into the circuit to display the time on this. If not, check your serial console connection on the computer is set to the right speed, and that you have wired your board up correctly.

### Building A Simple Clock

Now that we understand how the clock module works, we are going to modify the circuit to include the display. Instead of printing the time to serial, we are going to display the time on our displays. To begin with you need to build the circuit featured below.

![Simple Clock Circuit Image](/img/simple-clock_bb.png)

**Exercise 5**: Using the circuit above, join your clock code and display code to make the display read what the time is. Your function should look similar to below.

```c++
void displayTime(){
    // write your code here
}
```

### Measuring Temperature

Now that we have a simple clock working, we are going to add an extra sensor in - a barometric sensor. A barometric sensor is essentially an air pressure sensor which also measures temperature and estimated altitude. If you wanted to, you could use a standard sensor that only measures temperature, however this particular sensor is quite nice in that it has a highly accurate digital temperature reading and it is easy to use. The pressure sensor looks similar to below and the data sheet can be found [here](https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf)

![Pressure sensor](/img/bmp180.jpeg)

There are two ways we can choose to program this particular sensor: coding it from scratch using the I2C data, or using the [Adafruit Sensor Library](https://github.com/adafruit/Adafruit_Sensor) to avoid reinventing the wheel. This seems like a perfect time in the tutorial to introduce using external Arduino libraries. Installing external libraries is a very simple process which involves downloading the packaged code from the source, and placing it in a correct folder. If you have never installed a library before, the Arduino Guide has an excellent explanation of how you can install different libraries which you can find [here](https://www.arduino.cc/en/Guide/Libraries).

**Exercise 6**: Using the links in the paragraph above, install the Adafuit Sensor Library. Once you have finished, restart your Arduino IDE.

#### Building our Circuit

Now that you have all the correct libraries installed, it is time to build this sensor into our circuit! The circuit you need to build is featured below.

![](/img/temperatureClock_bb.png)

#### Time for more Control

To include the libraries in what we have done so far, we need to add the following include lines to the base code.

```c++
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
```

Using this library means we can treat the sensor as an encapsulated container of constantly updated values. As a consequence though, we need to add the following global declarations into our base code. These allow us to store and access the sensor data later in the program.

```c++
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); //barometer sensor
float temperature;
```

As the underlying code in the Adafruit library uses the ```Wire.h``` library, similarly we need to start the transmission using the```bmp.begin();``` command. This command needs to go in your setup code.

Like learning the clock, we are first going to print out the temperature to the serial. Actually accessing the sensor values that are measured is super easy. To access the temperature, we use the object and store the value in the temperature variable we declared earlier. We then print it to the serial, as shown in the function below.

```c++
void serialTemperature(){
    bmp.getTemperature(&temperature);
    Serial.print(temperature);
    Serial.println("* C");    
}
```

To make our lives easier because we don't always want to just display the temperature to the Serial, we are going to write a function called ```displayTemperature``` which gets the temperature and displays it to screen for us. The code should look like below.

```c++
void displayTemperature(){
    bmp.getTemperature(&temperature);
    temperature = temperature * 100;
    showDigits((int)temperature);  
}
```

This code does three things. The first task gets the temperature values and stores them in our temperature variable. The second one multiples the value by 100 so that it is a number we could display on screen with the decimal point in the correct location. The last step utilises your code from earlier to write the temperature on screen.

**Exercise 7**: Using the above code, write a new function that displays the temperature using 3 digits and draws a C or F to represent what temperature type you are measuring on the final digit.

**Exercise 8**: Modify your code such that it can deal with negative temperatures being displayed.

**Exercise 9**: Modify your code such that if the button is pressed, the display will show the temperature and turn on the LED. If the button is not pressed, it will display the time and turn off the LED.

### Extra Functionalities
Congratulations, you have now built a simple alarm clock which has the capability of measuring temperature! By increasing your circuit to contain more LEDs or buttons, you can now complete the following exercises. Additionally, you may want to consider using a piezo speaker to emit the alarm sound, which can be wired into your circuit as shown in the image below. Piezo speakers are programmed identically to LEDs, where ```HIGH``` will make the speaker turn on and emit a noise, and ```LOW``` will turn the speaker off.

![](/img/piezoCircuit_bb.png)

**Exercise 10**: Using the buttons in your circuit, write a function to allow you to change and set the time manually.

**Exercise 11**: Introduce an alarm system into your clock, such that you can input the time an alarm should go off, and it will sound an alarm.

**Exercise 12**: Introduce a timer into your clock, such that it can start a timer for a number of minutes, and set an alarm off when the timer ends.

## Making Your Clock Smart

Now is the time to cover why we should bother connecting devices to the Internet. The answer to this is very simple: by connecting our project to the Internet, we have the ability to communicate, interact and collaborate with the world wide web to enable far more functionality! We get the ability to store collected data in data centres to use later. We can combine current sensor data with Internet-based data to get more accurate information about what we are sensing. In short, by utilising the Internet, we can grow our project to interact further with the world.

Unfortunately, the Arduino is a very simple microprocessor, designed to control simple projects which are relatively small in size. It is perfect for projects up to the size of what we have currently done, but it has no network capabilities of its own when using the standard Arduino Uno, and its limited processing capabilities and speed make it less easy to connect to the web securely.This is where more powerful boards like the Raspberry Pi comes into the picture.

The Raspberry Pi is essentially a fully functional computer that is sized similarly to an Arduino board. It has the capabilities of running an operating system which allows you to run mutliple program concurrently, can hook to the Internet with ease and is just as cheap to purchase as an Arduino. The Raspberry Pi does have a limited amount of pins which you can connect sensors to, however it isn't as predictable at turning them on and off reliably. Additionally, it is not very good at running real time systems (like clocks). The Arduino can also handle both digital and analog sensors, in contrast to the Raspberry Pi which can only run digital sensors.  However, together the Arduino and Raspberry Pi can be quite a powerful team.

By combining the capabilities of the Arduino and Raspberry Pi, you can reap the benefits of both systems. You can make your project more complex, and use programs concurrently while still using the simplicity of the Arduino code to control sensors and other modules.

The AlaMode board, pictured below, is essentially an expansion board for the Raspberry Pi which is Arduino compatible.  This is what we'll be using to connect to the Pi in this tutorial, although you could just as easily connect a regular Arduino over USB and use the Pi as a normal computer.

![AlaMode Board](/img/alamode.jpg)

The best thing about this board though is that it has an inbuilt real time clock, which the Raspberry Pi doesn't have, and as you have seen, a normal Arduino doesn't have either. Like all electronics, the Alamode also has a datasheet which you can find [here](http://wyolum.com/docs/Alamode/AlaMode_V1datasheet.pdf).

### Connecting our Simple Clock to the Raspberry Pi

### Checking previous old Arduino Code
As has been alluded above, the AlaMode is essentially an Arduino in different clothing. What this means is that your code from the first part of this tutorial should run perfectly if you move the pins over from your Arduino to the corresponding pins on the AlaMode board. We are going to use this as a perfect opportunity to check that your wiring is the same as the remainder of the tutorials. Please keep in mind that the RTC module and the pressure sensor are not displayed in the pin define section as they are both hooked into the SDA/SCL pins which run the I2C busses. The defined pins that will be used for the remainder of the tutorial are as follows.

```c++
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
```

**Exercise 14:** Move all your wires from your Arduino to the AlaMode board and run your code. Ensure that you have left pin 0 and 1 free for future use, and that your pins match to the allocated pins above.

### Smart alarms
The _smart_ feature we are going to add into our clock is to modify our alarms from exercise 11 to automatically enable based on the first event entered into our calendars from today. The idea is that as a user of an alarm clock, we will never have to set an alarm before we go to bed again. Instead, our clock will look at what we have the next day, and automatically set the alarm for us. We are going to use Google Calendar for this tutorial because it is easily accessible and has a well supported API already in place.

#### Setting Up Google Calendars

A good tutorial for setting up the Google Calendars API already already exists [here](https://developers.google.com/google-apps/calendar/quickstart/python).

**Exercise 15:** Follow the Google Calendar API Python tutorial to set up the APIs, and run the example from step 3 to ensure that it works correctly.

#### Serial Communication
Up to now, you have more than likely used the Serial communication to print things out in the serial monitor, such as the time or temperature earlier. This functionality of printing out data is just scraping the bottom of the barrel for what the Serial library can do. Serial is a protocol which allows us to pass information between different parts of a project. The [Serial](https://www.arduino.cc/en/reference/serial) library allows for communication between multiple Arduinos, an Arduino and other programs, or even the user and the Arduino.

In our project of building a smart alarm clock, the Arduino is going to be receiving data. To begin with, we are going to go on a tangent and build the following small program to experiment with. All this program does is wait for some information to be passed to the serial, and then echoes it back onto the serial communication.

```c++
byte text;

void setup() {                
  Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        text = Serial.read();
        Serial.write(text);
    }
}
```

Once you have uploaded this sketch onto your Arduino, open up the serial communication window and type a message to the Arduino. Once you press send, you should see the same message reflected back in the window. In this example, the Arduino is reading data that we manually type to it. But as said earlier, the Arduino is not just limited to this type of communication. In the upcoming sections, we are going to use a similar idea on the Arduino code. Thus, for the purpose of integrating Google Calendars with Python to our Arduino code, we are going to exclusively look at using Serial to integrate with a python program.

#### Setting up the Arduino Side
This section will walk through a simple alarm set up in your Arduino code, however you are more than welcome to integrate your code from earlier in the tutorial.

To add an alarm into our program, you will need to have two variables which you can access. The first one is to symbolise whether your alarm is set or not, and the second is what time the alarm is set to.
```c++
bool alarmIsSet = false;
int alarmTime = 0;
```

Once we have these two variables set up, the next thing we have to do is to set up our main control loop. If you are working off the base code, you will have one if-else statement related to displaying the time or temperature in your main control loop. This is content that we still need to keep in our program, and what we need to add is to sit directly after it. There are two checks that we need to perform. The first is that if there is anything currently available in the serial communication channel that we need to read (in other words, a new alarm time). If there is, we need to tell the Arduino side of this project that an alarm is set, and then read what the time is and store that value. The second thing we need to check is that if there is an alarm set, does it need to go off. The code to add to the control loop looks similar to below.

```c++
if (Serial.available() > 0) {
    alarmIsSet = true;
    setAlarm(Serial.read());
}

if(alarmIsSet){
    checkAlarm();
}
```

You will notice that we have used two helper functions in the code above. We are also going to step through what the basic versions of this code should look like. In the ```setAlarm(int time)``` function, we want to store the time and show to the user that there is an alarm set. For now, we will assume that the python side of our program will deal with sending a correctly formatted time, so all we have to do is store the value we read from serial and store it for later use. The code, while simple is quite important, and shown below.

```c++
void setAlarm(char* time){
    TODO ABBIE FIX THIS TO BE PROPER I CAN'T REMEMBER HOW TO DO THIS OFF THE TOP OF MY HEAD, SOMETHING LIKE THIS THOUGH
    int hour, minute, second
    snprintf(time,"%d:%d:%d", hour, minute, second);    
    alarmTime = hour * 100 + second;
    digitalWrite(LEDgreen, HIGH);
}
```

The ```checkAlarm()``` function is not as simple as the one to set the alarm, but still achievable. To check the alarm, we first have to get the current time and then store it in a similar fashion to what the alarm value was passed to us like. In other words, we have to multiply the hour value by 100 and add the minutes to get a 4 digit positive integer that represents the time. Once we have this number, we need to check if the alarm time is the same as the current time. If it isn't we do nothing and continue on with the program. If it is, we need to play the alarm sound through the piezo speaker and wait for the user to press a button before resetting the alarm state back to off. The code for this function is featured below.

```c++
void checkAlarm(){
    // get the current time
    getTime();
    int goOffAt = 100*hour + minute;
    if(goOffAt == alarmTime){
        // make an alarm sound
        digitalWrite(piezo, HIGH);
    }

    // if the button is pressed, cancel/turn off the alarm!
    if(digitalRead(button1) == HIGH){
        // reset alarm related variables
        digitalWrite(piezo, LOW);
        digitalWrite(LEDgreen, LOW);
        alarmIsSet = false;        
    }

}
```

With all this code in place, our Arduino side of the program is ready to go. All we have to do now is write the python code to get our serial data. In the mean time, you will notice that by running the program, nothing has changed. The time and temperature are still being displayed.

#### Getting and Sending the Alarm Details in python

__TODO get code off raspberry pi and write this section up__

#### Putting it All Together
Now that you have everything up and going, it is just a matter of running the two programs at the same time. Start off by getting the Arduino side of the project uploaded to the board. Once that is running, then start the python code running in your terminal.

To test that your alarm clock is indeed working, you need to log into your google calendar and make sure there is an event in your calendar for a few minutes time before you start your python code. Provided you do this for the first time (as this will trigger the first alarm), then your clock is up and running perfectly.

The way that we choose the alarms in the python script will not work for everyone, and there are smarter ways to do this. For example, maybe you actually want to wake up 30 minutes before the event starts, or only have an alarm if the name of the event in your calendar is called "Alarm". It is entirely up to your personal preference what works best for you and you are highly encouraged to experiment!

## Final Remarks
Congratulations on building your first smart alarm clock! You now have a device that will wake you up based on your calendar events. Throughout this tutorial you have learned the basics of using electronics, segmented displays, shift registers, real time clock modules, the serial communication protocol and hooking your projects up to the Internet!

But you shouldn't stop here. There are still many different directions you could take your smart clock. So here are some ideas to get you started. Keep in mind that not all of these future challenges are easy, and some will require more learning about new and different sensors. However, that shouldn't stop the fun!

- Changing the segment display to a screen that you can write to. See if you can write the temperature on the same screen. Make the screen change brightness automatically based on either time or other sensors.
- Instead of using the Piezo speaker to simulate the alarm sound, try and send a message back to the python program to play music through a raspberry pi speaker.
- Write an app to control your smart alarm clock from your phone. Maybe try and get the smart alarm clock to be able to play music from your phone!
- Modify the alarm time to incorporate the local traffic for your first calendar event. See if in bad traffic it will wake you up earlier, or if public transport timetables change that it can modify the time to still get you where you want to be on time.
- Get your clock to have different alarms based on the morning you are waking up. If you are going to work, having a day off or even sleeping in, maybe one of those days you would prefer the radio, or temperatures being read out loud instead of music.

Happy Tinkering!
