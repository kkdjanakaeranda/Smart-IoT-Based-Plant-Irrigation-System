# 🌱 Smart IoT-Based Plant Irrigation System

An IoT-based Smart Plant Irrigation System developed using **ESP32** to automate irrigation based on real-time environmental conditions. The system continuously monitors soil moisture, temperature, humidity, rainfall, and water tank level to make intelligent irrigation decisions. It also provides remote monitoring and manual control through the **Blynk IoT** mobile application.

---

## 📌 Features

* 🌱 Automatic irrigation based on soil moisture level
* 📱 Remote monitoring and pump control using Blynk IoT
* 🌡️ Temperature and humidity monitoring using DHT22
* 🌧️ Rain detection to prevent unnecessary watering
* 💧 Water tank level monitoring using an HC-SR04 ultrasonic sensor
* 🚰 Automatic pump control through a relay module
* 🔔 Low water level alert using buzzer
* 🟢🔴 System status indication using LEDs
* 📟 16×2 I2C LCD displaying system information
* 🔄 Automatic and Manual operating modes

---

# 📷 Project Overview

The Smart Irrigation System automatically waters plants only when required.

The ESP32 continuously reads sensor data and decides whether irrigation is necessary based on:

* Soil moisture level
* Rain detection
* Water tank availability

Users can also monitor sensor values and manually control the irrigation pump using the Blynk mobile application.

---

# 🛠 Hardware Components

| Component                            | Quantity    |
| ------------------------------------ | ----------- |
| ESP32 DevKit V1 (30-pin)             | 1           |
| Capacitive Soil Moisture Sensor v1.2 | 1           |
| DHT22 Temperature & Humidity Sensor  | 1           |
| FC-37 Rain Sensor Module             | 1           |
| HC-SR04 Ultrasonic Sensor            | 1           |
| 5V Single Channel Relay Module       | 1           |
| 5V Mini Water Pump                   | 1           |
| 16×2 LCD with I2C Module             | 1           |
| Active Buzzer                        | 1           |
| Green LED                            | 1           |
| Red LED                              | 1           |
| Push Button                          | 1           |
| Jumper Wires                         | As Required |
| Breadboard                           | 1           |
| External 5V Power Supply             | 1           |

---

# 💻 Software

* Arduino IDE
* ESP32 Board Package
* Blynk IoT Platform
* LiquidCrystal_I2C Library
* DHT Sensor Library
* WiFi Library

---

# ⚙️ Pin Configuration

| ESP32 Pin | Component            |
| --------- | -------------------- |
| GPIO4     | DHT22                |
| GPIO34    | Soil Moisture Sensor |
| GPIO32    | Rain Sensor Analog   |
| GPIO26    | Rain Sensor Digital  |
| GPIO5     | HC-SR04 Trigger      |
| GPIO18    | HC-SR04 Echo         |
| GPIO23    | Relay Module         |
| GPIO25    | Green LED            |
| GPIO33    | Red LED              |
| GPIO27    | Active Buzzer        |
| GPIO14    | Push Button          |
| GPIO21    | LCD SDA              |
| GPIO22    | LCD SCL              |

---

# 📱 Blynk Dashboard

| Virtual Pin | Description              |
| ----------- | ------------------------ |
| V0          | Soil Moisture (%)        |
| V1          | Temperature (°C)         |
| V2          | Humidity (%)             |
| V3          | Water Tank Level (%)     |
| V4          | Rain Sensor Analog Value |
| V5          | Pump Status              |
| V6          | Auto / Manual Mode       |
| V7          | Rain Detection Status    |
| V8          | Manual Pump Control      |

---

## ⚙️ Configuration

Before uploading the code to the ESP32, update the following values in the Arduino sketch.

### Blynk Configuration

```cpp
// Blynk Details
#define BLYNK_TEMPLATE_ID ""      // Add your Blynk Template ID
#define BLYNK_TEMPLATE_NAME ""    // Add your Blynk Template Name
#define BLYNK_AUTH_TOKEN ""       // Add your Blynk Authentication Token
```

### Wi-Fi Configuration

```cpp
char ssid[] = "";   // Enter your Wi-Fi SSID
char pass[] = "";   // Enter your Wi-Fi Password
```

After updating these values, upload the sketch to your ESP32 using the Arduino IDE.

---

## 📚 Required Libraries

Install the following libraries from the Arduino Library Manager:

- Blynk by Volodymyr Shymanskyy
- DHT Sensor Library by Adafruit
- Adafruit Unified Sensor
- LiquidCrystal I2C
- WiFi (ESP32 Built-in)
- Wire (ESP32 Built-in)

---

# 🔄 System Workflow

1. ESP32 powers on and connects to Wi-Fi.
2. Blynk cloud connection is established.
3. Sensors continuously collect environmental data.
4. ESP32 evaluates:

   * Soil moisture
   * Rain status
   * Water tank level
5. In **Automatic Mode**, irrigation starts only when:

   * Soil moisture is below the threshold.
   * No rain is detected.
   * Tank level is above the minimum limit.
6. In **Manual Mode**, the pump can be controlled through the Blynk application.
7. LCD displays live system information.
8. Sensor readings are transmitted to the Blynk dashboard.

---

# 🧠 Automatic Irrigation Logic

The irrigation pump turns **ON** only if all of the following conditions are met:

* Soil moisture is below **35%**
* Rain is **not** detected
* Water tank level is above **20%**

Otherwise, the pump remains **OFF**.

---

# 🚨 Safety Features

* Prevents pump operation when the water tank level is below 20%.
* Buzzer alerts the user when the water tank is nearly empty.
* Red LED indicates warning conditions.
* Green LED indicates normal operating conditions.

---

# 📟 LCD Information

The LCD automatically alternates between two screens every 2 seconds.

### Screen 1

```text
AUTO S:45%
W:80% P:ON
```

Displays:

* Operating Mode
* Soil Moisture
* Water Tank Level
* Pump Status

### Screen 2

```text
Temp:30.5°C
Hum:72% R:NO
```

Displays:

* Temperature
* Humidity
* Rain Status

---

# 📂 Project Structure

```text
Smart-IoT-Based-Plant-Irrigation-System/
│
├── base_code/
|   ├── base_code.ino
├── README.md
├── images/
│   ├── prototype.jpg
│   ├── circuit_diagram.png
│   └── blynk_dashboard.png
└── docs/
    └── Project_Report.pdf
```

---

# 🚀 Future Improvements

* Firebase cloud database integration
* Historical data logging
* Weather forecast integration
* OLED graphical display
* Mobile notifications
* Solar-powered operation
* AI-based irrigation prediction

---

# 👨‍💻 Developed By

**Janaka Eranda**

IoT Course Project

ESP32 • Blynk IoT • Arduino IDE
