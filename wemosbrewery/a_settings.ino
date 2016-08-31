// Recipe Settings
// Liters you will make
int liters = 25; //The number of liters of boiled wort you are planning on having AFTER the boil.

// Mash Stage
int mashTemp = 65; // Temp in celcius
int mashTime = 75; // number of minutes you are going to mash

// Boil Stage
int boilTemp = 99; // Temp in celcius, set to max 99
int boilTime = 60; // number of minutes you are going to boil

// Hop Additions
int HopTime1 = 5; // after how many minutes of boiling the wort should this hop be added?
int HopTime2 = 10;
int HopTime3 = 15;
int HopTime4 = 20;

// Fermentation Stage
int fermentTimeprimary = 14; // Number of days you are going to ferment
int fermentTimeSecondary = 0; // Number of days you are going to ferment in secondary, set to zero if you do not plan on using secondary fermenting, this is often not required.

int yeastPitchTemperature = 22; // The ideal pitch temperature for your yeast, in celcius.

int fermentationTemperatureMin = 20; // Minimum Temp that the yeast can thrive in, in celcius.
int fermentationTemperatureMax = 24; // Maximum Temp that the yeast can thrive in, in celcius.


// Brew stages, adjust this if you faced a power out, or reset at some point during the brew, otherwise leave at zero.
// 0 is standby
// 1 is heating strike water
// 2 is waiting for mash
// 3 is mashing
// 4 is extracting mash and waiting for boil
// 5 is heating towards boil
// 6 is boil
// 7 is cooling
// 8 is adding yeast
// 9 is fermenting primary
// 10 is fermenting secondary
int brewStage = 0;


// Equipment Settings
//How many KiloWatt are your heating elements combined?
int KiloWatt = 3;


// Arduino Settings
// Time basic settings
int startingHour = 0; // set your starting hour here, not at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 0;
int days = 0;


// Time accuracy settings
int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis
int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch


// Blynk Settings
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c21a8fe8af874a119c1649d3d846bacb";
