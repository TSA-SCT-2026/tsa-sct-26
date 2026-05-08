#include <Arduino.h>
#include <ESP32Servo.h>

// Keep these in sync with firmware/tsa_sorter/config.h when recalibrating.
#define PIN_SELECTOR_SERVO      32
#define SELECTOR_BIN1_ANGLE_DEG 60
#define SELECTOR_BIN2_ANGLE_DEG 90
#define SELECTOR_BIN3_ANGLE_DEG 130
#define SELECTOR_BIN4_ANGLE_DEG 155
#define SELECTOR_UNLOAD_ANGLE_DEG 90

static Servo gServo;
static uint16_t gCurrentAngle = SELECTOR_BIN3_ANGLE_DEG;
static char gLineBuf[16];
static uint8_t gLineLen = 0;

static void moveTo(uint16_t angle) {
    if (angle > 180) angle = 180;
    gCurrentAngle = angle;
    gServo.write(angle);
    Serial.printf("-> %u deg\n", angle);
}

static void printStatus() {
    Serial.printf(
        "current=%u  bin1=%u  bin2=%u  bin3=%u  bin4=%u  unload=%u\n",
        gCurrentAngle,
        SELECTOR_BIN1_ANGLE_DEG,
        SELECTOR_BIN2_ANGLE_DEG,
        SELECTOR_BIN3_ANGLE_DEG,
        SELECTOR_BIN4_ANGLE_DEG,
        SELECTOR_UNLOAD_ANGLE_DEG);
}

static void printHelp() {
    Serial.println("Commands:");
    Serial.println("  <0-180>   move to angle");
    Serial.println("  1 2 3 4   move to bin angle");
    Serial.println("  u         unload position");
    Serial.println("  s         status");
    Serial.println("  h         help");
}

static void handleLine(const char* line) {
    if (line[0] == 'h') { printHelp(); return; }
    if (line[0] == 's') { printStatus(); return; }
    if (strcmp(line, "u") == 0) { moveTo(SELECTOR_UNLOAD_ANGLE_DEG); return; }
    if (strcmp(line, "1") == 0) { moveTo(SELECTOR_BIN1_ANGLE_DEG); return; }
    if (strcmp(line, "2") == 0) { moveTo(SELECTOR_BIN2_ANGLE_DEG); return; }
    if (strcmp(line, "3") == 0) { moveTo(SELECTOR_BIN3_ANGLE_DEG); return; }
    if (strcmp(line, "4") == 0) { moveTo(SELECTOR_BIN4_ANGLE_DEG); return; }

    int val = atoi(line);
    if (val >= 0 && val <= 180) {
        moveTo((uint16_t)val);
    } else {
        Serial.printf("? '%s'\n", line);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    gServo.attach(PIN_SELECTOR_SERVO);
    gServo.write(gCurrentAngle);
    delay(500);
    Serial.println("servo tuning ready");
    printStatus();
    printHelp();
}

void loop() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\r') continue;
        if (c == '\n') {
            gLineBuf[gLineLen] = '\0';
            if (gLineLen > 0) handleLine(gLineBuf);
            gLineLen = 0;
        } else if (gLineLen < sizeof(gLineBuf) - 1) {
            gLineBuf[gLineLen++] = c;
        }
    }
}
