Here is the exact Markdown text for your README.md file.

Copy everything in the block below, paste it into your README.md file in Notepad, and save it:

Markdown
# ESP32 Temperature-Controlled Cooling System with Hysteresis

An embedded IoT project built using the ESP32 microcontroller to monitor real-time temperature and dynamically regulate a cooling fan. The system utilizes hysteresis control logic to ensure operational stability and prevent rapid switching near trigger thresholds.

---

## Key Features & Technical Concepts

* **Closed-Loop Feedback Control:** Real-time temperature acquisition and automated switching output.
* **Hysteresis Logic:** Dual-threshold control (25°C ON / 23°C OFF) to eliminate state chatter and extend actuator hardware life.
* **Visual Telemetry:** Live temperature readout and actuator status updating on a 128x64 I2C OLED display.
* **Power Isolation:** Low-voltage gate driver configuration using an IRLZ44N N-channel MOSFET to safely isolate microcontroller logic (3.3V) from high-current DC motor loads (5V).

---

## 🛠️ Hardware Components

* **Microcontroller:** ELEGOO ESP32 Development Board
* **Sensor:** DS18B20 Digital Temperature Sensor Module (1-Wire)
* **Display:** 0.96" SSD1306 OLED Screen (I2C)
* **Switching:** IRLZ44N Logic-Level N-Channel MOSFET
* **Actuator:** 5V USB Brushless Cooling Fan
* **Prototyping:** Breadboard, 220Ω & 10kΩ Resistors, Jumper Wires

---

## Pin Configuration

| Component Module | ESP32 Pin | Signal / Type |
| :--- | :--- | :--- |
| **DS18B20 Sensor** | GPIO 4 | Digital Data (1-Wire) |
| **OLED SDA** | GPIO 21 | I2C Data |
| **OLED SCL** | GPIO 22 | I2C Clock |
| **MOSFET Gate** | GPIO 16 | Digital Output (PWM/High) |

---

## How It Works

1. **Initialization:** The ESP32 initializes I2C bus communications, the OLED display, and 1-wire protocol drivers.
2. **Data Acquisition:** The DS18B20 samples real-time environmental thermal data every second.
3. **Threshold Processing (Hysteresis):**
   * If `Temperature >= 25.0°C`, the MOSFET Gate toggles HIGH, turning **ON** the cooling fan.
   * The fan remains active as temperature drops until `Temperature <= 23.0°C`, at which point the Gate toggles LOW to turn **OFF** the fan.
4. **Telemetry Display:** Temperature values and state flags (`Fan: ON/OFF`) render on
