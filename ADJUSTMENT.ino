String adjustment_list(byte a) {
  if (a == 1)       return "Base Speed     ";
  else if (a == 2)  return "Motor Error    ";
  else if (a == 3)  return "Turn Speed     ";
  else if (a == 4)  return "junction Delay ";
  else if (a == 5)  return "Stop timer     ";
  else if (a == 6)  return "U Turn Delay   ";
  else if (a == 7)  return "Inverse Delay  ";
  else if (a == 8)  return "Brake time      ";
  else if (a == 9)  return "Turn90_Delay   ";
  else if (a == 10) return "Turn Brake     ";
  else if (a == 11) return "node delay     ";
  else if (a == 12) return "Turn reset     ";
}

void adjustment_panel() {
  oled.set1X();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 12;
  int value;
  while (1) {
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8 ; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        value = EEPROM.read(i + base + 20);
        if (i + base == 2) text(String(i + base) + ") " + adjustment_list(i + base) + String(value - 30), 0, i);
        else text(String(i + base) + ") " + adjustment_list(i + base) + String(value), 0, i);
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
      if (r == 1) set_adjust(ind);
      else {
        eeprom_refresh();
        return;
      }
      temp = 0;
    }
  }
}

void set_adjust(int indicator) {
  oled.set2X();
  oled.clear();
  int value;
  value = EEPROM.read(indicator + 20);

  byte temp = 0, limit = 255;
  if (indicator == 1) limit = 250;
  else if (indicator == 2) limit = 60;
  else if (indicator == 12) limit = 30;
  else if (indicator == 11) limit = 100;
  else if (indicator == 5) limit = 100;
  while (1) {
    if (temp != value) {
      temp = value;
      if (indicator == 2) text("SET: " + String(value+30) + "    ", 5, 3);
      else text("SET: " + String(value) + "    ", 5, 3);
    }

    byte inc = push(INCREMENT_BUTTON);
    byte dec = push(DECREMENT_BUTTON);

    if (inc == 1) { // Short press increment
      value++;
      if (value > limit) value = limit;
    }
    else if (inc == 2) { // Long press increment
      value += 10; // Increase by 10 
      if (value > limit) value = limit;
    }

    if (dec == 1) { // Short press decrement
      value--;
      //if (value < 0) value = 0;
    }
    else if (dec == 2) { // Long press decrement 
      value -= 10; // Decrease by 10
      //if (value < 0) value = 0;
    }

    byte r = push(SELECT_BUTTON);
    if (r) {
      if (r == 1) {
        EEPROM.update(20 + indicator, value);
        delay(10);
        oled.set1X();
        return;
      }
      else return;
    }
  }
}

void counter_adjust() {
  oled.set2X();
  oled.clear();
  counter = EEPROM.read(19);
  byte temp = -1, limit = 25;
  while (1) {
    if (temp != counter) {
      temp = counter;
      text("SET: " + String(counter) + "    ", 5, 3);
    }

    byte inc = push(INCREMENT_BUTTON);
    byte dec = push(DECREMENT_BUTTON);

    if (inc == 1) { // Short press increment
      counter++;
      if (counter > limit) counter = limit;
    }
    else if (inc == 2) { // Long press increment 
      counter += 3; 
      if (counter > limit) counter = limit;
    }

    if (dec == 1) { // Short press decrement
      counter--;
      if (counter < 0) counter = 0;
    }
    else if (dec == 2) { // Long press decrement 
      counter -= 3;
      if (counter < 0) counter = 0;
    }

    byte r = push(SELECT_BUTTON);
    if (r) {
      if (r == 1) {
        EEPROM.write(19, counter);
        delay(10);
        oled.set1X();
        return;
      }
      else {
        counter = EEPROM.read(19);
        return;
      }
    }
  }
}