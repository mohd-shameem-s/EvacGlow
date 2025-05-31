#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_PWMServoDriver.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // PCA9685

// Ultrasonic sensor
#define TRIG 8
#define ECHO 9
#define ACTIVATION_DISTANCE 25  // cm

// Sensor and output pins
#define FLAME1 A0
#define FLAME2 A1
#define MQ2_1 A2
#define MQ2_2 A3
#define DHT1_PIN 2
#define DHT2_PIN 3
#define GREEN1 4
#define GREEN2 7
#define BLUE1 5
#define BLUE2 10
#define RED1 6
#define RED2 11
#define BUZZER 13

// PCA9685 channels for servos
#define SERVO1_CH 0
#define SERVO2_CH 1

DHT dht1(DHT1_PIN, DHT11);
DHT dht2(DHT2_PIN, DHT11);

// Timing and thresholds
#define SMOKE_CAUTION 250
#define SMOKE_FIRE 350
#define TEMP_CAUTION 40
#define TEMP_FIRE 50

unsigned long previousBlinkMillis = 0;
const long blinkInterval = 400;
unsigned long previousCheckMillis = 0;
const long checkInterval = 2000;
const long idleTimeout = 30000;

bool systemActive = false;
unsigned long lastDetectedTime = 0;
bool red1State = false, red2State = false;
bool blue1State = false, blue2State = false;
bool buzzerState = false;
bool fire1 = false, fire2 = false;
bool caution1 = false, caution2 = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  Wire.begin(); // Initiate I2C for both LCD and PCA9685
  pwm.begin();
  pwm.setPWMFreq(50); // Standard 50 Hz for servos

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  dht1.begin();
  dht2.begin();

  int pins[] = { GREEN1, GREEN2, BLUE1, BLUE2, RED1, RED2, BUZZER };
  for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }

  setServo(SERVO1_CH, 0);    // Path 1 closed
  setServo(SERVO2_CH, 180);  // Path 2 closed (reverse logic)

  lcd.setCursor(0, 0);
  lcd.print("::  EVACGLOW  ::");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousCheckMillis >= checkInterval) {
    previousCheckMillis = currentMillis;

    float distance = measureDistance();
    bool objectDetected = (distance > 0 && distance <= ACTIVATION_DISTANCE);

    if (objectDetected) {
      if (!systemActive) Serial.println("🟢 Object detected - System Activated");
      systemActive = true;
      lastDetectedTime = currentMillis;
    } else {
      if (systemActive) Serial.println("🔴 No object detected - System Idle");
    }

    if (systemActive && (currentMillis - lastDetectedTime >= idleTimeout)) {
      Serial.println("⏳ No object for 30s - System Deactivated");
      systemActive = false;
    }

    Serial.println("=========== EVACGLOW STATUS ===========");
    Serial.print("Ultrasonic Distance : ");
    Serial.print(distance, 1);
    Serial.print(" cm    | Object Detected : ");
    Serial.println(objectDetected ? "✅ YES" : "❌ NO");
    Serial.print("System Active       : ");
    Serial.println(systemActive ? "🟢 YES" : "🔴 NO");
    Serial.print("Idle Timeout        : 30s\n");

    if (!systemActive) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("::  EVACGLOW  ::");
      lcd.setCursor(0, 1);
      lcd.print("System Idle 30s");
      turnOffAll();
      setServo(SERVO1_CH, 0);    // Close
      setServo(SERVO2_CH, 180);  // Close
      Serial.println("❌ No object detected - System Idle");
      Serial.println("=====================================\n");
      return;
    }

    int flameVal1 = digitalRead(FLAME1);
    int flameVal2 = digitalRead(FLAME2);
    int smoke1 = analogRead(MQ2_1);
    int smoke2 = analogRead(MQ2_2);
    float temp1 = dht1.readTemperature();
    float temp2 = dht2.readTemperature();

    fire1 = (flameVal1 == LOW || smoke1 > SMOKE_FIRE || temp1 > TEMP_FIRE);
    fire2 = (flameVal2 == LOW || smoke2 > SMOKE_FIRE || temp2 > TEMP_FIRE);
    caution1 = (!fire1 && (smoke1 > SMOKE_CAUTION || temp1 > TEMP_CAUTION));
    caution2 = (!fire2 && (smoke2 > SMOKE_CAUTION || temp2 > TEMP_CAUTION));

    // Servo logic
    setServo(SERVO1_CH, (fire1 || caution1) ? 0 : 90);
    setServo(SERVO2_CH, (fire2 || caution2) ? 180 : 90); // reverse logic

    // Print Path 1
    Serial.print("Path 1 -> Flame: ");
    Serial.print(flameVal1 == LOW ? "🔥YES" : "❄️OK");
    Serial.print(" | Smoke: "); Serial.print(smoke1);
    Serial.print(" | Temp: "); Serial.print(temp1); Serial.print("°C");
    Serial.print(" | Status: ");
    Serial.print(fire1 ? "FIRE" : (caution1 ? "⚠️Caution" : "✅Safe"));
    Serial.print(" | Door: ");
    Serial.println((fire1 || caution1) ? "🔒Closed" : "🔓Open");

    // Print Path 2
    Serial.print("Path 2 -> Flame: ");
    Serial.print(flameVal2 == LOW ? "🔥YES" : "❄️OK");
    Serial.print(" | Smoke: "); Serial.print(smoke2);
    Serial.print(" | Temp: "); Serial.print(temp2); Serial.print("°C");
    Serial.print(" | Status: ");
    Serial.print(fire2 ? "FIRE" : (caution2 ? "⚠️Caution" : "✅Safe"));
    Serial.print(" | Door: ");
    Serial.println((fire2 || caution2) ? "🔒Closed" : "🔓Open");
    Serial.println("======================================\n");

    // LCD Display
    lcd.clear();
    if (fire1) {
      red1State = true;
      digitalWrite(GREEN1, LOW);
      lcd.setCursor(0, 0); lcd.print("Path 1: FIRE!");
    } else if (caution1) {
      blue1State = true;
      red1State = false;
      digitalWrite(GREEN1, LOW);
      lcd.setCursor(0, 0); lcd.print("Path 1: Caution");
    } else {
      red1State = false; blue1State = false;
      digitalWrite(GREEN1, HIGH);
      lcd.setCursor(0, 0); lcd.print("Path 1: Safe");
    }

    if (fire2) {
      red2State = true;
      digitalWrite(GREEN2, LOW);
      lcd.setCursor(0, 1); lcd.print("Path 2: FIRE!");
    } else if (caution2) {
      blue2State = true; red2State = false;
      digitalWrite(GREEN2, LOW);
      lcd.setCursor(0, 1); lcd.print("Path 2: Caution");
    } else {
      red2State = false; blue2State = false;
      digitalWrite(GREEN2, HIGH);
      lcd.setCursor(0, 1); lcd.print("Path 2: Safe");
    }
  }

  // Blinking LED & Buzzer
  if (millis() - previousBlinkMillis >= blinkInterval) {
    previousBlinkMillis = millis();
    digitalWrite(RED1, red1State ? !digitalRead(RED1) : LOW);
    digitalWrite(RED2, red2State ? !digitalRead(RED2) : LOW);
    digitalWrite(BLUE1, blue1State ? !digitalRead(BLUE1) : LOW);
    digitalWrite(BLUE2, blue2State ? !digitalRead(BLUE2) : LOW);

    if (fire1 || fire2) {
      buzzerState = !buzzerState;
      digitalWrite(BUZZER, buzzerState);
    } else {
      digitalWrite(BUZZER, LOW);
      buzzerState = false;
    }
  }
}

float measureDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void turnOffAll() {
  int pins[] = { GREEN1, GREEN2, BLUE1, BLUE2, RED1, RED2, BUZZER };
  for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    digitalWrite(pins[i], LOW);
  }
}

void setServo(uint8_t channel, int angle) {
  int pulse = map(angle, 0, 180, 102, 512); // PCA9685 pulse width
  pwm.setPWM(channel, 0, pulse);
}
