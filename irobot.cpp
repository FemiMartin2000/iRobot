#include <SoftwareSerial.h>
#include <Servo.h>

// Servo Motors
Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;
Servo servo06;

// Bluetooth Module
SoftwareSerial Bluetooth(3, 4); // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX)

// Servo positions
int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos;
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos;
int speedDelay = 20;
String dataIn = "";

// Line Follower Sensor Pins
#define IR_LEFT A0  // Left IR sensor
#define IR_RIGHT A1 // Right IR sensor

// Motor Driver (L298N) Pins
#define MOTOR_LEFT_FORWARD 10
#define MOTOR_LEFT_BACKWARD 11
#define MOTOR_RIGHT_FORWARD 12
#define MOTOR_RIGHT_BACKWARD 13

void setup() {
  // Attach servos to their respective pins
  servo01.attach(5);
  servo02.attach(6);
  servo03.attach(7);
  servo04.attach(8);
  servo05.attach(9);
  servo06.attach(10);

  // Bluetooth
  Bluetooth.begin(38400);
  Bluetooth.setTimeout(1);
  delay(20);

  // Line follower sensor setup
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  // Motor Driver setup
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Stop motors initially
  stopMotors();

  // Initial positions of the robotic arm
  servo1PPos = 90;
  servo01.write(servo1PPos);
  servo2PPos = 150;
  servo02.write(servo2PPos);
  servo3PPos = 35;
  servo03.write(servo3PPos);
  servo4PPos = 140;
  servo04.write(servo4PPos);
  servo5PPos = 85;
  servo05.write(servo5PPos);
  servo6PPos = 80;
  servo06.write(servo6PPos);
}

void loop() {
  // Check for Bluetooth commands
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();
    processBluetoothCommand(dataIn);
  }

  // Line Follower Logic
  int leftSensor = digitalRead(IR_LEFT);
  int rightSensor = digitalRead(IR_RIGHT);

  if (leftSensor == LOW && rightSensor == LOW) {
    moveForward();  // Both sensors on track
  } else if (leftSensor == LOW && rightSensor == HIGH) {
    turnLeft();  // Adjust left
  } else if (leftSensor == HIGH && rightSensor == LOW) {
    turnRight();  // Adjust right
  } else {
    stopMotors();  // Stop if no line detected
  }
}

// Function to process Bluetooth commands for robotic arm
void processBluetoothCommand(String data) {
  if (data.startsWith("s1")) moveServo(servo01, servo1PPos, data);
  else if (data.startsWith("s2")) moveServo(servo02, servo2PPos, data);
  else if (data.startsWith("s3")) moveServo(servo03, servo3PPos, data);
  else if (data.startsWith("s4")) moveServo(servo04, servo4PPos, data);
  else if (data.startsWith("s5")) moveServo(servo05, servo5PPos, data);
  else if (data.startsWith("s6")) moveServo(servo06, servo6PPos, data);
}

// Function to move servos with smooth movement
void moveServo(Servo &servo, int &prevPos, String data) {
  int newPos = data.substring(2).toInt();
  if (prevPos > newPos) {
    for (int j = prevPos; j >= newPos; j--) {
      servo.write(j);
      delay(20);
    }
  } else {
    for (int j = prevPos; j <= newPos; j++) {
      servo.write(j);
      delay(20);
    }
  }
  prevPos = newPos;
}

// Functions for motor movement
void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}
