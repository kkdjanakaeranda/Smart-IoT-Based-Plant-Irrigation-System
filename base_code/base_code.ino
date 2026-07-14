// blink details for ashen
#define BLYNK_TEMPLATE_ID "" //add your template id
#define BLYNK_TEMPLATE_NAME "" //add your template name
#define BLYNK_AUTH_TOKEN "" //add your token

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

char ssid[] = "";//add your ssid
char pass[] = ""; // add your password

#define DHTPIN 4
#define DHTTYPE DHT22

#define SOIL_PIN 34

#define RAIN_DIGITAL 26
#define RAIN_ANALOG 32

#define TRIG_PIN 5
#define ECHO_PIN 18

#define RELAY_PIN 23
#define GREEN_LED 25
#define RED_LED 33
#define BUZZER_PIN 27
#define BUTTON_PIN 14

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd address
BlynkTimer timer;

bool pumpState = false;
bool manualMode = false;
bool manualPumpRequest = false;

int soilDryValue = 3500;
int soilWetValue = 1200;

int tankHeightCM = 20; // tank height

int soilPercent = 0;
float temperature = 0;
float humidity = 0;
int rainAnalog = 0;
bool isRaining = false;
int tankPercent = 0;

BLYNK_WRITE(V6) {
  manualMode = param.asInt() == 1;
}

BLYNK_WRITE(V8) {
  manualPumpRequest = param.asInt() == 1;

  Serial.print("V8 Manual Pump = ");
  Serial.println(manualPumpRequest ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  pinMode(RAIN_DIGITAL, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(RELAY_PIN, LOW); // relay OFF
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  analogReadResolution(12);
  analogSetPinAttenuation(SOIL_PIN, ADC_11db);
  analogSetPinAttenuation(RAIN_ANALOG, ADC_11db);

  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("Connecting WiFi");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Blynk Connected");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);

  timer.setInterval(1000L, mainSystem);
  timer.setInterval(2000L, sendToBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
}

void mainSystem() {
  readSensors();
  controlPump();
  updateOutputs();
  updateLCD();
  printSerial();
}

void readSensors() {
  int soilRaw = analogRead(SOIL_PIN);
  soilPercent = map(soilRaw, soilDryValue, soilWetValue, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  rainAnalog = analogRead(RAIN_ANALOG);
  isRaining = digitalRead(RAIN_DIGITAL) == LOW;

  tankPercent = getWaterLevelPercent();
}

void controlPump() {
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  if (buttonPressed) {
    if (manualMode) {
      manualPumpRequest = !manualPumpRequest;
    } else {
      manualMode = true;
      manualPumpRequest = true;
    }

    Blynk.virtualWrite(V6, manualMode ? 1 : 0);
    Blynk.virtualWrite(V8, manualPumpRequest ? 1 : 0);

    delay(300);
  }

  if (manualMode) {
    pumpState = manualPumpRequest;
  } else {
    pumpState = soilPercent < 35 && !isRaining && tankPercent > 20;
  }

  if (tankPercent <= 20) {
    pumpState = false;
  }
}

void updateOutputs() {
  digitalWrite(RELAY_PIN, pumpState ? HIGH : LOW);

  if (tankPercent <= 20) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);

    if (soilPercent >= 35 && !isRaining) {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
    } else {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
    }
  }
}

void updateLCD() {

  static int screen = 0;
  static unsigned long lastChange = 0;

  if (millis() - lastChange >= 2000) {
    screen = !screen;   // Toggle between 0 and 1
    lastChange = millis();
  }

  lcd.clear();

  if (screen == 0) {

    lcd.setCursor(0, 0);
    lcd.print(manualMode ? "MAN " : "AUTO");
    lcd.print(" S:");
    lcd.print(soilPercent);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("W:");
    lcd.print(tankPercent);
    lcd.print("% ");
    lcd.print(pumpState ? "P:ON" : "P:OFF");

  } else {

    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(temperature, 1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum:");
    lcd.print((int)humidity);
    lcd.print("% ");

    lcd.print("R:");
    lcd.print(isRaining ? "YES" : "NO");
  }
}

void sendToBlynk() {
  Blynk.virtualWrite(V0, soilPercent);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, tankPercent);
  Blynk.virtualWrite(V4, rainAnalog);
  Blynk.virtualWrite(V5, pumpState ? 1 : 0);
  Blynk.virtualWrite(V6, manualMode ? 1 : 0);
  Blynk.virtualWrite(V7, isRaining ? 1 : 0);
  Blynk.virtualWrite(V8, manualPumpRequest ? 1 : 0);
}

void printSerial() {
  Serial.println("----- Smart Irrigation -----");

  Serial.print("Mode: ");
  Serial.println(manualMode ? "MANUAL" : "AUTO");

  Serial.print("Manual Pump Request: ");
  Serial.println(manualPumpRequest ? "ON" : "OFF");

  Serial.print("Soil: ");
  Serial.print(soilPercent);
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Rain Analog: ");
  Serial.println(rainAnalog);

  Serial.print("Rain Status: ");
  Serial.println(isRaining ? "RAIN DETECTED" : "NO RAIN");

  Serial.print("Tank: ");
  Serial.print(tankPercent);
  Serial.println("%");

  Serial.print("Pump: ");
  Serial.println(pumpState ? "ON" : "OFF");

  Serial.println();
}

int getWaterLevelPercent() {
  long duration;
  float distanceCM;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return tankPercent;
  }

  distanceCM = duration * 0.034 / 2;

  int waterLevel = tankHeightCM - distanceCM;
  int percentage = map(waterLevel, 0, tankHeightCM, 0, 100);

  return constrain(percentage, 0, 100);
}
