# 🔥 EVACGLOW: Real-Time Fire Detection and Safe Path Indicator


EVACGLOW is an intelligent fire detection and evacuation guidance system designed to monitor environmental hazards such as flame, smoke, and temperature, and guide occupants toward safe evacuation paths using real-time sensors, visual indicators, and servo-controlled pathways.

## 📦 Features

- 🔥 Dual-path fire detection using flame, smoke (MQ2), and temperature (DHT11) sensors.
- 🚪 Automated door control with servo motors via PCA9685 driver.
- 📊 I2C LCD display for real-time status feedback.
- 🧠 Object detection using ultrasonic sensor to activate/deactivate system based on presence.
- 🔔 Alerting system with blinking LEDs and buzzer for emergency indication.
- 🔄 Idle timeout system to save power and reduce false alerts.
- 💡 Status logging through Serial Monitor.

---

## 🔌 Hardware Connections

### 🔋 Power
| Component      | Connection                        |
|---------------|------------------------------------|
| Arduino 5V     | → Breadboard power rail (+)        |
| Arduino GND    | → Breadboard ground rail (−)       |

> **⚠️ Servos must use an external 5V power supply**  
> Connect PCA9685 V+ to external 5V, and its GND to Arduino GND.

---

### 🖥️ I2C Devices
| Device         | VCC  | GND  | SDA  | SCL  |
|----------------|------|------|------|------|
| I2C LCD        | 5V   | GND  | A4   | A5   |
| PCA9685 Driver | 5V   | GND  | A4   | A5   |

> Shared I2C bus — no conflict.

---

### ⚙️ Servo Motors (via PCA9685)
| Servo           | PCA9685 Channel | Signal Pin | Power | Ground |
|-----------------|------------------|-------------|--------|--------|
| Path 1 Servo    | Channel 0        | PWM0        | V+     | GND    |
| Path 2 Servo    | Channel 1        | PWM1        | V+     | GND    |

---

### 🧲 Ultrasonic Sensor (HC-SR04)
| Pin   | Arduino |
|-------|---------|
| VCC   | 5V      |
| GND   | GND     |
| Trig  | D8      |
| Echo  | D9      |

---

### 🌡️ Temperature Sensors (DHT11)
| Sensor | Data Pin | VCC | GND |
|--------|----------|-----|-----|
| DHT1   | D2       | 5V  | GND |
| DHT2   | D3       | 5V  | GND |

---

### 🔥 Flame Sensors
| Sensor | OUT Pin | VCC | GND |
|--------|---------|-----|-----|
| Flame1 | A0      | 5V  | GND |
| Flame2 | A1      | 5V  | GND |

---

### 🌫️ Smoke Sensors (MQ2)
| Sensor | AOUT Pin | VCC | GND |
|--------|----------|-----|-----|
| MQ2_1  | A2       | 5V  | GND |
| MQ2_2  | A3       | 5V  | GND |

---

### 💡 LEDs (with 220Ω resistors)
| LED      | Arduino Pin |
|----------|-------------|
| GREEN1   | D4          |
| GREEN2   | D7          |
| BLUE1    | D5          |
| BLUE2    | D10         |
| RED1     | D6          |
| RED2     | D11         |

---

### 🔊 Buzzer
| Component | Arduino Pin |
|-----------|--------------|
| Buzzer    | D13          |

---

## 🧠 System Logic

- The system activates when an object is detected ≤ 25 cm.
- Checks each path for:
  - Flame (LOW signal)
  - Smoke (analog MQ2 > threshold)
  - Temperature (> caution/fire thresholds)
- Status Levels:
  - ✅ Safe → Green LED ON, servo opens path.
  - ⚠️ Caution → Blue LED blinking, servo closes path.
  - 🔥 Fire → Red LED blinking, buzzer on, servo closes path.
- Auto-deactivates after 30s of no object detected.

---

## 🧾 Code Overview

### Libraries Used
- `Wire.h` – I2C communication
- `LiquidCrystal_I2C.h` – LCD display
- `DHT.h` – DHT sensor
- `Adafruit_PWMServoDriver.h` – PCA9685

### Custom Functions
- `measureDistance()` – Returns ultrasonic distance in cm.
- `setServo(channel, angle)` – Controls PCA9685 servo angle.
- `turnOffAll()` – Turns off all LEDs and buzzer.

---

## ✅ Getting Started

1. **Connect hardware** according to the schematic above.
2. **Power the PCA9685 and servos separately** with 5V external power.
3. **Upload the sketch** to your Arduino Uno via Arduino IDE.
4. **Open Serial Monitor** at 9600 baud to view status logs.
5. **Approach the ultrasonic sensor** to activate the system.

---

## 🔄 System Status via Serial Monitor

```
=========== EVACGLOW STATUS ===========
Ultrasonic Distance : 18.4 cm | Object Detected : ✅ YES
System Active       : 🟢 YES
Path 1 -> Flame: ❄️OK | Smoke: 150 | Temp: 32.0°C | Status: ✅Safe | Door: 🔓Open
Path 2 -> Flame: 🔥YES | Smoke: 400 | Temp: 56.1°C | Status: FIRE | Door: 🔒Closed
======================================
```

---

## 🧪 Threshold Settings (can be adjusted)

```cpp
#define SMOKE_CAUTION 250
#define SMOKE_FIRE    350
#define TEMP_CAUTION  40
#define TEMP_FIRE     50
#define ACTIVATION_DISTANCE 25 // cm
#define idleTimeout   30000    // 30 seconds
```

---

## 🛠️ Troubleshooting

- 🔄 **Servos not working?**
  - Make sure V+ of PCA9685 is connected to **external 5V**, not Arduino.
- 📴 **System stays idle?**
  - Ensure ultrasonic sensor is powered and wired correctly.
- 🔇 **No buzzer sound during fire?**
  - Confirm RED1/RED2 LEDs blink – buzzer only activates in FIRE state.

---

## 📚 License

This project is open-source under the MIT License.

---

## 🙌 Acknowledgements

- Adafruit Libraries  
- Arduino Community  
- Fire safety and smart evacuation research

---

### 🚨 Stay Safe. Think Smart. Choose EVACGLOW. 🚨
