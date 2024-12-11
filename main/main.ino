#include <Servo.h>
#include "OLED_Driver.h"
#include "GUI_paint.h"
#include "DEV_Config.h"
#include "Debug.h"
#include "ImageData.h"  // Görsel verilerin bulunduğu kütüphane

// Button's pin
const int buttonPin = 12;
int buttonState = 1;
int situation = 0;

// Joysticks' pins
const int yPinOne = A0;
const int xPinOne = A1;
const int xPinTwo = A3;
const int yPinTwo = A2;

// Joysticks' values
int xValueOne;
int yValueOne;  
int xValueTwo; 
int yValueTwo;

// Servos' pins and angles
Servo servoOne;
const int servoOnePin = 9;
int servoOneAngle;

Servo servoTwo;
const int servoTwoPin = 6;
int servoTwoAngle;

Servo servoThree;
const int servoThreePin = 5;
int servoThreeAngle;

Servo servoFour;
const int servoFourPin = 4;
int servoFourAngle;

// Buzzer pin 
const int buzzerPin = A5; 

// OLED image buffer
UBYTE *BlackImage;
UWORD Imagesize = ((OLED_1in5_WIDTH % 2 == 0) ? (OLED_1in5_WIDTH / 2) : (OLED_1in5_WIDTH / 2 + 1)) * OLED_1in5_HEIGHT;

void setup() {
  // Setup for OLED
  System_Init();
  OLED_1in5_Init();
  OLED_1in5_Clear();

  if ((BlackImage = (UBYTE *)malloc(Imagesize / 8)) == NULL) {
    Serial.print("Failed to apply for black memory...\r\n");
    return;
  }
  Paint_NewImage(BlackImage, OLED_1in5_WIDTH / 4, OLED_1in5_HEIGHT / 2, 270, BLACK);
  Paint_SetScale(16);
  Paint_SelectImage(BlackImage);
  Paint_Clear(BLACK);

  // Setup for servos and other pins
  pinMode(buttonPin, INPUT_PULLUP);  // Button pin setup
  pinMode(buzzerPin, OUTPUT);        // Buzzer pin setup

  // Servo motor pin setup
  servoOne.attach(servoOnePin);
  servoTwo.attach(servoTwoPin);
  servoThree.attach(servoThreePin);
  servoFour.attach(servoFourPin);

  Serial.begin(9600);  // Start serial communication
}

void loop() {
  buttonState = digitalRead(buttonPin);  // Read button state

  if (buttonState == LOW) {
    delay(50); // Debounce delay to avoid false triggering
    while (digitalRead(buttonPin) == LOW); // Wait until button is released

    if (situation == 0) {
      tone(buzzerPin, 1000, 200); // Start sound
      delay(200);
      situation = 1; // Set situation to 1 to enable control
    } else {
      tone(buzzerPin, 500, 300); // Stop sound
      delay(300);
      situation = 0; // Set situation to 0 to disable control
    }
  }

  // Control only if situation is 1
  if (situation == 1) {
    // Display the image when robot is working
    displayImage();

    // Read angles from joysticks
    xValueOne = analogRead(xPinOne);
    xValueTwo = analogRead(xPinTwo);
    yValueOne = analogRead(yPinOne);
    yValueTwo = analogRead(yPinTwo);

    // Control servos based on joystick inputs
    controlRobot();

    Serial.println("Robot is working");
  } else {
    // Clear the display when robot is off
    clearDisplay();
    Serial.println("Robot is off");
  }
}

// Function to control servos based on joystick input
void controlRobot() {
  // Mapping joystick values to servo angles
  servoOneAngle = map(yValueTwo, 0, 1023, 0, 180);
  servoTwoAngle = map(xValueOne, 0, 1023, 0, 180);
  servoThreeAngle = map(xValueTwo, 0, 1023, 0, 180);
  servoFourAngle = map(yValueOne, 0, 1023, 0, 180);

  // Writing the mapped angles to the servos
  servoOne.write(servoOneAngle);
  servoTwo.write(servoTwoAngle);
  servoThree.write(servoThreeAngle);
  servoFour.write(servoFourAngle);
}

// Function to display the image when robot is working
void displayImage() {
  OLED_1in5_Display(gImage_1in5); // Display the image from ImageData.h
}

// Function to clear the display when robot is off
void clearDisplay() {
  OLED_1in5_Clear(); // Clear the OLED display
}