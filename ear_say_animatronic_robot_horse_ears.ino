/*
ear_say_animatronic_robot_horse_ears
================
Author:  Steve North
Author URI:  http://socialsciences.exeter.ac.uk/sociology/staff/north/
License: AGPLv3 or later
License URI: http://www.gnu.org/licenses/agpl-3.0.en.html
Can: Commercial Use, Modify, Distribute, Place Warranty
Can't: Sublicence, Hold Liable
Must: Include Copyright, Include License, State Changes, Disclose Source

Copyright (c) 2018, The University of Exeter
*/

#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_TiCoServo.h>


// To use microseconds instead of degrees 

#define SERVO_SCALE_MIN 0 //
#define SERVO_SCALE_MAX 1023 //

#define CENTRED_SERVO_LEFT_POSITION 100   // 0 to 1023
#define CENTRED_SERVO_RIGHT_POSITION 900   // 0 to 1023

#define ROLL_LEFT_SERVO_LEFT_POSITION 510   //
#define ROLL_LEFT_SERVO_RIGHT_POSITION 1023   // 

#define ROLL_RIGHT_SERVO_LEFT_POSITION 0   //
#define ROLL_RIGHT_SERVO_RIGHT_POSITION 510   // 

#define PITCH_FORWARD_SERVO_LEFT_POSITION 0   //
#define PITCH_FORWARD_SERVO_RIGHT_POSITION 1023   // 

#define PITCH_BACK_SERVO_LEFT_POSITION 1023   //
#define PITCH_BACK_SERVO_RIGHT_POSITION 0   //

// To use degrees instead of microseconds
/*
#define SERVO_SCALE_MIN -90 //
#define SERVO_SCALE_MAX 90 //

#define CENTRED_SERVO_LEFT_POSITION -60   // 
#define CENTRED_SERVO_RIGHT_POSITION 60   // 

#define ROLL_LEFT_SERVO_LEFT_POSITION 0   //
#define ROLL_LEFT_SERVO_RIGHT_POSITION 90   // 

#define ROLL_RIGHT_SERVO_LEFT_POSITION -90   //
#define ROLL_RIGHT_SERVO_RIGHT_POSITION 0   // 

#define PITCH_FORWARD_SERVO_LEFT_POSITION -90   //
#define PITCH_FORWARD_SERVO_RIGHT_POSITION 90   // 

#define PITCH_BACK_SERVO_LEFT_POSITION 90   //
#define PITCH_BACK_SERVO_RIGHT_POSITION -90   //
*/

/* ##################################  */

// Params for random micro default ear movements (when not specifically triggered for specific movement behaviour)

#define percentageTimeEarsRadomMicroMove 16 // range 1-100%
#define percentageTimeEarsFlickForward 2 // range 1-100%
#define percentageTimeEarsRadomFlickback 2 // range 1-100%
#define percentageTimeEarsStatic 80 // range 1-100%
// IMPORTANT: above values need to total 100%!!

#define RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED 7 // on a scale of 0-100, which is auto mapped to current servo increment range


/* ##################################  */


float X, Y, Z;

#define DELAY_AFTER_EACH_LOOP_BEFORE_ACELEROMETER_CHECK 500 //


// Trigger limits for accelerometer, to create events initiate specific ear move behaviours
#define THRESHOLD_ROLL_LEFT 3   // threshold for...
#define THRESHOLD_ROLL_RIGHT -3   // threshold for...
#define THRESHOLD_PITCH_FORWARD 3   // threshold for...
#define THRESHOLD_PITCH_BACK -3   // threshold for...


// Servo parameters. On Circuit Playground, only pins 9 and 10 are
// supported by the TiCoServo library.
// Servo position can be specified in degrees or in microseconds; library
// can distinguish between the two. The #defines below are reasonable
// min/max pulse durations (in microseconds) for many servos, but for
// maximum control you'll probably need to do some calibration to find
// the optimal range for your specific servos.
#define SERVO_LEFT_PIN    9
#define SERVO_RIGHT_PIN    10

// Note: for TowerPro SG92R servo: Position "0" (1.5ms pulse) is middle, "90" (~2ms pulse) is all the way to the right, "-90" (~1ms pulse) is all the way to the left.
// 0 = 1ms = -90 = -90 degrees
// 511 = 1.5ms = 0 degrees
// 1023 = 2ms = +90 degrees

// ... but actual calibration for the TowerPro SG92R servo, in order to get closer to the 180 degree range, was 0.9ms - 2.5ms

#define SERVO_MIN 900 // 0.9 ms pulse (started with value set at 1000 and calibrated with TowerPro SG92R servo to get further movement range)
#define SERVO_MAX 2500 // 2.5 ms pulse (started with value set at 2000 and calibrated with TowerPro SG92R servo to get further movement range)

Adafruit_TiCoServo servo_left;
Adafruit_TiCoServo servo_right;

void setup() {
  servo_left.attach(SERVO_LEFT_PIN, SERVO_MIN, SERVO_MAX);
  servo_right.attach(SERVO_RIGHT_PIN, SERVO_MIN, SERVO_MAX);
  Serial.begin(9600);
  randomSeed(analogRead(0)); // seed random numbers from unused pin 0 on board
  CircuitPlayground.begin();
}

void loop() {
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  CircuitPlayground.clearPixels();


/*
  Serial.print("X: ");
  Serial.print(X);
  Serial.print("  Y: ");
  Serial.print(Y);
  Serial.print("  Z: ");
  Serial.println(Z);
*/

  // Servo control
  int new_Servo_left_position, new_Servo_right_position, new_Servo_left_position_scaled_to_servo_range, new_Servo_right_position_scaled_to_servo_range;


  if (CircuitPlayground.motionY() > THRESHOLD_ROLL_LEFT) 
  {
  Serial.println("ROLL LEFT");
  CircuitPlayground.setPixelColor(7, 0, 0, 255);

  new_Servo_left_position = ROLL_LEFT_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = ROLL_LEFT_SERVO_RIGHT_POSITION;                  // 0 to 1023
  new_Servo_left_position_scaled_to_servo_range = map(new_Servo_left_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range
  new_Servo_right_position_scaled_to_servo_range = map(new_Servo_right_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range

  
servo_left.write(new_Servo_left_position_scaled_to_servo_range);                               // Move servo
servo_right.write(new_Servo_right_position_scaled_to_servo_range);                               // Move servo
  
  } 
  
  else if (CircuitPlayground.motionY() < THRESHOLD_ROLL_RIGHT) 
  {
  Serial.println("ROLL RIGHT");
  CircuitPlayground.setPixelColor(2, 0, 0, 255);

  new_Servo_left_position = ROLL_RIGHT_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = ROLL_RIGHT_SERVO_RIGHT_POSITION;                  // 0 to 1023
  new_Servo_left_position_scaled_to_servo_range = map(new_Servo_left_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range
  new_Servo_right_position_scaled_to_servo_range = map(new_Servo_right_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range

  
servo_left.write(new_Servo_left_position_scaled_to_servo_range);                               // Move servo
servo_right.write(new_Servo_right_position_scaled_to_servo_range);                               // Move servo

  }
  
  else if (CircuitPlayground.motionX() > THRESHOLD_PITCH_FORWARD) 
  {
  Serial.println("PITCH FORWARD"); 
  CircuitPlayground.setPixelColor(4, 0, 0, 255);
  CircuitPlayground.setPixelColor(5, 0, 0, 255);

  new_Servo_left_position = PITCH_FORWARD_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = PITCH_FORWARD_SERVO_RIGHT_POSITION;                  // 0 to 1023
  new_Servo_left_position_scaled_to_servo_range = map(new_Servo_left_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range
  new_Servo_right_position_scaled_to_servo_range = map(new_Servo_right_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range

  
servo_left.write(new_Servo_left_position_scaled_to_servo_range);                               // Move servo
servo_right.write(new_Servo_right_position_scaled_to_servo_range);                               // Move servo

  }

  else if (CircuitPlayground.motionX() < THRESHOLD_PITCH_BACK) 
  {
  Serial.println("PITCH BACK");
  CircuitPlayground.setPixelColor(0, 0, 0, 255);
  CircuitPlayground.setPixelColor(9, 0, 0, 255); 

  new_Servo_left_position = PITCH_BACK_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = PITCH_BACK_SERVO_RIGHT_POSITION;                  // 0 to 1023
  new_Servo_left_position_scaled_to_servo_range = map(new_Servo_left_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range
  new_Servo_right_position_scaled_to_servo_range = map(new_Servo_right_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range

  
servo_left.write(new_Servo_left_position_scaled_to_servo_range);                               // Move servo
servo_right.write(new_Servo_right_position_scaled_to_servo_range);                               // Move servo

        
  }

  else 
  {
  
  
  Serial.println("CENTRED"); 

 
 // next lines makes default ear position fixed, with no micro-animations
 // new_Servo_left_position = CENTRED_SERVO_LEFT_POSITION;                  // 
 // new_Servo_right_position = CENTRED_SERVO_RIGHT_POSITION;                  //


 // Start: more interesting random movements of ears when no trigger events received (accelerometer is centred)

/*************************** START: DEFAULT LEFT EAR ****************************************************/

int randomDefaultEarBehaviour_LEFT = random(1,100);

if (randomDefaultEarBehaviour_LEFT > 0 && randomDefaultEarBehaviour_LEFT < percentageTimeEarsRadomMicroMove + 1)
{
// This ear: do RadomMicroMove!
  
// do something...

new_Servo_left_position = 
random(
CENTRED_SERVO_LEFT_POSITION - map(RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX), 
CENTRED_SERVO_LEFT_POSITION + map(RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX)
);

}

else if (randomDefaultEarBehaviour_LEFT > percentageTimeEarsRadomMicroMove && randomDefaultEarBehaviour_LEFT 
< (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward) + 1)
{
// This ear: do FlickForward!
// do something...

new_Servo_left_position = PITCH_FORWARD_SERVO_LEFT_POSITION;

}

else if (randomDefaultEarBehaviour_LEFT > (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward) && randomDefaultEarBehaviour_LEFT 
< (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward + percentageTimeEarsRadomFlickback) + 1)
{
// This ear: do Flickback!
// do something...

new_Servo_left_position = PITCH_BACK_SERVO_LEFT_POSITION;

}

else 
{
// if randomDefaultEarBehaviour_LEFT > (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward + percentageTimeEarsRadomFlickback) and = percentageTimeEarsStatic
// This ear: do static / stay still / don't move!

// do something...

new_Servo_left_position = CENTRED_SERVO_LEFT_POSITION;                  // 

}

/*************************************** END: DEFAULT LEFT EAR ****************************************************/





/*********************************** START: DEFAULT RIGHT EAR ****************************************************/

int randomDefaultEarBehaviour_RIGHT = random(1,100);

if (randomDefaultEarBehaviour_RIGHT > 0 && randomDefaultEarBehaviour_RIGHT < percentageTimeEarsRadomMicroMove + 1)
{
// This ear: do RadomMicroMove!
  
// do something...

new_Servo_right_position = 
random(
CENTRED_SERVO_RIGHT_POSITION - map(RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX), 
CENTRED_SERVO_RIGHT_POSITION + map(RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX)
);

}

else if (randomDefaultEarBehaviour_RIGHT > percentageTimeEarsRadomMicroMove && randomDefaultEarBehaviour_RIGHT 
< (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward) + 1)
{
// This ear: do FlickForward!
// do something...

new_Servo_right_position = PITCH_FORWARD_SERVO_RIGHT_POSITION;

}

else if (randomDefaultEarBehaviour_RIGHT > (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward) && randomDefaultEarBehaviour_RIGHT 
< (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward + percentageTimeEarsRadomFlickback) + 1)
{
// This ear: do Flickback!
// do something...

new_Servo_right_position = PITCH_BACK_SERVO_RIGHT_POSITION;

}

else 
{
// if randomDefaultEarBehaviour_RIGHT > (percentageTimeEarsRadomMicroMove + percentageTimeEarsFlickForward + percentageTimeEarsRadomFlickback) and = percentageTimeEarsStatic
// This ear: do static / stay still / don't move!

// do something...

new_Servo_right_position = CENTRED_SERVO_RIGHT_POSITION;                  // 

}

/********************************************************* END: DEFAULT RIGHT EAR ****************************************************/



// Scale new positions to servo range
  
new_Servo_left_position_scaled_to_servo_range = map(new_Servo_left_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range
new_Servo_right_position_scaled_to_servo_range = map(new_Servo_right_position, SERVO_SCALE_MIN, 1023, SERVO_SCALE_MAX, SERVO_MAX);    // Scale to servo range



// Move the servos / ears !!
  
servo_left.write(new_Servo_left_position_scaled_to_servo_range);                               // Move servo
servo_right.write(new_Servo_right_position_scaled_to_servo_range);                               // Move servo

  
  }
   

  delay(DELAY_AFTER_EACH_LOOP_BEFORE_ACELEROMETER_CHECK);

  
}
