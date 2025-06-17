#define TRIG_PIN 17
#define ECHO_PIN 16

#define SOUND_SPEED 0.034

float prevDistance = 0;
const int motionSensorSensitivity = 3;

bool motionSensorHR_S04(){ // senses motion with a HR_S04

  bool status;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW); 

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.0344 / 2;

  Serial.print("Distance 1: ");
  Serial.print(distance);
  Serial.println(" cm");

  if ( ((distance > 1) && (prevDistance > 1)) && (abs(distance - prevDistance) > motionSensorSensitivity)){
    Serial.println("!!! Movement detected !!!");
    status = true;
  } 

  else {
    Serial.println("No Movement detected!");
    status = false;
  }
  delay(100);

  prevDistance = distance;

  return status;
}


