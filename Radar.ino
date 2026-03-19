#include <Servo.h>
#include <FastLED.h>

Servo radarServo;
const int servoPin = 5;

#define TRIG_RIGHT 3
#define ECHO_RIGHT 4
#define TRIG_LEFT  10
#define ECHO_LEFT  11
#define MAX_DISTANCE 40

long dRight = 999;
long dLeft  = 999;

#define NUM_LEDS_16 16
#define DATA_PIN_16 8

#define NUM_LEDS_8 8
#define DATA_PIN_8 9

#define BRIGHTNESS 30

CRGB leds16[NUM_LEDS_16];
CRGB leds8[NUM_LEDS_8];

unsigned long tUltra = 0;
unsigned long tLED   = 0;
const unsigned long ULTRA_PERIOD = 120;
const unsigned long LED_PERIOD   = 150;

int servoAngle = 0;
int servoDir = 1;

long readDistance(int trig, int echo) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    long d = pulseIn(echo, HIGH, 20000);
    if (d == 0) return 999;
    return d * 0.034 / 2;
}

void fadeAll() {
    fadeToBlackBy(leds16, NUM_LEDS_16, 120);
    fadeToBlackBy(leds8, NUM_LEDS_8, 120);
}

int mapHalfRing(int angle, int numLeds, int side) {
    float idx;
    int half = numLeds / 2;
    if (side == 0) idx = angle * (half - 1) / 180.0;
    else idx = half + angle * (half - 1) / 180.0;
    return constrain(round(idx), 0, numLeds - 1);
}

void drawGreenSweep(int side, int angle) {
    leds16[mapHalfRing(angle, NUM_LEDS_16, side)] = CRGB::Green;
    leds8[mapHalfRing(angle, NUM_LEDS_8, side)]   = CRGB::Green;
}

void drawRedHit(int side, int angle, long distance) {
    if (distance >= MAX_DISTANCE) return;

    int i16 = mapHalfRing(angle, NUM_LEDS_16, side);
    int i8  = mapHalfRing(angle, NUM_LEDS_8, side);

    if (distance < MAX_DISTANCE / 2) {
        leds16[i16] = CRGB::Red;
        leds8[i8]   = CRGB::Red;
    } else {
        leds16[i16] = CRGB::Red;
    }
}

void setup() {
    radarServo.attach(servoPin);
    pinMode(TRIG_RIGHT, OUTPUT);
    pinMode(ECHO_RIGHT, INPUT);
    pinMode(TRIG_LEFT, OUTPUT);
    pinMode(ECHO_LEFT, INPUT);

    FastLED.addLeds<WS2812B, DATA_PIN_16, GRB>(leds16, NUM_LEDS_16);
    FastLED.addLeds<WS2812B, DATA_PIN_8, GRB>(leds8, NUM_LEDS_8);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    servoAngle += servoDir;
    if (servoAngle >= 180 || servoAngle <= 0) servoDir = -servoDir;
    radarServo.write(servoAngle);
    delay(10);

    if (millis() - tUltra >= ULTRA_PERIOD) {
        tUltra = millis();
        dRight = readDistance(TRIG_RIGHT, ECHO_RIGHT);
        dLeft  = readDistance(TRIG_LEFT, ECHO_LEFT);
    }

    if (millis() - tLED >= LED_PERIOD) {
        tLED = millis();
        int invAngle = 180 - servoAngle;
        fadeAll();
        drawGreenSweep(0, invAngle);
        drawGreenSweep(1, invAngle);
        drawRedHit(0, invAngle, dRight);
        drawRedHit(1, invAngle, dLeft);
        FastLED.show();
    }
}
