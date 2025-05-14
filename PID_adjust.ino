String PID_adjust(byte a) {
  if (a == 1)       return "Kp            ";
  else if (a == 2)  return "Kd            ";
  else if (a == 3)  return "Ki            ";
  else if (a == 4)  return "menu 4        ";
  else if (a == 5)  return "menu 5        ";
  else if (a == 6)  return "menu 6        ";
  else if (a == 7)  return "menu 7        ";
  else if (a == 8)  return "menu 8        ";
  else if (a == 9)  return "menu nine     ";
  else if (a == 10) return "menu ten      ";
  else if (a == 11) return "menu eleven   ";
  else if (a == 12) return "menu twelve   ";
}

void PIDAdjust_panel() {
  oled.set1X();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 12;
  int value;
  while (1) {
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8 ; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        value = EEPROM.read(i + base + 100);
       // if (i + base == 2) text(String(i + base) + ") " + PID_adjust(i + base) + String(value - 25), 0, i);
       // else 
       text(String(i + base) + ") " + PID_adjust(i + base) + String(value), 0, i);
      }
    }

    byte inc = push(INCREMENT_BUTTON);
    byte dec = push(DECREMENT_BUTTON);

    if (inc == 1) { // Short press increment
      ind++;
      if (ind > limit) {
        ind = 1;
        base = ind;
        peak = base + 7;
      }
      else if (ind > peak) {
        peak = ind;
        base = peak - 7;
      }
    }
    else if (inc == 2) { // Long press increment 
      ind += 3; 
      if (ind > limit) ind = limit;
      if (ind > peak) {
        peak = ind;
        base = peak - 7;
      }
    }

    if (dec == 1) { // Short press decrement
      ind--;
      if (ind < 1) {
        ind = limit;
        peak = ind;
        base = peak - 7;
      }
      else if (ind < base) {
        base = ind;
        peak = base + 7;
      }
    }
    else if (dec == 2) { // Long press decrement 
      ind -= 3; 
      if (ind < 1) ind = 1;
      if (ind < base) {
        base = ind;
        peak = base + 7;
      }
    }

    byte r = push(SELECT_BUTTON);
    if (r) {
      if (r == 1) PID_Valadjust(ind);
      else {
        eeprom_refresh();
        return;
      }
      temp = 0;
    }
  }
}

void PID_Valadjust(int indicator) {
  oled.set2X();
  oled.clear();
  int value;
  value = EEPROM.read(indicator + 100);

  byte temp = 0, limit = 255;
  while (1) {
    if (temp != value) {
      temp = value;
     // if (indicator == 2) text("SET: " + String(value+25) + "    ", 5, 3);
      //else
       text("SET: " + String(value) + "    ", 5, 3);
    }

    byte inc = push(INCREMENT_BUTTON);
    byte dec = push(DECREMENT_BUTTON);

    if (inc == 1) { // Short press increment
      value++;
      if (value > limit) value = 0;
    }
    else if (inc == 2) { // Long press increment
      value += 10; // Increase by 10 
      if (value > limit) value = 0;
    }

    if (dec == 1) { // Short press decrement
      value--;
      if (value < 0) value = limit;
    }
    else if (dec == 2) { // Long press decrement 
      value -= 10; // Decrease by 10
      if (value < 0) value = limit;
    }

    byte r = push(SELECT_BUTTON);
    if (r) {
      if (r == 1) {
        EEPROM.update(100 + indicator, value);
        delay(10);
        oled.set1X();
        return;
      }
      else return;
    }
  }
}

