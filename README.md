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

Exploring what it means to be a horse, using expressive ears to communicate both intention and emotion. 

Inspired by the Japanese tradition called ‘Kemonomimi’ (animal eared), which is found in both anime and manga.

This is a project that will reflect on the making of robotic horse ears, living amongst horses using the ears, interspecies communication, becoming 'horse' and even the nature of the quantitative studies that might be conducted to evaluate this artefact.

Umamimi is also inspired by the machines developed for animatronic and puppetry performances. 

Built on a metal framework, with black faux fur covering, two micro servo motors each move an ear independently, in incremental steps, through a maximum range of 180 degrees. An ATmega32u4 processor has been programmed in the Arduino language (a set of C/C++ functions). When the user makes small changes in the inclination of her head, Umamimi’s built-in accelerometer sends these to the software, which responds with programmed (and fully customisable) ear movements: fully forward, fully back, either ear turned outward. When in neutral (meaning that the accelerometer and therefore the device is level), a range of random default ear flicks and movements have been specified.

Horses with different personalities may be configured by selecting a header file (see default.h for a template).

The configurable parameters are as follows:

// Params for random micro default ear movements (when not specifically triggered for specific movement behaviour)

#define PERCENTAGE_TIME_EARS_STATIC 90 // range 1-100% - % of time ears are static

#define DELAY_BETWEEN_EACH_STEP_WHEN_EAR_MOVING 10 // Ear travel time (break each movement into steps and specify pause length between steps)

#define PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE 50 // % of ear movements that tend towards upper limits of preset range 


PSEUDOCODE

Loop () 

	{  // Start loop which checks for accelerometer position

	Get the accelerometer position from the microprocessor
	if (position is outside a given threshold limits for pitch and roll - user's head orientation changes triggers ear movement)
		{
			Do something appropriate with one or more ears: move both front, move both back rotate a single ear to outside
		}

	else  // accelerometer position is level /centred or within threshold limits for pitch and roll
		{ 
		
		if (JUST_USER_TRIGGERED_MOVEMENTS)
			{
				new servo/ear positions = defined centre positions for ears
			}
		else // use random micro movements... this is the random microMove algorithm (the automatic ear moves made when the accelerometer is centred)
			{
				Get a random number between 1 and 100
				if (random number > PERCENTAGE_TIME_EARS_STATIC)
					{
						Get a random number between 1 and 100
						if (random number > PERCENTAGE_EAR_RANDOM_MICRO_MOVES_TENDING_TOWARDS_UPPER_LIMIT_OF_RANGE) // restricts the number of higher values, by weighting in favour of lower random values
							{
								restrict RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED by the defined ATTENUATION_FACTOR_FOR_MICRO_MOVES_RANGE
							}
						else // choose a random ear position that is within the full RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED without 
							{
								Use the full RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED
							}
						new servo/ear position = a random number between 1 and 100 and map this to a value in the (full or restricted) RANGE_FOR_RANDOM_MICRO_ANIMATIONS_RELATIVE_TO_CENTRED
					} // close random number was outside PERCENTAGE_TIME_EARS_STATIC... So, generate a random micromove
			
				else // random number was inside PERCENTAGE_TIME_EARS_STATIC
					{
						new servo/ear positions = defined centre positions for ears
					}
	 
			} // use random micro movements
		
		} // close accelerometer position is level /centred or within threshold limits for pitch and roll
		
	if (the new servo/ear positions are the same as the last positions)
		{
			don't move to the same place! // it's a waste of battery life and the wears the servos
		}
	else // the new servo/ear positions are different from the last positions
		{
		if (the new servo/ear positions are user triggered moves / changes in the accelerometer OR the accelerometer is centred immediately after a user triggered move)
			{
				move servos to the new positions (immediately) // this is to make the device responsive to user input, even if the parameters have configured a laconic horse
			}
		else // the moves are random micro movements
			{
				move servo from the previous position to the new position, incrementally with a delay between each increment determined by DELAY_BETWEEN_EACH_STEP_WHEN_EAR_MOVING
			}
		} // close the new servo/ear positions are different from the last positions
	
	} // close loop which checks for accelerometer position
	



Umamimi - coding setup and config - Arduino IDE and libraries

1. Install Arduino IDE (for Windows, I used: arduino-1.8.5-windows.exe)
2. Install the Adafruit TiCoServo library v1.0.1 in Arduino IDE 1.8.5, do this: Sketch > Include Library > Manage Libraries > search for TiCo and then click Install
Note: Adafruit Circuit Playground v.1.6.8 should be pre-installed in Arduino IDE v.1.8.5
Note: these need to included at the start of a sketch with:
#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_TiCoServo.h>
Note: if needing to test Circuit Playground from Arduino IDE, see File > Examples > Adafruit Circuit Playground
3. To edit Umamimi Robotic Horse ears code, load the sketch umamimi_robotic_horse_ears.ino from the cloned GitHub repository.
4. To connect the Adafruit Circuit Playground in the robotic ears to Arduino IDE (in order to upload the compiled code(, do: Tools > Board > Adafruit Circuit Playground
5. Then do: Tools > Port > <The USB port that the Circuit Playground is connected to. For example: 'COM3 (Adafruit Circuit Playground)'

Umamimi parts list for hardware

1 x AdaFruit Circuit Playground Classic (all-in-one board that has sensors and LEDs)
2 x Tower Pro SG92R Digital Micro Servo
1 x Battery Holder with On/Off Switch for 4 x AA batteries
1 x 2-Pin Male JST-XH Balance Connector with extension wire
1 x Black Micro Suede Trapper Hat
Wire in different colours: 22AWG stranded, outer diameter 1.6mm.
Firm plastic to provide ear shape and rigidity - I upcycled the plastic covers from ringbinder files
1 x metre length of 100% Cotton Velvet - Black - Fabric Velveteen - for ear covering
Velcro tape - to secure inner lining of trapper hat, after opening has been made to insert the device skeleton
Small, elderly, circular metal sticky tape tin (or similar) - to house the Circuit Playground
A roll of metal builders’ band - for umamimi skeleton / head band
Cable ties (with a width narrow enough to go through the holes in the builders’ band) - to fix parts of the skeleton together and to attach the servos, battery holder and Circuit Playground container to the skeleton
Glue gun for ear plastic shaping, sticking black velvet to plastic ears shapers, attaching the servo horn to the ears etc..
Rivet gun - to hold the plastic ear shapers in the stressed ear position (glue on its own didn’t hold!)
