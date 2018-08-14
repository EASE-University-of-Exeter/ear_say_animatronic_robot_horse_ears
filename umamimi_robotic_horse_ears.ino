/*
umamimi_robotic_horse_ears
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


// ######### HORSE CONFIG FILES #####################################

//#include "default.h"
//#include "free.h"
//#include "duke.h"
#include "tashe.h"
//#include "smokie.h"

// ######### HORSE CONFIG FILES #####################################



#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_TiCoServo.h>

// set next true to turn off randomMicroMovements when accelerometer is centred
#define JUST_USER_TRIGGERED_MOVEMENTS false

#define TIME_EAR_STAYS_IN_POS_AFTER_USER_TRIGGER 1000

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


/*

// ##################################

// DEFAULT HORSE!!

// Params for random micro default ear movements (when not specifically triggered for specific movement behaviour)

#define DELAY_AFTER_EACH_LOOP_BEFORE_ACELEROMETER_CHECK 500 // Overall arousal / vigilance / reactiveness / responsiveness level

#define RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED 70 // on a scale of 0-100, which is auto mapped to current servo increment range

#define PERCENTAGE_TIME_EARS_STATIC 90 // range 1-100% - % of time ears are static

#define DELAY_BETWEEN_EACH_STEP_WHEN_EAR_MOVING 10 // Ear travel time (break each movement into steps and specify pause length between steps)

#define PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE 50 // % of ear movements that tend towards upper limits of preset range 


// ##################################

*/


float X, Y, Z;


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

// initialise these with the centred positions, so that they are defined on the first accelerometer checking loop
int previous_Servo_left_position_scaled_to_servo_range = CENTRED_SERVO_LEFT_POSITION;
int previous_Servo_right_position_scaled_to_servo_range = CENTRED_SERVO_RIGHT_POSITION;

#define MIN_RANGE_FOR_RANDOM 0
#define MAX_RANGE_FOR_RANDOM 100

#define ATTENUATION_FACTOR_FOR_MICRO_MOVES_RANGE 0.10

#define INCREMENT_BETWEEN_MOVEMENT_STEPS 20


bool isUserTriggeredMove = false;
bool isUserTriggeredReturnToCentreLEFT = false;
bool isUserTriggeredReturnToCentreRIGHT = false;

void setup() {

  servo_left.attach(SERVO_LEFT_PIN, SERVO_MIN, SERVO_MAX);
  servo_right.attach(SERVO_RIGHT_PIN, SERVO_MIN, SERVO_MAX);
  Serial.begin(9600);
  randomSeed(analogRead(0)); // seed random numbers from unused pin 0 on board
  CircuitPlayground.begin();
} // close setup

void loop() {

//Serial.println("loop");
  

  CircuitPlayground.clearPixels();


/*
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  Serial.print("X: ");
  Serial.print(X);
  Serial.print("  Y: ");
  Serial.print(Y);
  Serial.print("  Z: ");
  Serial.println(Z);
*/

  // Servo control
  int new_Servo_left_position, new_Servo_right_position, new_Servo_left_position_scaled_to_servo_range, new_Servo_right_position_scaled_to_servo_range;

  isUserTriggeredMove = false;


  if (CircuitPlayground.motionY() > THRESHOLD_ROLL_LEFT) 
  {
  Serial.println("ROLL LEFT");
  CircuitPlayground.setPixelColor(7, 0, 0, 255);

  new_Servo_left_position = ROLL_LEFT_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = ROLL_LEFT_SERVO_RIGHT_POSITION;                  // 0 to 1023

  isUserTriggeredMove = true;

  
  } // close ROLL LEFT
  
  else if (CircuitPlayground.motionY() < THRESHOLD_ROLL_RIGHT) 
  {
  Serial.println("ROLL RIGHT");
  CircuitPlayground.setPixelColor(2, 0, 0, 255);

  new_Servo_left_position = ROLL_RIGHT_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = ROLL_RIGHT_SERVO_RIGHT_POSITION;                  // 0 to 1023
  
  isUserTriggeredMove = true;


  } // close ROLL RIGHT
  
  else if (CircuitPlayground.motionX() > THRESHOLD_PITCH_FORWARD) 
  {
  Serial.println("PITCH FORWARD"); 
  CircuitPlayground.setPixelColor(4, 0, 0, 255);
  CircuitPlayground.setPixelColor(5, 0, 0, 255);

  new_Servo_left_position = PITCH_FORWARD_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = PITCH_FORWARD_SERVO_RIGHT_POSITION;                  // 0 to 1023
  
  isUserTriggeredMove = true;


  } // close PITCH FORWARD

  else if (CircuitPlayground.motionX() < THRESHOLD_PITCH_BACK) 
  {
  Serial.println("PITCH BACK");
  CircuitPlayground.setPixelColor(0, 0, 0, 255);
  CircuitPlayground.setPixelColor(9, 0, 0, 255); 

  new_Servo_left_position = PITCH_BACK_SERVO_LEFT_POSITION;                  // 0 to 1023
  new_Servo_right_position = PITCH_BACK_SERVO_RIGHT_POSITION;                  // 0 to 1023

 isUserTriggeredMove = true;

        
  } // close PITCH BACK

  else 
 
  { // accelerometer is centred
    
 // Serial.println("CENTRED"); 

 

 if (JUST_USER_TRIGGERED_MOVEMENTS)

 {

 
 // next lines makes default ear position fixed, with no micro-animations
 new_Servo_left_position = CENTRED_SERVO_LEFT_POSITION;                  // 
 new_Servo_right_position = CENTRED_SERVO_RIGHT_POSITION;                  //

 } // close JUST_USER_TRIGGERED_MOVEMENTS

else // JUST_USER_TRIGGERED_MOVEMENTS == false ... use randomMicroMoves when accelerometer is centred

{

 // Start: more interesting random movements of ears when no trigger events received (accelerometer is centred)
 

/*************************** START: DEFAULT LEFT EAR ****************************************************/


int randomDefaultEarBehaviour_LEFT = random(MIN_RANGE_FOR_RANDOM,MAX_RANGE_FOR_RANDOM);


if (randomDefaultEarBehaviour_LEFT > PERCENTAGE_TIME_EARS_STATIC && randomDefaultEarBehaviour_LEFT < MAX_RANGE_FOR_RANDOM + 1)

{

// This ear: do RandomMicroMove!
  
// do something...

Serial.println("RANDOM MICROMOVE: LEFT EAR");

// next bit is about deciding how often the full range will be available for random moves within the range
// For exampe: if RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED is 10, then values between -10 and +10 should be produced (adjusted to the range of the servos).
// However, PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE may restrict the number of higher values,
// by weighting in favour of lower or higher random values. 
// So, if PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE is defined as 50%, then 50% of the values will be within the -10 and +10 range,
// but the other 50% will be within a range determined by ATTENUATION_FACTOR_FOR_MICRO_MOVES_RANGE (as a range that is a % of RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED.

int fullRangeOrAttenuated = random(MIN_RANGE_FOR_RANDOM,MAX_RANGE_FOR_RANDOM);
int range;

if (fullRangeOrAttenuated > PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE)
{
range = RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED *  ATTENUATION_FACTOR_FOR_MICRO_MOVES_RANGE;  
}
else
{
range = RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED;
}

new_Servo_left_position = 
random(
CENTRED_SERVO_LEFT_POSITION - map(range, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX), 
CENTRED_SERVO_LEFT_POSITION + map(range, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX)
);


} // close RadomMicroMove: left ear


else {

// This ear: do static / stay still / don't move!

//Serial.println("RANDOM MICROMOVE: LEFT EAR: CENTRED_SERVO_LEFT_POSITION");

new_Servo_left_position = CENTRED_SERVO_LEFT_POSITION;                  // 

} // close: left ear stays static

/*************************************** END: DEFAULT LEFT EAR ****************************************************/


/*********************************** START: DEFAULT RIGHT EAR ****************************************************/

int randomDefaultEarBehaviour_RIGHT = random(MIN_RANGE_FOR_RANDOM,MAX_RANGE_FOR_RANDOM);


if (randomDefaultEarBehaviour_RIGHT > PERCENTAGE_TIME_EARS_STATIC && randomDefaultEarBehaviour_RIGHT < MAX_RANGE_FOR_RANDOM + 1)

{

// This ear: do RandomMicroMove!
  
// do something...

Serial.println("RANDOM MICROMOVE: RIGHT EAR");


// next bit is about deciding how often the full range will be available for random moves within the range
// For exampe: if RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED is 10, then values between -10 and +10 should be produced (adjusted to the range of the servos).
// However, PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE may restrict the number of higher values,
// by weighting in favour of lower or higher random values. 
// So, if PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE is defined as 50%, then 50% of the values will be within the -10 and +10 range,
// but the other 50% will be within a range determined by ATTENUATION_FACTOR_FOR_MICRO_MOVES_RANGE (as a range that is a % of RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED.

int fullRangeOrAttenuated = random(MIN_RANGE_FOR_RANDOM,MAX_RANGE_FOR_RANDOM);
int range;

if (fullRangeOrAttenuated > PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE)
{
range = RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED *  ATTENUATION_FACTOR_FOR_MICRO_MOVES_RANGE;  
}
else
{
range = RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED;
}

new_Servo_right_position = 
random(
CENTRED_SERVO_RIGHT_POSITION - map(range, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX), 
CENTRED_SERVO_RIGHT_POSITION + map(range, 0, 100, SERVO_SCALE_MIN, SERVO_SCALE_MAX)

);


} // close RadomMicroMove: right ear


else {

// This ear: do static / stay still / don't move!

//Serial.println("RANDOM MICROMOVE: RIGHT EAR: CENTRED_SERVO_RIGHT_POSITION");

new_Servo_right_position = CENTRED_SERVO_RIGHT_POSITION;                  // 

} // close: right ear stays static

/********************************************************* END: DEFAULT RIGHT EAR ****************************************************/


} // close if randomMovesEnabled loop

} // close: accelerometer is centred





   

// send new positions to the servos / ears
writeNewPositionsToServos(new_Servo_left_position,new_Servo_right_position);

delay(DELAY_AFTER_EACH_LOOP_BEFORE_ACELEROMETER_CHECK);
  
}






// ############################################################################




void writeNewPositionsToServos(int new_Servo_left_position,int new_Servo_right_position) {


// Servo control
int new_Servo_left_position_scaled_to_servo_range, new_Servo_right_position_scaled_to_servo_range;

// Scale new positions to servo range

new_Servo_left_position_scaled_to_servo_range = map(new_Servo_left_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range
new_Servo_right_position_scaled_to_servo_range = map(new_Servo_right_position, SERVO_SCALE_MIN, SERVO_SCALE_MAX, SERVO_MIN, SERVO_MAX);    // Scale to servo range


if (new_Servo_left_position_scaled_to_servo_range != previous_Servo_left_position_scaled_to_servo_range) // check not trying to move servo to same position and ignore if are
{

Serial.print("LEFT: moving to ");
Serial.println(new_Servo_left_position_scaled_to_servo_range); 

if (isUserTriggeredMove)
{

servo_left.write(new_Servo_left_position_scaled_to_servo_range);                               // Move servo

Serial.println("LEFT EAR: user triggered");

isUserTriggeredReturnToCentreLEFT = true;

}

else if (isUserTriggeredReturnToCentreLEFT)
{

servo_left.write(new_Servo_left_position_scaled_to_servo_range);                               // Move servo

Serial.println("LEFT EAR: user triggered return to centre");

isUserTriggeredReturnToCentreLEFT = false;

}


else
{
// move to new position incrementally...
moveLeftEarInStepsTowardsNewServoPosition(previous_Servo_left_position_scaled_to_servo_range, new_Servo_left_position_scaled_to_servo_range);
}

previous_Servo_left_position_scaled_to_servo_range = new_Servo_left_position_scaled_to_servo_range; // Remember the new servo position, as previous

  
} // close: if not the same as previous position

//else { Serial.println("LEFT: already there"); }

if (new_Servo_right_position_scaled_to_servo_range != previous_Servo_right_position_scaled_to_servo_range) // check not trying to move servo to same position and ignore if are
{

Serial.print("RIGHT: moving to ");
Serial.println(new_Servo_right_position_scaled_to_servo_range);

if (isUserTriggeredMove)
{
servo_right.write(new_Servo_right_position_scaled_to_servo_range);                               // Move servo

Serial.println("RIGHT EAR: user triggered");

isUserTriggeredReturnToCentreRIGHT = true;

}


else if (isUserTriggeredReturnToCentreRIGHT)
{
servo_right.write(new_Servo_right_position_scaled_to_servo_range);                               // Move servo

Serial.println("RIGHT EAR: user triggered return to centre");

isUserTriggeredReturnToCentreRIGHT = false;

}

else
{
// move to new position incrementally...
moveRightEarInStepsTowardsNewServoPosition(previous_Servo_right_position_scaled_to_servo_range, new_Servo_right_position_scaled_to_servo_range);
}

previous_Servo_right_position_scaled_to_servo_range = new_Servo_right_position_scaled_to_servo_range;  // Remember the new servo position, as previous


} // close: if not the same as previous position

//else { Serial.println("RIGHT: already there"); }


} // close writeNewPositionsToServos()






void moveLeftEarInStepsTowardsNewServoPosition(int previous_Servo_position, int new_Servo_position) 

{

int tempPos = previous_Servo_position;

if (new_Servo_position > previous_Servo_position)

{

  // ###### GOING UP!!!!! #################

  Serial.print("LEFT EAR travelling UP: ");

  while (tempPos < new_Servo_position - INCREMENT_BETWEEN_MOVEMENT_STEPS)
  {
  tempPos = tempPos + INCREMENT_BETWEEN_MOVEMENT_STEPS;
  Serial.print(tempPos);
  Serial.print(", ");
  servo_left.write(tempPos); 
  delay(DELAY_BETWEEN_EACH_STEP_WHEN_EAR_MOVING);
  }

  servo_left.write(new_Servo_position);

  Serial.println(" ");
  Serial.print("LEFT EAR: Has arrived at: ");

  Serial.println(new_Servo_position);  

}


else // new_Servo_position < previous_Servo_position)

{

 // ###### GOING DOWN!!!!! #################
  
Serial.print("LEFT EAR travelling DOWN: ");
  while (tempPos > new_Servo_position + INCREMENT_BETWEEN_MOVEMENT_STEPS)
  {
  tempPos = tempPos - INCREMENT_BETWEEN_MOVEMENT_STEPS;
  Serial.print(tempPos);
  Serial.print(", ");
  servo_left.write(tempPos); 
  delay(DELAY_BETWEEN_EACH_STEP_WHEN_EAR_MOVING);
  }

  servo_left.write(new_Servo_position);

  Serial.println(" ");
  Serial.print("LEFT EAR: Has arrived at: ");


  
  Serial.println(new_Servo_position);

}

} // close moveLeftEarInStepsTowardsNewServoPosition



void moveRightEarInStepsTowardsNewServoPosition(int previous_Servo_position, int new_Servo_position) 

{

int tempPos = previous_Servo_position;

if (new_Servo_position > previous_Servo_position)

{

 // ###### GOING UP!!!!! #################

Serial.print("RIGHT EAR travelling UP: ");

  while (tempPos < new_Servo_position - INCREMENT_BETWEEN_MOVEMENT_STEPS) 
  {
  tempPos = tempPos + INCREMENT_BETWEEN_MOVEMENT_STEPS;
  Serial.print(tempPos);
  Serial.print(", ");
  servo_right.write(tempPos); 
  delay(DELAY_BETWEEN_EACH_STEP_WHEN_EAR_MOVING);
  }

  servo_right.write(new_Servo_position);

  Serial.println(" ");
  Serial.print("RIGHT EAR: Has arrived at: ");
  Serial.println(new_Servo_position);

}


else // new_Servo_position < previous_Servo_position)

{

 // ###### GOING DOWN!!!!! #################

Serial.print("RIGHT EAR travelling DOWN: ");  

  while (tempPos > new_Servo_position + INCREMENT_BETWEEN_MOVEMENT_STEPS)
  {
  tempPos = tempPos - INCREMENT_BETWEEN_MOVEMENT_STEPS;
  Serial.print(tempPos);
  Serial.print(", ");
  servo_right.write(tempPos); 
  delay(DELAY_BETWEEN_EACH_STEP_WHEN_EAR_MOVING);
  }
  servo_right.write(new_Servo_position);

  Serial.println(" ");
  Serial.print("RIGHT EAR: Has arrived at: ");
  Serial.println(new_Servo_position);

}


} // close moveRightEarInStepsTowardsNewServoPosition




