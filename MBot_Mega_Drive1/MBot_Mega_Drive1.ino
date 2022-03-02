// Starting point generated by mBlock5 for MegaPi

// This code implements basic driving in Arduino C for the mBot Mega using the Bluetooth Controller.
// The left joystick controls left-right slide and forward-backward, and the right joystick
// controls rotation.
//
// David Staas, Feb. 2022

// MePS2.MeAnalog(2) is the left stick X; 4 is Y; 6 is right stick x; 8 is Y.
// Stick values are -255 to 255.
// Port 1A = front right (motor 1)
// Port 1B = back right (motor 9)
// Port 2A = back left (motor 2)
// Port 2B = front left (motor 10)
// For motor.run(), 255 means clockwise 100% if you're looking at the wheel.

#include <MePS2.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>
#include "src/MeNewRGBLed.h"
#include "src/MeCollisionSensor.h"

MeNewRGBLed left_led(67,4); // Port A13
MeNewRGBLed right_led(68,4); // Port A14
MeMegaPiDCMotor right_front(1);
MeMegaPiDCMotor right_rear(9);
MeMegaPiDCMotor left_rear(2);
MeMegaPiDCMotor left_front(10);
MeCollisionSensor left_bump(65); // Port A11 (left)
MeCollisionSensor right_bump(66); // Port A12 (right)

double currentTime = 0;
double lastTime = 0;

// These are the MePS2.MeAnalog() button assignments for the Bluetooth controller left and right joysticks
// See https://github.com/Makeblock-official/Makeblock-Libraries/blob/master/src/MePS2.h
const int JOYSTICK_LX = 2;
const int JOYSTICK_LY = 4;
const int JOYSTICK_RX = 6;
const int JOYSTICK_RY = 8;
const int L2_BUTTON = 7;

// Universal limit we'll use so we don't run the motors at full speed
// (can cause Bluetooth disconnects without Lithium Ion batteries)
const float LOW_SPEED = 0.50;
const float MID_SPEED = 0.75;
const float HIGH_SPEED = 1.0;
float motor_limit = LOW_SPEED;


float lx_lf, lx_lr, lx_rf, lx_rr; // Left stick X-axis contribution to each wheel
float ly_lf, ly_lr, ly_rf, ly_rr; // Left stick Y-axis contribution to each wheel
float rx_lf, rx_lr, rx_rf, rx_rr; // Right stick X-axis contribution to each wheel
float lf, lr, rf, rr;             // Motor run levels for each wheel (combined contributions)

// Multipliers for rotating the motors in the correct direction
const float NEG = -1.0;
const float POS = 1.0;

MePS2 MePS2(PORT_15);

double getLastTime() {
  return currentTime = millis() / 1000.0 - lastTime;
}

void _delay(float seconds) {
  if(seconds < 0.0) {
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void setup() {
  MePS2.begin(115200);
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  Serial.begin(115200); // Needed only for Arduino IDE Serial Monitor output; you can comment out if not using
  setLeds(255, 0, 0);
}

void _loop() {
  MePS2.loop();
}

// Sets all 4 beads for both LED modules to the given RGB value
void setLeds(int r, int g, int b) {
  left_led.setColor(0, r, g, b);
  right_led.setColor(0, r, g, b);
  left_led.show();
  right_led.show();
}

void checkSpeedButton() {
  // L2 button on controller cycles through speeds (low, mid, high)
  if(MePS2.ButtonPressed(L2_BUTTON)) {
    if (motor_limit == LOW_SPEED) {
      motor_limit = MID_SPEED;
      setLeds(255, 216, 0); // Yellow
    } else if (motor_limit == MID_SPEED) {
      motor_limit = HIGH_SPEED;
      setLeds(0, 255, 0); // Green
    } else {
      motor_limit = LOW_SPEED;
      setLeds(255, 0, 0); // Red
    }
    _delay(0.5);
  }  
}


void checkRearImpact() {
  if (left_bump.isCollision() || right_bump.isCollision()) {
    // Run front wheels forward for 0.4 sec.
    left_front.run(-128);
    right_front.run(128);
    left_rear.run(0);
    right_rear.run(0);
    _delay(.4);
    // All wheels off for another 0.6 sec.
    left_front.run(0);
    right_front.run(0);          
    _delay(.6);
  }
}


void loop() {
  checkSpeedButton();
  checkRearImpact();
  
  // Left/right slide (left joystick X axis)
  lx_lf = NEG * (MePS2.MeAnalog(JOYSTICK_LX));
  lx_lr = POS * (MePS2.MeAnalog(JOYSTICK_LX));
  lx_rf = NEG * (MePS2.MeAnalog(JOYSTICK_LX));
  lx_rr = POS * (MePS2.MeAnalog(JOYSTICK_LX));

  // Forward/backward (left joystick Y axis)
  ly_lf = NEG * (MePS2.MeAnalog(JOYSTICK_LY));
  ly_lr = NEG * (MePS2.MeAnalog(JOYSTICK_LY));
  ly_rf = POS * (MePS2.MeAnalog(JOYSTICK_LY));
  ly_rr = POS * (MePS2.MeAnalog(JOYSTICK_LY));

  // Left/right rotate (right joystick X axis)
  rx_lf = NEG * (MePS2.MeAnalog(JOYSTICK_RX));
  rx_lr = NEG * (MePS2.MeAnalog(JOYSTICK_RX));
  rx_rf = NEG * (MePS2.MeAnalog(JOYSTICK_RX));
  rx_rr = NEG * (MePS2.MeAnalog(JOYSTICK_RX));

  // Combine left-right, up-down, and rotate contributions to each
  // wheel, then apply the motor limit
  lf = (lx_lf + ly_lf + rx_lf) * motor_limit;
  lr = (lx_lr + ly_lr + rx_lr) * motor_limit;
  rf = (lx_rf + ly_rf + rx_rf) * motor_limit;
  rr = (lx_rr + ly_rr + rx_rr) * motor_limit;

  // Apply the resultant power output to the motors
  left_front.run(lf);
  left_rear.run(lr);
  right_front.run(rf);
  right_rear.run(rr);

  _loop();
}
