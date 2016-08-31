#include <Wire.h>
#include <SoftwareSerial.h>
#include <Suli.h>
#include <Four_Digit_Display_Arduino.h>

// Temperature Probes
#include <OneWire.h>
#include <DallasTemperature.h>

// Temperature probe data wire is plugged into port D2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


// DHT 11
#include "DHT.h"
#define DHTPIN 6   // pin D6 on the Arduino
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// 4-Digit Displays
Four_Digit_Display_Arduino disp;

// Display 1 - Wort
int character1;
int character2;
int character3;
int character4;

// Display 2 - Room
int character5;
int character6;
int character7;
int character8;


// Other
int temp_wort = 18; // temperature of the liquid
int temp_room = 18; // temperature in the room
int humidity_room = 30; // humidity of the room
int temp_to_wemos;


// MAIN CODE

// How many DHT11 Samples?
const int numReadings = 10;

// Function to return average temperature, about 2 degrees out, minus 2.
float temp_readings[numReadings];  // the readings from the input
int temp_index = 0;                // the index of the current reading
float temp_total = 0;              // the running total

float get_smoothed_temp() {
  temp_total = temp_total - temp_readings[temp_index]; // remove oldest reading
  temp_readings[temp_index] = dht.readTemperature();
  temp_total += temp_readings[temp_index];
  temp_index = (temp_index + 1) % numReadings;

  return ((temp_total / numReadings) - 2);
}

// Function to return average humidity, about 10% points out, add 10.
float humidity_readings[numReadings];  // the readings from the input
int humidity_index = 0;                // the index of the current reading
float humidity_total = 0;              // the running total

float get_smoothed_humidity() {
  humidity_total = humidity_total - humidity_readings[humidity_index]; // remove oldest reading
  humidity_readings[humidity_index] = dht.readHumidity();
  humidity_total += humidity_readings[humidity_index];
  humidity_index = (humidity_index + 1) % numReadings;

  return ((humidity_total / numReadings) + 10);
}

void setup()
{
  Serial.begin(9600);
  sensors.begin();
  dht.begin();
  disp.pointOff();

  pinMode (A1, OUTPUT);
}


// Display 1 = Liquid Temperature
void set_wort_values() {
  character1 = temp_wort / 10;
  character2 = temp_wort % 10;
  
  if (temp_wort >= 100) {
    character1 = 9;
    character2 = 9;
  }
}

// Display 2 = Room Temp and Humidity
void set_room_values() {
  character5 = temp_room / 10;
  character6 = temp_room % 10;
  character7 = humidity_room / 10;
  character8 = humidity_room % 10;

  if (temp_room >= 100) {
    character5 = 9;
    character6 = 9;
    character7 = humidity_room / 10;
    character8 = humidity_room % 10;
  }
}

// Display 1 = Liquid Temperature
// D9 and D10
void update_wort_lcd () {
  disp.begin(9, 10, 4);
  disp.display(0, character1);
  disp.display(1, character2);
  disp.display(2, character3);
  disp.display(3, character4);
}

// Display 2 = Room Temp and Humidity
// D11 and D12
void update_room_lcd () {
  disp.begin(11, 12, 4);
  disp.display(0, character5);
  disp.display(1, character6);
  disp.display(2, character7);
  disp.display(3, character8);
}

void loop() {
  sensors.requestTemperatures();
  temp_wort = sensors.getTempCByIndex(0);

  temp_room = (get_smoothed_temp());
  humidity_room = (get_smoothed_humidity());

  set_wort_values();
  delay(100);
  update_wort_lcd();
  delay(100);

  set_room_values();
  delay(100);
  update_room_lcd();
  delay(100);

  //The communication code from NANO to WEMOS
  //This code sends a HIGH to one of the wemos' digital ports for the duration of temperature in Celcius times 100
  //example: 22 degrees equals a 2200 ms pulse. We use a normal delay function for this.
  //The wemos counts how long it is being pulsed and divides the number back by 100 to get the last temperature readings from the NANO.

  temp_to_wemos = temp_wort * 100;

  digitalWrite (A1, HIGH);
  delay(temp_to_wemos);
  digitalWrite (A1, LOW);
}
