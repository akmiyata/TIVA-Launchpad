This folder contains files related to the solution for Lab 5 in the University of Texas' "Embedded Systems - Shape The World" course. 

Description: Lab 5 is intended to simulate a traffic light intersection, with two sets of traffic lights (for East-West, and North-South directions) as well as a
crosswalk light indicator. 

Requirements (see https://www.youtube.com/watch?v=qNl7vayZEz0&list=PLyg2vmIzGxXEle4_R2VA_J5uwTWktdWTu&index=15 for an illustration of intended operation):
 1. Button 1- West car sensor; if pressed, west traffic light turns from red to green (after south lights have cycled to Red).
 2. Button 2- South car sensor; if pressed, south traffic light turns from red to green (after south lights have cycled to Red).
 3. Button 3- Walk button; if pressed, both west and south traffic lights must cycle to red, and the LED on the PCB will turn from red to green.
 4. Lights will stay in state from "3." until a car is sensed at one of the traffic lights (i.e. button 1 or button 2 is pressed). Once a button is pressed, 
 the LED on the PCB will blink 3 times, then turn to steady red.

Hardware needed: 
  3 push-button switches
  2 each of green, yellow, and red LEDS
  17 wires, male-to-male or male-to-female (see detail below)
  Breadboard
  Six 220 ohm resistors (for LEDS)
  3 10K ohm resistors (for switch inputs)

Pin assignments:
  Red wire- +3.3V power (also four additional red jumper wires to connect breadboard rails)
  Black wire- GND (also up to two additional black jumper wires to connect breadboard rails)
  
  PB0 (green wire)- (powers green light for west street)
  PB1 (yellow wire1)- (powers yellow light for west street)
  PB2 (white wire1)- (powers red light for west street)
  
  PB3 (green wire)- powers green light for south street
  PB4 (yellow wire1)- powers yellow light for south street
  PB5 (white wire1)- powers red light for south street
  
  PE0 (blue wire)- Input button for west street car sensor 
  PE1 (blue wire)- Input button for south street car sensor
  PE2 (blue wire)- Input button for crosswalk
