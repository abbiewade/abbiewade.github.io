# Building a Smart Alarm Clock

You can use the [editor on GitHub](https://github.com/abbiewade/abbiewade.github.io/edit/master/smartalarmclock.md) to maintain and preview the content for your website in Markdown files. You can see the webpage [here](https://abbiewade.github.io/smartalarmclock). 

This tutorial makes the assumption that you have played around with the Arduino, Understand the basics of the program structure and have at a minimum used LEDs and buttons. 

TODO: Add links to data sheets, better explain the pins on each sensor, ... 

## Just Your Basic Clock

To get started, we are going to build a alarm clock that is not connected to the internet. The code developed in this section will act as part of the framework for building an alarm clock connected to the internet further down. 

### Displaying Digits
The first part of the challenge of building a clock is to learn how to display numbers using a common anode 7 segment 4 digit display. The 7 segment display is built up of 7 LEDs arranged as shown in the image below. Each of the LEDs is considered a segment, which can be used in conjunction to draw numeric values on the display. While we call this a 7 segment display, technically there are 8 LED segments that make up the display, with the last one being for a decimal point. 

![](/img/DI-0017_2.jpg)

7 Segment displays come in two varieties - common anode and common cathode. Both display types contain the word _common_ in front of them. This simply refers to the fact that for all the LED segments on a digit are linked by at one end. The anode display links the LEDs at the VCC line, which means that if you wish to turn an LED on, you need to change it to a ```LOW``` value. The cathode display is the opposite to the anode, where the common end of the LEDs is connected to the GND line, and the ```HIGH``` value is used to turn a LED segment on. Ultimately, there is no difference in functionality of the two kinds, you just need to invert the logic for your program if the LED segments do the opposite of what you expect. As such, for the remainder of this project we are going to assume when talking about any 7 segment display that it is of the common anode variety. 

The 7 segment display works by assigning different lettering to each segment, as shown in the image below. What this means is that if you set the pin attached to ```A``` to ```LOW``` then it will turn on. 

![](/img/7-segment display.png)

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

![](/img/7segment4digitDisplay_bb.png)

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

The choice of which is better is up to you, but in terms of size and cost, for this project a shift register is a better choice. 

Shift registers are a chip which allows for additional inputs into the circuit. Shift registers have two different functionalities. The first of these functionalities is to use serial communcication to collect information from sensors. The second functionality is to use parallel communication to allow for multi-pin output control. It is the later that we are going to experiment with below. 

In this part of the tutorial we are going to introduce the shift register into the circuit and attempt to complete the same task as in the previous section, drawing zeros on the display. The circuit to construct should look similar to the one below. 

![](/img/ShiftRegister7segment4digitDisplay_bb.png)

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

Up until now, we have been displaying the same number on all four digits of our display. It is not as trivial to display different numbers on each digit. 

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
  }
}
```

```c++
// showing 4 digits
void showdigits (int number) {
  // e.g. we have "1234"
  showdigit(number / 1000); // segments are set to display "1"
  digitalWrite(firstDigit, HIGH); // first digit on,
  digitalWrite(secondDigit, LOW); // other off
  digitalWrite(thirdDigit, LOW);
  digitalWrite(fourthDigit, LOW);

  delay (1);

  number = number % 1000; // remainder of 1234/1000 is 234
  digitalWrite(firstDigit, LOW); // first digit is off
  showdigit(number / 100); //// segments are set to display "2"
  digitalWrite(secondDigit, HIGH); // second digit is on
  delay (1); // and so on....

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
**Exercise 4**: Using the code above, modify your existing code to write a counter starting at 0000 and counting up to 9999, incrementing the number by 1 each second. For example, your counter should start at 0000, then display 0001, followed by 0002 and so on. 

flickering -- too fast 

```c++
  for (int i = 0; i < 10000; i++) {
    long currenttime = millis();
    while (currenttime + 100 > millis()) {
      showdigits(i);
    }
  }
```

... 

### Real Time Clock Module

![](/img/tiny-rtc_bb.png)

![](/img/simple-clock_bb.png)


- setting time manually 

### Measuring Temperature


### Additional Functionalities
Congratulations, you have now built a simple alarm clock which has the capabilites of measuring temperature. By increasing your circuit to contain more LEDs or buttons, you can now complete the following exercises. Enjoy. 


**Exercise X**: Introduce an alarm system into your clock, such that you can input the time an alarm should go off, and it will sound an alarm. 

**Exercise X**: Introduce a timer into your clock, such that it can start a timer for a number of minutes, and set an alarm off when the timer ends. 


## Making Your Clock Smart
TODO

## Final Remarks
TODO
