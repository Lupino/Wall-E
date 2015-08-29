#include <Servo.h>

const int motoPin1 = 9;
const int motoPin2 = 10;
const int motoPin3 = 11;
const int motoPin4 = 12;

const int echoPin = 4;
const int trigPin = 5;

const int servoPin = 6;

const int leftSide = 75;
const int middleSide = 90;
const int rightSide = 105;
int currentPos = 90;

const bool MOVE_FORWORD = true;
const bool MOVE_BACKWORD = false;

int leftMotoState = 0;
int rightMotoState = 0;

const int STATE_MOVE_FORWORD = 1;
const int STATE_MOVE_BACKWORD = 2;
const int STATE_STOP = 3;

Servo myservo;

void startMoto(int pin1, int pin2, bool forward) {
    if (forward == MOVE_BACKWORD) {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }
}

void startLeftMoto(bool forword) {
    if (leftMotoState != forword) {
        startMoto(motoPin1, motoPin2, forword);
        leftMotoState = forword;
    }
}

void startRightMoto(bool forword) {
    if (rightMotoState != forword) {
        startMoto(motoPin3, motoPin4, forword);
        rightMotoState = forword;
    }
}

void stopMoto(int pin1, int pin2) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
}

void stopLeftMoto() {
    if (leftMotoState != STATE_STOP) {
        stopMoto(motoPin1, motoPin2);
        leftMotoState = STATE_STOP;
    }
}

void stopRightMoto() {
    if (rightMotoState != STATE_STOP) {
        stopMoto(motoPin3, motoPin4);
        rightMotoState = STATE_STOP;
    }
}

int getDistance() {
    digitalWrite(trigPin, LOW);            // 使发出发出超声波信号接口低电平2μs
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);           // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);            // 保持发出超声波信号接口低电平
    int distance = pulseIn(echoPin, HIGH); // 读出脉冲时间
    distance= distance/58;                 // 将脉冲时间转化为距离（单位：厘米）
    return distance;
}

void move(int pos) {
    if (currentPos > pos) {
        for (;currentPos >= pos; currentPos -= 1) {
            myservo.write(currentPos);
            delay(15);
        }
    } else {
        for (;currentPos <= pos; currentPos += 1) {
            myservo.write(currentPos);
            delay(15);
        }
    }
}

void setup() {
    pinMode(motoPin1, OUTPUT);
    pinMode(motoPin2, OUTPUT);
    pinMode(motoPin3, OUTPUT);
    pinMode(motoPin4, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
    myservo.attach(servoPin);
    myservo.write(currentPos);
}

void processLeftSide() {
    move(leftSide);
    int distance = getDistance();
    if (distance < 15) {
        stopLeftMoto();
        startRightMoto(MOVE_BACKWORD);
    } else if (distance > 20 && distance < 40) {
            stopLeftMoto();
            startRightMoto(MOVE_FORWORD);
    }
}

void processMiddleSide() {
    move(middleSide);
    int distance = getDistance();
    if (distance < 15) {
        startLeftMoto(MOVE_BACKWORD);
        startRightMoto(MOVE_BACKWORD);
    } else if (distance > 20 && distance < 40) {
        startLeftMoto(MOVE_FORWORD);
        startRightMoto(MOVE_FORWORD);
    } else {
        stopLeftMoto();
        stopRightMoto();
    }
}

void processRightSide() {
    move(rightSide);
    int distance = getDistance();
    if (distance < 15) {
        startLeftMoto(MOVE_BACKWORD);
        stopRightMoto();
    } else if (distance > 20 && distance < 40) {
        startLeftMoto(MOVE_FORWORD);
        stopRightMoto();
    }
}

void loop() {
    processMiddleSide();
    processRightSide();
    processMiddleSide();
    processLeftSide();
}
