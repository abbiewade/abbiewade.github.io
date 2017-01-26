# Building a Smart Alarm Clock

You can use the [editor on GitHub](https://github.com/abbiewade/abbiewade.github.io/edit/master/smartalarmclock.md) to maintain and preview the content for your website in Markdown files. You can see the webpage [here](https://abbiewade.github.io/smartalarmclock). 

This tutorial makes the assumption that you have played around with the Arduino, Understand the basics of the program structure and have at a minimum used LEDs and buttons. 

## Just Your Basic Clock

To get started, we are going to build a simple clock that is not connected to the internet. The code developed in this section will act as part of the framework for building an alarm clock connected to the internet further down. 

### Displaying Digits
The first part of the challenge of building a clock is to learn how to display numbers using a common anode 7 segment 4 digit display. 

Describe how the 7 segment display works
Show the lettering of the display

To start off we are going to wire this display as you can see in the image below and make each of the four digits display the same number. 

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

To get this ball rolling, we are first going to start by making our display show four 0's on screen. To do this, we need to put the following code in the control loop. 

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
What this code does is ... 

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

Why choosing a shift register is more appropriate in this project

How a shift register works

What the circuit looks like 

... 

Add the following to our defines, making sure to remove the letters A-G and DP from the previous program. 

```c++
#define dataPin 5
#define latchPin 6
#define clockPin 7
```

add to setup, making sure to remove the letters A-G and DP from the previous program. 
```c++
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
```

binary number representations of the digits - where 0 means it will be turned on, and 1 means turned off. 

```c++
    const int zero  = 0b00000011;
    shiftOut(dataPin, clockPin, MSBFIRST, zero);
```

**Exercise 3**: Modify your helper functions above to use the binary structure and shift register to display the incrementing numbers. 

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
**Exercise 3**: Using the code above, modify your existing code to write a counter starting at 0000 and counting up to 9999, incrementing the number by 1 each second. For example, your counter should start at 0000, then display 0001, followed by 0002 and so on. 

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





### Measuring Temperature

### Additional Functionalities
- timer
- alarm
- setting time manually 
- ...
