//////////////////////////////////////////////////////
//      Stephanie De Pol and Juliette Rocco         //
// ECE 106 Final Project: Capacitive Touch Music Box//
//                August 2nd 2020                   //
//////////////////////////////////////////////////////


/*Libraries:
 * - Servo library for the servo motor
 * - Capactive Sensor library for touch activation
 */          
#include <CapacitiveSensor.h>
#include <Servo.h>


/*Capacitance Variables
 * - The capacitive touch sensor is initalized to pins 2 and 4 on the Arduino
 *                - pin 2: sensing/receiving pin
 *                 -pin 4: sending pin
 * - Threshold is set for the amount of change in capacitance needed to activate
 */
CapacitiveSensor capSensor = CapacitiveSensor(4,2);
int threshold = 100;


/*Servo Motor Variables
 * -Without using a physical potentiometer, a potentiometer variable is used to adjust the angle
 * -Direction variable for which way to spin
 *        - 0 indicates forwards
 *        - 1 indicates backwards
 */
Servo myServo;
int potVal,direction = 0;
int angle;


/*Music Variables
 * - Variables for rhythm, and pitch are indicated below
 * - The buzzer is set to pin 8 on the Arduino
 * - The length of the song is 15 and the variable "i" is used to iterate
 */
//rhythm variables
int tempo = 1000;
int w  = tempo; 
int h = w/2;
int q = h/2;
int e = q/2;
int s = e/2;

//pitch variables
int c4 = 261;
int d4 = 293;
int e4 = 330;
int f4 = 350;
int g4 = 392;
int an4 = 440;
int b4 = 494;
int c5 = 523;

int speakerPin = 8;
int length = 15;
int i = 0;


/*Setup
 * - Setup initalizes the servo to pin 12 on the Arduino
 * - the pin for the speaker is set to output 
 * - serial communication is open if needed
 */
void setup() {
  myServo.attach(12);
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);

}

/* Twinkle Twinkle Little Star Song
 * - Each time the twinkle function is called
 *   it will play one note from the song at the pitch and rhythm
 *   specified in the arrays of the function
 * - the variable "i" keeps track of what note it is on
 * - playing one note at a time allows the user to continue the song
 *   where they left off even if they once took their finger off the 
 *   sensor
 */
void twinkle() {

  //arrays that contain the information to play the song
  int lineNotes [] = {c4,c4,g4,an4,an4,g4,f4,f4,e4,e4,d4,d4,c4};
  int lineRhythm [] = {q,q,q,q,q,q,h,q,q,q,q,q,h};

  if (i < 15){
    tone(speakerPin,lineNotes[i],lineRhythm[i]);
    delay(lineRhythm[i]);
    i++;
  }
  
}

/*Loop Function
 * - This function reads the value of capacitance of the sensor
 * - If the capacitance meets the threshold, it will activate the motion one tick
 *   at a time as well as one note
 * - Keeping track of the direction, if the servo motor has reached maximum angles
 *   it will change direction and go the other way for as long as the sensor is pressed
 * - The "if statements" allow for the servo motor and music to continue where they left off
 *   the last time the sensor was touched
 */
void loop() {

//read the value of the sensor and take 30 samples
long sensorValue = capSensor.capacitiveSensor(30);

  if (sensorValue > threshold) {

    //forwards direction
    if (direction == 0){

      //reached maximum angle, change to backwards direction
      if (potVal >= 1020){
        direction = 1;
      //increment the angle by 20 every 200ms
      }else{
        angle = map(potVal, 0, 1023,0,179);
        myServo.write(angle);
        potVal += 20;
        delay(200);
      }
    }
    //backwards direction
    if (direction == 1){

      //reached maximum angle, change to fowards direction
      if (potVal == 0){
        direction = 0;
     //decrement the angle by 20 every 200ms
      }else{
        angle = map(potVal, 0, 1023,0,179);
        myServo.write(angle);
        potVal -= 20;
        delay(200);
      }
    }

    //if the song has reached the 15th note, reset
    if (i == 15){
      i = 0;
    }

    //play one note of the song
    twinkle();
  }
}
