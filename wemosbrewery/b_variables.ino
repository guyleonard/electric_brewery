// Timing
unsigned long timeNow = 0;
unsigned long timeLast = 0;
int timeBlynkLast;

int hours = startingHour;
int minutesCounter = 0;
int timeEstimate = 0;
int timeTemp = 0;
int temperature;

int hop = 1;

// Heating
int heater1 = 0;
int heater2 = 0;
int heaterElement1 = D3;
int heaterElement2 = D4;

int heaterBlynk1 = LOW;

int iceNeeded = liters;

// Brew Stage Switch in BLUE
int switch1 = 0;
int switchState = 0;

// Temperature from NANO
int temp_calculated;
int nano_a1_pin_current;
int nano_a1_pin_previous = LOW;
long timeNowNano;
long timeLastNano;

// BLYNK
int blynkCalled;

// BLYNK LCDs
String blynk_LCD1 = "HELLO WORLD";
String blynk_LCD2 = "HELLO WORLD";


