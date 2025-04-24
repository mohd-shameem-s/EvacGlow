# 🔥 EVACGLOW: Smart Fire Evacuation Guidance System

**EVACGLOW** is an intelligent fire safety and evacuation system designed to detect fire, smoke, and high temperatures in two potential escape paths. Using sensors and visual/audio indicators, it helps guide individuals to the safest route during emergencies.

---

## 🚀 Features

- 🧠 **Auto-activation** when a person is detected nearby via ultrasonic sensor.
- 🔥 **Real-time fire, smoke, and temperature detection** using flame sensors, MQ2 gas sensors, and DHT11.
- 📊 **16x2 I2C LCD Display** shows the safety status of two evacuation paths.
- 🚦 **LED Indicators**:
  - Green = Safe
  - Blue = Caution
  - Red = Fire
- 🔊 **Buzzer Alarm** for active fire alerts.
- 🕒 **Auto-idle mode** after 45 seconds of inactivity.

---

## 🧰 Components Used

- Arduino Uno
- Ultrasonic Sensor (HC-SR04)
- Flame Sensors (x2)
- MQ2 Smoke/Gas Sensors (x2)
- DHT11 Temperature Sensors (x2)
- I2C LCD 16x2
- LEDs: Red, Blue, Green (x2 each)
- Buzzer
- Resistors, Jumper Wires, Breadboard

---

## ⚙️ How It Works

1. **Detection**: System activates when someone approaches (within 25 cm).
2. **Monitoring**: Continuously monitors two paths for:
   - 🔥 Flames
   - ☁️ Smoke
   - 🌡️ Temperature
3. **Classification**:
   - **Safe**: No danger detected.
   - **Caution**: Smoke > 250 OR Temp > 40°C.
   - **Fire**: Flame detected OR Smoke > 350 OR Temp > 50°C.
4. **Alerts**:
   - Visual alerts through LEDs.
   - Audio alert via buzzer if fire is detected.
   - Real-time LCD updates.

---

## 📷 Screenshots

> To be added

---

## 🛠️ Setup

1. Wire all components as per the schematic.
2. Upload the Arduino sketch from this repo to your board.
3. Open the Serial Monitor (9600 baud) to view debug logs.
4. Power up and approach the ultrasonic sensor to activate.

---

## 📜 License

MIT License. Feel free to use, modify, and share with credit.

---

## 🙌 Contributions

Feel free to fork this repo and contribute improvements or new features! PRs are welcome!

