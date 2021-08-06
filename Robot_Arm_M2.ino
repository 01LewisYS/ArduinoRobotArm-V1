/*Note:
  Q: Which one is "Left Servo"? Which one is "Right Servo"?
  Ans: Look at your computer screen. Now, position the robot arm so that the robot arm's clamp/gripper also faces the computer screen.
       Now look at your robot arm, the leftmost servo is the Left Servo, and the rightmost servo is the Right Servo.
  
  Left servo (servoL) controls the height of clamp from ground. 
  As n in servo.write(n) increases, height from ground increases.
  
  Right servo (servoR) controls the horizontal retraction and extension of arm.
  As n in servoR.write(n) increases, the robot arm extends out more. 

  Base servo (servoB) is the servo that's between the left & right servos (other than the clamp's servo).
  Base servo controls the rotation of the entire robot arm, so that the gripper/clamp can face any direction.
  For servoB.write(n),
  n=90: clamp faces towards computer screen.
  0<n<90: clamp faces more towards the right, where n=0 is completely right.
  90<n<180: clamp faces more towards the left, where n=180 is completely left. 

  Clamp servo (servoC) controls the opening and closing of clamp. 
  Increasing n of servoC.write(n) closes clamp more, while decreasing n opens the clamp more.

  IMPORTANT NOTICE (!):
  This program will only work properly if:
  1) All your servos are calibrated as such during the construction of your Robot Arm:
  servoB.write(90);
  servoL.write(90);
  servoR.write(90);
  servoC.write(25);

  2) You built your robot arm using this instruction manual guide:
  https://static.rapidonline.com/pdf/75-0040_v1.pdf
*/

#include <Servo.h>

#include <NewPing.h>
NewPing sonar(7, 8, 20); //NewPing sonar(trig, echo, max distance to be read in cm (max=4m))

Servo servoB; //Base servo
Servo servoR; //Right servo
Servo servoL; //Left servo
Servo servoC; //Clamp servo
int angleB; //Base servo's angle
int angleR; //Right servo's angle
int angleL; //Left servo's angle
int angleC; //Clamp servo's angle
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoB.attach(3);
  servoR.attach(6);
  servoL.attach(5);
  servoC.attach(9);
  delay(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Distance: ");
  distance = sonar.ping_cm();
  Serial.println(distance);
  
  //PICK UP LOAD
  if(distance==13){ //If human hand is 13cm away from Ultrasonic sensor, activate robot arm to pick up load.
    pickUpRotation();
    openClamp();
    extendsOutP();
    closeClamp();
    extendsInP();
    restRotationP();
  }
  
  //DROP OFF LOAD
  if(distance==15){ //If human hand is 15cm away from Ultrasonic sensor, activate robot arm to drop off load at designated location.
    dropOffRotation();
    extendsOutD();
    openClamp();
    extendsInD();
    restRotationD();
    closeClamp();
  }

  delay(50);
}

//CLAMP FUNCTIONS ==============!!
//To close the Robot Arm's clamp
void closeClamp(){
  for (angleC=servoC.read(); angleC<110; angleC++){
    servoC.write(angleC);
    delay(15);
  }
}

//To open the Robot Arm's clamp
void openClamp(){
  for (angleC=servoC.read(); angleC>30; angleC--){ 
    servoC.write(angleC);
    delay(10);
  }
}

//ARM EXTENDING OUT FUNCTIONS ==============!!
//Robot arm extends towards load for (P)ick up.
void extendsOutP(){ 
  for (angleL=servoL.read(); angleL<140; angleL++){
    servoL.write(angleL);
    delay(15);
  }
  for (angleR=servoR.read(); angleR<160; angleR++){
    servoR.write(angleR);
    delay(15);
  }
}

/*Robot arm extends towards drop off point to (D)rop off of Load.
Arm extends outwards in 1 single fast burst movement instead of slow movements because load is extremely heavy, 
hence all power needs to be packed in this 1 quick motion.
*/
void extendsOutD(){ 
  servoL.write(180);
  servoR.write(150);
}

//ARM RETRACTING BACK IN FUNCTIONS ==============!!
//Robot arm retracts back in after (P)icking up the Load at designated location.
void extendsInP(){ 
  for (angleR=servoR.read(); angleR>80; angleR--){
    servoR.write(angleR);
    servoL.write(angleR);
    delay(5);
  }
  servoR.write(60);
  servoL.write(160);
}

void extendsInD(){ 
  //Robot Arm retracts back in after (D)roping off the Load at designated location.
  for (angleR=servoR.read(); angleR>70; angleR--){
    servoR.write(angleR);
    delay(15);
  }
  for (angleL=servoL.read(); angleL>90; angleL--){
    servoL.write(angleL);
    delay(15);
  }
}

//ROTATION OF BASE SERVO TO PICK UP LOAD ==============!!
//Robot Arm rotates to pick up load that's on the right.
void pickUpRotation(){
  for (angleB; angleB>0; angleB--){ 
    servoB.write(angleB);
    delay(15);
  }
}

//Robot Arm rotates to the left for load drop-off at a designated location.
void dropOffRotation(){
  for (angleB; angleB<180; angleB++){ 
    servoB.write(angleB);
    delay(15);
  }
}

//ROTATION OF BASE SERVO BACK TO RESTING POSITION AFTER LOAD-PICKUP ==============!!
//Robot Arm rotates back to resting position (robot clamp facing forward towards computer screen) after (P)icking up load.
void restRotationP(){
  for (angleB; angleB<90; angleB++){
    servoB.write(angleB);
    delay(15);
  }
}

//Robot Arm rotates back to resting position (robot clamp facing forward towards computer screen) after (D)ropping off load.
void restRotationD(){
  for (angleB; angleB>90; angleB--){ 
    servoB.write(angleB);
    delay(15);
  }
}