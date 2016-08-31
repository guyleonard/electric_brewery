void loop() {

  Blynk.run();
  blynkUpdate1();
  blynkUpdate2();
  blynkUpdate3();

  //TEMPERATURE SETTINGS
  temperature = temperatureFunction2();

  //Switch on the heaters if the function calls for this
  digitalWrite(heaterElement1, heater1);
  digitalWrite(heaterElement2, heater2);
  digitalWrite(D5, heater1); // RED LED

  timeNow = millis() / 1000; // the number of milliseconds that have passed since boot
  seconds = timeNow - timeLast; // the number of seconds that have passed since the last time 60 seconds was reached.

  // if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.
  if (seconds >= 60) {
    timeLast = timeNow;
    minutes = minutes + 1;
    minutesCounter = minutesCounter + 1;
  }

  // if one hour has passed, start counting minutes from zero and add one hour to the clock
  if (minutes >= 60) {
    minutes = 0;
    hours = hours + 1;
  }

  //if 24 hours have passed , add one day
  if (hours == 24) {
    hours = 0;
    days = days + 1;
  }

  if (hours == (24 - startingHour) && correctedToday == 0) {
    delay(dailyErrorFast * 1000);
    seconds = seconds + dailyErrorBehind;
    correctedToday = 1;
  }
  //every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds.
  //Change these varialbes according to the error of your board.
  // The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days
  // and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).
  if (hours == 24 - startingHour + 2) {
    correctedToday = 0;
  }


  // read the value of the switch (BLUE)
  // digitalRead() checks to see if there is voltage
  // on the pin or not
  switch1 = digitalRead(D6);
  if (switch1 == HIGH) {
    switchState = HIGH;
  }

  // below the various brew stages and what the program does during
  // 0 is standby
  // 1 is heating strike water
  // 2 is waiting for mash
  // 3 is mashing
  // 4 is extracting mash bag and waiting for boil,
  // 5 is heating towards boil
  // 6 is boil
  // 7 is Ice cooling
  // 8 is pitching
  // 9 is primary fermentation
  // 10 is secondary fermentation
  // 11 is bottling

  switch (brewStage) {

    case 0:
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("  BREWERY ");
      display.println("   READY  ");
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println("  Press   ");
      display.println(" BLUE BTN ");
      display.println(" To START ");
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "AUTOBREW Ready";
      blynk_LCD2 = "Enjoy Brewing!";

      if (brewStage == 0 && switchState == HIGH) {
        brewStage = 1;
        terminal.println("PREPARING STRIKE WATER. STIR EVERY FEW MINUTES");
        terminal.flush();
        Blynk.notify(String("AUTOBREW: Heating the Strike Water"));
      }
      break;

    case 1:
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("HEATING");
      display.println("STRIKE WTR");
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println(minutesCounter);
      display.println(temperature);
      display.print(hours);
      display.print(":");
      display.print(minutes);
      display.print(":");
      display.println(seconds);
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "Heating up the";
      blynk_LCD2 = "Strike Water";

      //timeTemp = (mashTemp - temperature) / ((KiloWatt * 3412) / (liters * 4) / 60);
      //timeEstimate = timeTemp - minutesCounter;
      //find this formula and a great calculator at https://elementsofheating.wordpress.com/2012/09/26/how-to-calculate-the-kw-required-to-heat-a-volume-of-water-in-a-particular-time/

      heater1 = HIGH;
      heater2 = HIGH;

      if (brewStage == 1 && temperature > mashTemp) {
        heater1 = LOW;
        heater2 = LOW;

        switchState = 0;
        brewStage = 2;
        terminal.println("ADD THE MASH BAG NOW. MAKE SURE IT IS FASTENED SECURELY. PUT THE TEMPERATURE PROBE INSIDE THE MASH BAG WITH YOUR GRAINS");
        terminal.flush();

        Blynk.notify(String("AUTOBREW: Add the Grains Now!"));
      }
      break;

    case 2:
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("  WATER  ");
      display.println("  READY  ");
      display.println();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println("  Press   ");
      display.println(" BLUE BTN ");
      display.println(" To MASH  ");
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "Strike Water Ready";
      blynk_LCD2 = "Press the Button";

      if ( temperature < mashTemp - 2) {
        heater1 = HIGH;
      }
      else if ( temperature > mashTemp + 2 ) {
        heater1 = LOW;
      }

      if (brewStage == 2 && switchState == HIGH) {
        brewStage = 3;
        terminal.println("THE MASH STARTS NOW. STIR EVERY FEW MINUTES TO ENSURE EVEN TEMPERATURE DISTRIBUTION. MAKE SURE NO GRAINS ARE CLOGGED TOGETHER ");

        terminal.flush();
        minutesCounter = 0;
        //timeEstimate = 0;
      }
      break;

    case 3:
      if ( temperature < mashTemp) {
        heater1 = HIGH;
      }
      else if (temperature > mashTemp) {
        heater1 = LOW;
      }

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("MASHING");
      display.println();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println(minutesCounter);
      display.println(temperature);
      display.print(hours);
      display.print(":");
      display.print(minutes);
      display.print(":");
      display.println(seconds);
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "Mashing the Grains";
      blynk_LCD2 = "Stir Occasionally.";

      //timeEstimate = mashTime - minutesCounter;

      if (minutesCounter > mashTime) {
        brewStage = 4;
        terminal.println("YOU NEED TO REMOVE THE MASH BAG NOW, DO THIS CAREFULLY, IT IS HOT. HANG IT OVER THE MASH TUN AS LONG AS POSSIBLE OR DUMP IT IN A FERMENTER. WHAT LEAKS IS CALLED THE SPARGE. ADD THIS TO THE WORT.");
        terminal.flush();
        Blynk.email("guy.leonard@gmail.com", "AUTOBREW: MASH DONE", "The Mash is done, remove the mash bag now.");
        Blynk.notify(String("AUTOBREW: Remove the Mash Bag"));
        minutesCounter = 0;

        switchState = 0;
        //timeEstimate = 0;
      }
      break;

    case 4:
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("REMOVE BAG");
      display.println("MASH DONE ");
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println("  Press   ");
      display.println(" BLUE BTN ");
      display.println(" To BOIL  ");
      display.display();
      delay(5);
      display.clearDisplay();

      heater1 = HIGH;
      heater2 = HIGH;

      blynk_LCD1 = "Remove Mash Bag";
      blynk_LCD2 = "Press Button for Boil";

      if (switchState == HIGH) {
        brewStage = 5;
        switchState = 0;
        terminal.println("HEATING THE WATER TO BOILING POINT");
        Blynk.notify(String("AUTOBREW: Heating to Boil Temps"));
        //timeTemp = (mashTemp - temperature) / ((KiloWatt * 3412) / (liters * 4) / 60);
        //timeEstimate = boilTime - minutesCounter;
        //find this formula and a great calculator at https://elementsofheating.wordpress.com/2012/09/26/how-to-calculate-the-kw-required-to-heat-a-volume-of-water-in-a-particular-time/

        terminal.flush();
        switchState = 0;
      }
      break;
      
    case 5:
      heater1 = HIGH;
      heater2 = HIGH;

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("HEATING");
      display.println();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println(minutesCounter);
      display.println(temperature);
      display.print(hours);
      display.print(":");
      display.print(minutes);
      display.print(":");
      display.println(seconds);
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "HEATING";
      blynk_LCD2 = "Boil Water";

      //timeTemp = (boilTemp - temperature) / ((KiloWatt * 3412) / (liters * 4) / 60);
      //timeEstimate = timeTemp - minutesCounter;
      //find this formula and a great calculator at https://elementsofheating.wordpress.com/2012/09/26/how-to-calculate-the-KiloWatt-required-to-heat-a-volume-of-water-in-a-particular-time/

      if (temperature > (boilTemp)) {

        brewStage = 6;
        terminal.println("THE BOIL HAS STARTED, NOTICE THAT YOU NEED TO ADD THE HOPS AT VARIOUS POINTS DURING THE BOIL.");
        terminal.flush();
        Blynk.notify(String("AUTOBREW: The Boil Has Started"));
        minutesCounter = 0;
        //timeEstimate = 0;
      }
      break;
      
    case 6:
      heater1 = HIGH;
      heater2 = HIGH;

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("BOILING");
      display.println();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println(minutesCounter);
      display.println(temperature);
      display.print(hours);
      display.print(":");
      display.print(minutes);
      display.print(":");
      display.println(seconds);
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "Boiling";
      blynk_LCD2 = "The Wort";

      //timeEstimate = boilTime - minutesCounter;

      if (minutesCounter >= HopTime1 && hop == 1) {

        terminal.println("ADD HOP 1");
        terminal.flush();
        Blynk.notify(String("AUTOBREEW: ADD HOP 1"));
        hop = 2;
      }

      if (minutesCounter >= HopTime2 && hop == 2) {

        terminal.println("ADD HOP 2");
        terminal.flush();
        Blynk.notify(String("AUTOBREW: ADD HOP 2"));
        hop = 3;
      }

      if (minutesCounter >= HopTime3 && hop == 3) {

        terminal.println("ADD HOP 3");
        terminal.flush();
        Blynk.notify(String("AUTOBREW: ADD HOP 3"));
        hop = 4;
      }

      if (minutesCounter >= HopTime4 && hop == 4) {

        terminal.println("ADD HOP 4");
        terminal.flush();
        Blynk.notify(String("AUTOBREW: ADD HOP 4"));
        hop = 5;
      }

      if (minutesCounter > boilTime) {

        heater1 = LOW;
        heater2 = LOW;

        brewStage = 7;
        terminal.println("THE BOIL IS FINISHED, START COOLING");
        terminal.flush();
        Blynk.email("guy.leonard@gmail.com", "AUTOBREW: Boil Finished", "Time for cooling.");
        Blynk.notify(String("AUTOBREW: The boil has finished, start cooling now!"));

        minutesCounter = 0;
        //timeEstimate = 0;
      }
      break;
      
    case 7:
      IceFunction(); //Call the function that calculates how much ice is needed.

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("  ADD ICE ");
      display.println();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println(minutesCounter);
      display.println(temperature);
      display.print ("Add ");
      display.print(iceNeeded);
      display.println(" L ice");
      display.print(hours);
      display.print(":");
      display.print(minutes);
      display.print(":");
      display.println(seconds);
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "Boil Finished";
      blynk_LCD2 = "Start Cooling";

      if (temperature <= (yeastPitchTemperature + 10)) {

        brewStage = 8;
        terminal.println("YOU CAN NOW MOVE THE WORT TO THE FERMENTER AND PITCH THE YEAST, BUT STIR THE WORT FIRST TO GET OXYGEN AND TO ENSURE EVEN TEMPERATURE. DOUBLE CHECK THE TEMPERATURE MANUALLY IN THE FERMENTER AND TRANSFER THE TEMP SENSOR TO THE FERMENTER BEFORE PITCHING");
        terminal.flush();
        Blynk.email("guy.leonard@gmail.com", "AUTOBRW: Wort Ready", "You can now move the wort to the fermenter and pitch the yeast.");
        Blynk.notify(String("AUTOBREW: The Wort is Ready"));

        minutesCounter = 0;
        //timeEstimate = 0;
      }
      break;

    case 8:
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println(" FERMENTING");
      display.println();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println("days:");
      display.println(days);
      display.println(temperature);
      display.print(hours);
      display.print(":");
      display.print(minutes);
      display.print(":");
      display.println(seconds);
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = " Yeast added";
      blynk_LCD2 = "Now Fermenting";

      //timeEstimate =
      //Timeprimary - days;

      if ((fermentTimeprimary - days) <= 0) {
        terminal.println("PRIMARY FERMENTATION DONE, MOVE TO SECONDARY OR BOTTLE. ADD SUGAR OR CO2 BEFORE BOTTLEING");
        terminal.flush();
        Blynk.email("guy.leonard@gmail.com", "AUTOBREW: Brew Ready", "The fermentation time is over, the brew is ready.");
        brewStage = 9;
      }
      break;
    case 9:

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println(" FINISHED");
      display.println();
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.println("days:");
      display.println(days);
      display.println(temperature);
      display.print(hours);
      display.print(":");
      display.print(minutes);
      display.print(":");
      display.println(seconds);
      display.display();
      delay(5);
      display.clearDisplay();

      blynk_LCD1 = "BEER IS READY";
      blynk_LCD2 = "ADD SUGAR OR CO2";
      break;
    case 10:
      //What to do when Brewstage equals 10
      break;
    case 11:
      //What to do when Brewstage equals 11
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
}// end void loop
