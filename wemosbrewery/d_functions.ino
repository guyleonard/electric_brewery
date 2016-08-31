// BLYNK Servers only allow 10 requests per second.
// If you make more requests your project will be disconnected and reconnected
// causing a lot of applications to crash. That is why we call 3 different functions
// to ensure only a limited amount of requests are made every second.

void blynkUpdate1 () {
  if ((timeNow - timeBlynkLast) > 5 && blynkCalled == 0) {
    Blynk.virtualWrite(9, minutesCounter);
    Blynk.virtualWrite(2, heater1);
    Blynk.virtualWrite(4, heater2);
    Blynk.virtualWrite(5, minutes);
    Blynk.virtualWrite(10, temperature);

    blynkCalled = 1;
  }
}

void blynkUpdate2 () {
  if ((timeNow - timeBlynkLast) > 10 && blynkCalled == 1) {
    Blynk.virtualWrite(6, brewStage);
    Blynk.virtualWrite(7, temperature);
    // Blynk.virtualWrite(8, timeEstimate);
    blynkCalled = 2;
  }
}

void blynkUpdate3 () {
  if ((timeNow - timeBlynkLast) > 15 && blynkCalled == 2) {
    lcd.clear();
    lcd.print(0, 0, blynk_LCD1); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(0, 1, blynk_LCD2); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    timeBlynkLast = timeNow;
    blynkCalled = 0;
  }
}

// This gets the current time in milliseconds
// then detects if there is a HIGH burst from the NANO
// and records the time until LOW
// and then converts it to the current temperature
// it needs some work, renaming and averaging

float temperatureFunction2 () {
  timeNowNano = millis();
  nano_a1_pin_current = digitalRead(D0);

  if (nano_a1_pin_current == HIGH && nano_a1_pin_previous == LOW) {
    timeLastNano = timeNowNano;
    nano_a1_pin_previous = HIGH;
  }
  else if (nano_a1_pin_current == LOW && nano_a1_pin_previous == HIGH) {
    temp_calculated = (timeNowNano - timeLastNano) / 100;
    nano_a1_pin_previous = LOW;
  }

  return temp_calculated;
}

void IceFunction () {

  iceNeeded = liters / 2;
}
//This function tells you how much ice you need to cool your boiling wort
// The formula may seem simple here, but it is dependant on many factors and not 100 procent accurate. A lot of research went into this easy formula. According to http://hyperphysics.phy-astr.gsu.edu/hbase/thermo/cice.html
// the phase change caused by adding 50% ice to your total volume, makes the temperature drop from 100 Celsius to 40 Celsius very fast
//The remaing temperature will be dropped by vapor, the difference in temperature with the ice,the pouring to the fermenter and the difference in temperature with roomtemperature.
//The risk of off flavors is also reduced significantly one the wort is cooled under 50 degrees celcius, so those last 30 degrees can take a little longer

//NOTE: THE ICE SHOULD BE ADDED IN THE FORM OF FROZEN BOTTLES, SO THAT YOU CAN TAKE THEM OUT AFTERWARDS WITHOUT DILUTING YOUR WORT.

////THE FUNCTIONS BELOW READ THE VALUES OF THE SLIDE BARS IN THE APP - THIS IS NOT STABLE YET, UNCOMMENT AT OWN RISK
//BLYNK_WRITE(V30)
//{
//  boilTemp = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V31)
//{
//mashTemp =  param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V37)
//{
//  liters = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V32)
//{
//  yeastPitchTemperature = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V33)
//{
//  fermentationTemperatureMax = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V34)
//{
//  fermentTimeprimary = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V35)
//{
//  mashTime = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V36)
//{
//  boilTime = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//BLYNK_WRITE(V40)
//{
//  HopTime1= param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V41)
//{
//  HopTime2 = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V42)
//{
//  HopTime3 = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
//
//BLYNK_WRITE(V43)
//{
//  HopTime4 = param.asInt();
//  Serial.println(param.asInt());
//  // You can also use:
//  // int i = param.asInt() or
//  // double d = param.asDouble()
//}
