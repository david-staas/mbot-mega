// Starting point generated by mBlock5 for MegaPi

#include <MePS2.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

MeMegaPiDCMotor right_front(1);
MeMegaPiDCMotor right_rear(9);
MeMegaPiDCMotor left_rear(2);
MeMegaPiDCMotor left_front(10);

// These are the MePS2.MeAnalog() button assignments for the Bluetooth controller left and right joysticks
const int JOYSTICK_LX = 2;
const int JOYSTICK_LY = 4;
const int JOYSTICK_RX = 6;
const int JOYSTICK_RY = 8;

// Universal limit we'll use so we don't run the motors at full speed
// (can cause Bluetooth disconnects without Lithium Ion batteries)
const float MOTOR_LIMIT = 0.50;  // Max. 50%

// Multipliers for rotating the motors in the correct direction
const float NEG = -1.0;
const float POS = 1.0;

MePS2 MePS2(PORT_15);

void _delay(float seconds) {
  if(seconds < 0.0){
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
 Serial.begin(115200);
}

void _loop() {
  MePS2.loop();
}

// MePS2.MeAnalog(2) is the left stick X; 4 is Y
// 6 is right stick x; 8 is Y.
// Stick values are -255 to 255.
// Port 1A = front right (motor 1)
// Port 1B = back right (motor 9)
// Port 2A = back left (motor 2)
// Port 2B = front left (motor 10)

// 255 means clockwise if you're looking at the wheel


void loop() {
//  Serial.println(val);
//  if(MePS2.ButtonPressed(9)){
//
//      right_front.run(0);
//      right_rear.run(0);
//      left_rear.run(0);
//      left_front.run(0);
//      motor_3.run(0);
//      motor_11.run(0);
//      motor_4.run(0);
//      motor_12.run(0);
//
//  }

  // Left-right slide
  float lx_lf = NEG * (MePS2.MeAnalog(JOYSTICK_LX));
  float lx_lr = POS * (MePS2.MeAnalog(JOYSTICK_LX));
  float lx_rf = NEG * (MePS2.MeAnalog(JOYSTICK_LX));
  float lx_rr = POS * (MePS2.MeAnalog(JOYSTICK_LX));
  
//  left_front.run(left_front_val);  // left front
//  left_rear.run(left_rear_val);   // left rear
//  right_front.run(right_front_val);   // right front
//  right_rear.run(right_rear_val);   // right rear
//  Serial.println(String(left_front_val) + " " + String(left_rear_val) + " " + String(right_front_val) + " " + String(right_rear_val));



 // up-down-slide
  float ly_lf = NEG * (MePS2.MeAnalog(JOYSTICK_LY));
  float ly_lr = NEG * (MePS2.MeAnalog(JOYSTICK_LY));
  float ly_rf = POS * (MePS2.MeAnalog(JOYSTICK_LY));
  float ly_rr = POS * (MePS2.MeAnalog(JOYSTICK_LY));

  float lf = (lx_lf + ly_lf) * MOTOR_LIMIT;
  float lr = (lx_lr + ly_lr) * MOTOR_LIMIT;
  float rf = (lx_rf + ly_rf) * MOTOR_LIMIT;
  float rr = (lx_rr + ly_rr) * MOTOR_LIMIT;
  
  left_front.run(lf);  // left front
  left_rear.run(lr);   // left rear
  right_front.run(rf);   // right front
  right_rear.run(rr);   // right rear

  _loop();
}
