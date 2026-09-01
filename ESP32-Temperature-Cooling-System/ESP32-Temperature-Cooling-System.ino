// header libraries 
// Wire.h enables I2C communication  
// I2C protocol used to send data over two wires BETWEEN OLED AND ESP32
// I2C protocol OLED screen uses 
// ADA handles pixels on OLED
// OneWire and DallasTemp used to translate raw electric signals from DS18B20 sensor into human readable temps 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// labels for physical pins
// GPIO - physical metal pins on ESP32 microcontroller 
// pin 4 reads from DS18B20 sensor 
// pin 16 controls fan pin
#define ONE_WIRE_BUS 4    // DS18B20 Data Pin -> GPIO 4
#define FAN_PIN      16   // MOSFET Gate Pin -> GPIO 16

// OLED Screen Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// oneWire() creates a 1 wire communcation link on pin 4
// oneWire is not I2C, it uses a single data wire 
// sensors() hands that link over to the Dallas Temperature library so that it can calculate temperature 
// sensors convert the low level signals coming on pin 4 and converts to standard temperature units
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Threshold values
const float TEMP_ON  = 30.0; // Turn fan ON when temp reaches/exceeds 30.0°C
const float TEMP_OFF = 27.0; // Turn fan OFF when temp drops to/below 27.0°C

bool fanState = false;

void setup() {
  Serial.begin(115200);
  
  // configure pin 16 so ther ESP32 can send voltage out to switch the MOSFET
  pinMode(FAN_PIN, OUTPUT);
  // makes sure pin 16 output starts at 0 volts 
  digitalWrite(FAN_PIN, LOW); // Ensure fan starts OFF

  // Initialize OneWire Sensor
  sensors.begin();

  // Initialize OLED ar its I2C address 0x3C 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    // if it can't display freeze her e
    for (;;); 
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("System Initializing...");
  display.display();
  delay(1500);
}

void loop() {
  // Request temperature read from sensor
  sensors.requestTemperatures();
  float currentTemp = sensors.getTempCByIndex(0);

  // if sensor reads 30 or higher, fanState becomes true
  // if temperature is below 27, fanState becomes false
  // if temperature is between 27.1 and 29.9 fanstate stays on whatever if was on previously hence fan will not be rapidly clicking between ON and OFF
  if (currentTemp >= TEMP_ON) {
    fanState = true;
  } else if (currentTemp <= TEMP_OFF) {
    fanState = false;
  }

  // send 3.3V to output pin 16 on ESP32 is fanstate is true
  digitalWrite(FAN_PIN, fanState ? HIGH : LOW);

  // --- Update OLED Telemetry ---
  display.clearDisplay();
  
  // Header
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("THERMAL CONTROL");
  display.drawFastHLine(0, 10, 128, WHITE);

  // Current Temperature Readout
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(currentTemp, 1);
  display.print((char)247); // Degree symbol
  display.println("C");

  // Fan Status Display
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print("Fan Status: ");
  if (fanState) {
    display.println("ON [COOLING]");
  } else {
    display.println("OFF [IDLE]");
  }

  display.display();
  delay(1000); // Sample every 1 second
}
