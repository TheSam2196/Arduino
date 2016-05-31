/*Sonar Pins : 12 - Trigger, 11 - Echo
   RF : 9 - rx , 8 - tx
*/

#include "enes100.h"
#include "dfr_tank.h"
#include <SoftwareSerial.h>
#include "NewPing.h"

NewPing sonar( 13, 12, 1000);
//Declare the software serial which on pins 8,9 which
//are connected to the RF module tx and rx.
SoftwareSerial sSerial(8, 9);

enes100::RfClient<SoftwareSerial> rf(&sSerial);

enes100::Marker marker;

void setup() {
  Serial.begin(9600);
  sSerial.begin(9600);

  delay(500);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  rf.resetServer();
  rf.sendMessage("Team Girlz Connected.\n");
}

void loop() {

  delay(10000);
  if (rf.receiveMarker(&marker, 3)) {
    float x_current = 0;
    float y_current = 0;
    float theta_current = 0;
    rf.sendMessage("Team Material Girlz transmitting data\n");

    x_current = marker.x;
    y_current = marker.y;
    theta_current = marker.theta;
    rf.receiveMarker(&marker, 3);

    if (theta_current < 0 ) {
      rf.sendMessage("Moving Left\n");
      moveThatBitchLeft(0);
    }
    else if (theta_current > 0) {
      rf.sendMessage("Moving Right\n");
      moveThatBitchRight(0);
    }

    rf.receiveMarker(&marker, 3);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Moving Straight\n");
    moveTillTheEnd(.900);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Turn Left\n");
    moveThatBitchLeft(1.57);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Moving Straight\n");
    moveLeft(1.900);
    rf.sendMessage("STOP!\n");

    x_current = marker.x;
    y_current = marker.y;

    rf.sendMessage("Moving Backwards\n");
    analogWrite(5, 100);
    analogWrite(6, 100);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    rf.receiveMarker(&marker,3);
    rf.sendMessage("Storing Data\n");

    float start = 0;
    float ends = 0;
    int counter = 0;
    float bitchNo = 0;
    while (marker.y >= 0.050) {
      if (ends == 0) {
        rf.receiveMarker(&marker, 3);
        bitchNo = sonar.ping_cm();
        rf.sendMessage(bitchNo);
        rf.sendMessage("\n");
        if (sonar.ping_cm() > 20) {
          start = marker.y;
          while (sonar.ping_cm() > 20) {
            bitchNo = sonar.ping_cm();
            rf.sendMessage(bitchNo);
            rf.sendMessage("\n");
            counter++;
            rf.receiveMarker(&marker, 3);
            delay(500);
            if (counter >= 7) {
              ends = marker.y;
              break;
            }
          }
          counter = 0;
        }
      }
      else {
        break;
      }
    }

    rf.receiveMarker(&marker, 3);
    analogWrite(5, 0);
    analogWrite(6, 0);
    rf.sendMessage("STOP\n");
    rf.receiveMarker(&marker, 3);


    rf.sendMessage("Start Postion");
    rf.sendMessage(start);
    rf.sendMessage("\n");
    rf.sendMessage("End Postion");
    rf.sendMessage(ends);
    rf.sendMessage("\n");

    rf.sendMessage("Moving straight to starting point\n");
    moveLeft(start-0.1);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Turning Right\n");
    moveThatBitchRight(0);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Moving Straight\n");
    moveTillTheEnd(1.500);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Turn Right\n");
    moveThatBitchRight(-1.57);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Moving Straight\n");
    moveRight(0.7);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Turn Left\n");
    moveThatBitchLeft(0);
    rf.sendMessage("STOP!\n");
    rf.sendMessage("Moving Straight\n");
    moveTillTheEnd(2.250);
    rf.sendMessage("STOP!\n");

    rf.sendMessage("Performing Mission");
    delay(100000);

    //start performing mission:
    //lower the scoop if needed
    //moveTillTheEnd(2.whatever);
    //rf.sendMessage("Object in Custody\n");
    
    //after this rf doesnt matter except for messaging
    //raise scoop to level needed to flip object onto scale
    //method for finding weight of object
    //rf.sendMessage("Weight: ");
    //rf.sendMessage(weight);
    //rf.sendMessage("\n");
    //if have color sensor, use sensor to find color
    //rf.sendMessage("Color: ");
    //rf.sendMessage(color);
    //rf.sendMessage("\n");
    //rf.sendMessage("End of Mission");
    
  }

}

int moveThatBitchRight(float finals) {
  rf.receiveMarker(&marker, 3);
  while (marker.theta >= finals) {
    analogWrite(5, 255);
    analogWrite(6, 255);
    digitalWrite(4, HIGH);
    digitalWrite(7, LOW);
    rf.receiveMarker(&marker, 3);

  }
  analogWrite(5, 0);
  analogWrite(6, 0);
  return 0;
}

int moveThatBitchLeft(float finals) {
  rf.receiveMarker(&marker, 3);
  while (marker.theta <= finals) {
    analogWrite(5, 255);
    analogWrite(6, 255);
    digitalWrite(4, LOW);
    digitalWrite(7, HIGH);
    rf.receiveMarker(&marker, 3);
  }
  analogWrite(5, 0);
  analogWrite(6, 0);
  return 0;
}

int moveTillTheEnd(float ends) {
  rf.receiveMarker(&marker, 3);
  while (marker.x <= ends) {
      analogWrite(5,255);
      analogWrite(6,255);
      digitalWrite(4,HIGH);
      digitalWrite(7,HIGH);
    rf.receiveMarker(&marker, 3);
  }
  analogWrite(5, 0);
  analogWrite(6, 0);
  return 0;
}

int moveLeft(float ends) {
  rf.receiveMarker(&marker, 3);
  while (marker.y <= ends) {
    analogWrite(5, 255);
    analogWrite(6, 255);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    rf.receiveMarker(&marker,3);
  }
  analogWrite(5, 0);
  analogWrite(6, 0);
  return 0;
}

int moveRight(float start) {
  rf.receiveMarker(&marker,3);
  while (marker.y >= start) {
    analogWrite(5, 255);
    analogWrite(6, 255);
    digitalWrite(4, 255);
    digitalWrite(7, 0);
    rf.receiveMarker(&marker,3);
  }
  analogWrite(5, 0);
  analogWrite(6, 0);
  return 0;
}


//int lowerScoop(float height){
//  hardcode the lowering of scoop to ground based on 
//  how high the scoop is from the ground and how much power 
//  itll take to lower scoop
//  while(height>=0){
//    digitalWrite(2,HIGH);
//  }
//  digitalWrite(2,LOW);
//  rf.sendMessage("Scoop Lowered\n");

//int raiseScoop(float raise){
//  hardcode the raising of scoop to maxheight based on 
//  how high the scoop is needed from the ground and how much power 
//  itll take to raise scoop and based on when the object will flip
//  while(raise<=(max height hardcoded)){
//    digitalWrite(3,HIGH);
//  }
//  digitalWrite(3,LOW);
//  rf.sendMessage("Scoop Raised\n");

//int measureWeight(conditions){
//}

//int findColor(conditions){
//}



