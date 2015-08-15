const int motoPin1 = 9;
const int motoPin2 = 10;
const int motoPin3 = 11;
const int motoPin4 = 12;

const int echoPin = 4;
const int trigPin = 5;

const bool MOVE_FORWORD = true;
const bool MOVE_BACKWORD = false;

int state = 0;

const int STATE_MOVE_FORWORD = 1;
const int STATE_MOVE_BACKWORD = 2;
const int STATE_STOP = 3;

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
    startMoto(motoPin1, motoPin2, forword);
}

void startRightMoto(bool forword) {
    startMoto(motoPin3, motoPin4, forword);
}

void stopMoto(int pin1, int pin2) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
}

void stopLeftMoto() {
    stopMoto(motoPin1, motoPin2);
}

void stopRightMoto() {
    stopMoto(motoPin3, motoPin4);
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

void setup() {
    pinMode(motoPin1, OUTPUT);
    pinMode(motoPin2, OUTPUT);
    pinMode(motoPin3, OUTPUT);
    pinMode(motoPin4, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
    // startLeftMoto(true);
}

void loop() {
    int distance = getDistance();
    if (distance < 10 && state != STATE_MOVE_FORWORD) {
        startLeftMoto(MOVE_BACKWORD);
        startRightMoto(MOVE_BACKWORD);
        state = STATE_MOVE_FORWORD;
    } else if (distance > 15 && distance < 50 && state != STATE_MOVE_BACKWORD) {
        startLeftMoto(MOVE_FORWORD);
        startRightMoto(MOVE_FORWORD);
        state = STATE_MOVE_BACKWORD;
    } else if (state != STATE_STOP) {
        stopLeftMoto();
        stopRightMoto();
        state = STATE_STOP;
    }
}
