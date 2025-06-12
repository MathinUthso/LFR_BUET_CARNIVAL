// void reading() {
//   Sensor = 0; //this is to refresh initial value
//   sum = 0;
//   for (int i = 0 ; i < 6 ; i++) {
//      (i < 4) ? s[i] = analogRead(i) : s[i] = analogRead(i + 2);
//     (s[i] > tHold[i]) ? s[i] = 1 : s[i] = 0; //conditional statement. this is to convert analog value to digital. if you want to see real analog value, then comment it. but for line follow, you must uncomment it.
//     Sensor += s[i] * base[i]; //this is to merge all 6 values and imagine them in a single binary number. then i converted it into decimal number to use as final value. better search about base convertion
//     sum += s[i]; //finds out total count of sensor
//   }
// }
void PID_reading() {
  sum = 0;
  Sensor = 0;
  // adc_complete = false;
  // // Ensure ADC is running
  // ADCSRA |= (1 << ADSC);
  // // Wait for all 6 conversions (~48 µs)
  // //while (!adc_complete);

  for (int i = 0; i < 6; i++) {

    //s[i] = adc_buffer[i];
    (i < 4) ? s[i] = analogRead(i) : s[i] = analogRead(i + 2);

    // Convert to digital
    s[i] = (s[i] > tHold[i]) ? 1^inverseMode : 0^inverseMode;

    // Calculate position
    Sensor += s[i] * position[i];
    sum += s[i];
  }

  (sum== 0)? avg=35: avg = Sensor / sum;
}


void analog_display() {
  oled.clear();
  oled.set2X();
  while (!push(SELECT_BUTTON)) {
    for (int i = 0 ; i < 6 ; i++)
      (i < 4) ? s[i] = analogRead(i) : s[i] = analogRead(i + 2);
    for (byte i = 0; i < 3; i++) {
      if (s[i + 3] < 10) text(String(s[i + 3]) + "   ", 10, i * 3);
      else if (s[i + 3] < 100) text(String(s[i + 3]) + "  ", 10, i * 3);
      else if (s[i + 3] < 1000) text(String(s[i + 3]) + " ", 10, i * 3);
      else text(String(s[i + 3]), 10, i * 3);
      text(String(s[2 - i]) + "   ", 70, i * 3);
    }
  }
  oled.set1X();
}

void digital_display() {
  oled.clear();
  oled.set2X();
  text("L", 24, 4);
  text("F", 59, 4);
  text("R", 94, 4);
  

  while (!push(SELECT_BUTTON)) {
    //reading();
    PID_reading();
    text("Sum: ",24,6);
  text(String(sum),70,6);
    for (byte i = 0; i < 6; i++){
      text(String(s[5 - i]), 1 + i * 23, 1);}
    // (sl < 10) ? text(" " + String(sl) + " ", 12, 6) : text(String(sl), 13, 6);
    // (sf < 10) ? text(" " + String(sf) + " ", 47, 6) : text(String(sf), 48, 6);
    // (sr < 10) ? text(" " + String(sr) + " ", 82, 6) : text(String(sr), 83, 6);
  }
  oled.set1X();
}

void calibration_display() {
  oled.clear();
  oled.set2X();
  text(" STARTING ", 04, 2);
  oled.set1X();
  for (byte i = 0; i < 42; i++) {
    text(".", i * 3, 4);
    delay(25);
    if (i >= 41) cal();
    if (push(SELECT_BUTTON)) break;
  }

  oled.clear();
  text("MIN", 0, 0);
  text("MID", 0, 3);
  text("MAX", 0, 6);
  for (byte i = 0; i < 3; i++) {
    text(String(minVal[5 - i]), 30 + i * 37, 0);
    text(String(minVal[i]), 30 + i * 37, 1);
    text(String(tHold[5 - i]), 30 + i * 37, 3);
    text(String(tHold[i]), 30 + i * 37, 4);
    text(String(maxVal[5 - i]), 30 + i * 37, 6);
    text(String(maxVal[i]), 30 + i * 37, 7);
  }
  while (!push(SELECT_BUTTON));
}

void cal() {
   // Initialize min and max sensor values
  for (byte i = 0; i < 6; i++) {
    minVal[i] = 1023;
    maxVal[i] = 0;
  }

  //forward, backward, forward, backward
  for (byte movement = 0; movement < 4; movement++) {
    if (movement % 2 == 0)motor(50, 50);
    else motor(-50, -50);
  

  uint32_t moveStartTime = millis();

  while (millis() - moveStartTime < 500) {  // Move for 500ms
    // Read sensors while moving
    for (int j = 0; j < 6; j++) {
      s[j] = (j < 4) ? analogRead(j) : analogRead(j + 2);
      maxVal[j] = max(maxVal[j], s[j]);
      minVal[j] = min(minVal[j], s[j]);
    }
    //delay(10);  // Small delay for smoother sensor reading
  }

  motor(0, 0);  // Brief stop after each movement
  delay(100);   // Pause to stabilize before the next movement
}

motor(0, 0);  // Ensure motors are stopped after calibration

// ============ Calculate Threshold ============
for (byte i = 0; i < 6; i++) {
  tHold[i] = (maxVal[i] - minVal[i]) * 0.5 + minVal[i];
  EEPROM.update(i, tHold[i] / 4);delay(10);
  EEPROM.update(i + 6, maxVal[i] / 4);delay(10);
  EEPROM.update(i + 12, minVal[i] / 4);delay(10);
}
return;
}