
/*************************************************************
* Project: MovingLight V1.0
* MovingLight is a moveable Neopixel-LED light with two stepper 
* motors and is controlled via bluetooth and Blynk app.
* Author: Andre Nakonz
* Date: 2019/01/20
*
* For this example you need to install some libraries:
* 
* Adafruit nRF51 BLE Library
* Adafruit AccelStepper Library
* Adafruit Motor Shield V2 Library
* Adafruit Neopixel Library
* Blynk Library
*
* CREDITS
* Code snippets are from Adafruit. 
*
**************************************************************/

// BLYNK /////////////////////////////////////////

#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT Serial
#include <BlynkSimpleSerialBLE.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourToken";

// STEPPERMOTORS ///////////////////////////////////

#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(200, 2);
// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  myMotor1->onestep(FORWARD, MICROSTEP);
}
void backwardstep1() {  
  myMotor1->onestep(BACKWARD, MICROSTEP);
}
// wrappers for the second motor!
void forwardstep2() {  
  myMotor2->onestep(FORWARD, MICROSTEP);
}
void backwardstep2() {  
  myMotor2->onestep(BACKWARD, MICROSTEP);
}
// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);


// BLE /////////////////////////////////////////

#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <SPI.h>
// SHARED SPI SETTINGS (see adafruit webpages for details)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              -1    // Optional but recommended, set to -1 if unused
#define BLUEFRUIT_VERBOSE_MODE         true
// Create ble instance, see pinouts above
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// NEOPIXEL /////////////////////////////////

#include <Adafruit_NeoPixel.h>
#define PIXEL_COUNT 24 // Neopixel Ring for Photon has 24 LEDs
#define PIXEL_PIN 11 // Neopixel Ring uses D6
#define PIXEL_TYPE NEO_RGBW + NEO_KHZ800
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, SK6812);
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
int colorPicker = 0;
int colorR = 0;
int colorG = 0;
int colorB = 0;


// Start with mode = 0 
int mode = 0;


void setup() {
  // Stepper Motors. See Adafruit AccelStepper Libraray for details.
  AFMS.begin(); 
  stepper1.setMaxSpeed(100.0);
  stepper1.setAcceleration(100.0);
  stepper1.moveTo(1000);
  
  stepper2.setMaxSpeed(100.0);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(1000);

  // Bluetooth
  ble.begin(BLUEFRUIT_VERBOSE_MODE);
  ble.factoryReset(); // Optional
  ble.setMode(BLUEFRUIT_MODE_DATA);

  // Blynk
  Blynk.begin(auth, ble);
  
  // Neopixel
  strip.begin();
  strip.setBrightness(200); // 40/255 brightness (about 15%)
}

  BLYNK_WRITE(V0){
  mode = 0; // Stepper Stop
}

  BLYNK_WRITE(V1){
  mode = 1; // Stepper vertical and horizontal
}

  BLYNK_WRITE(V2){
  mode = 2; // Stepper vertical
}

  BLYNK_WRITE(V3){
  mode = 3; // Stepper horizontal
}

  BLYNK_WRITE(V10){
  mode = 10; // // Lights OFF
}

  BLYNK_WRITE(V11){
  mode = 11; // // Lights ON
}

  BLYNK_WRITE(V20){
  colorR = param.asInt(); //Color Red
}

  BLYNK_WRITE(V21){
  colorG = param.asInt(); //Color Green
}

  BLYNK_WRITE(V22){
  colorB = param.asInt(); //Color Blue
}

void loop() {

  
Blynk.run();

if(mode == 0){ //StepperStop
    stepper1.speed();
    stepper2.speed();
  
}

else if(mode == 1){ // MovingModeOne
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());
    if (stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());
    stepper1.run();
    stepper2.run();
}

else if(mode == 2){ // MovingModeOne
    if (stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());
    stepper2.run();
}

else if(mode == 3){ // MovingModeOne
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());
    stepper1.run();
}

else if(mode == 10){ //LIGHT OFF
       for (int i = 0; i < strip.numPixels(); i++) {
       strip.setPixelColor(i, 0, 0, 0);
     }
      strip.show();
}

else if(mode == 11){ // Rainbow Solid
    for (int i = 0; i < strip.numPixels(); i++) {
       strip.setPixelColor(i, colorG, colorR, colorB);
     }
     strip.show();
}
}


