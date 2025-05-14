String menu_list(byte a) {
  if (a == 1)       return "Counter           ";
  else if (a == 2)  return "Adjustment        ";
  else if (a == 3)  return "Analog Display    ";
  else if (a == 4)  return "Digital Display   ";
  else if (a == 5)  return "Calibration       ";
  else if (a == 6)  return "PID with Inverse  ";
  else if (a == 7)  return "Simple PID        ";
  else if (a == 8)  return "PID adjustment    ";
  else if (a == 9)  return "menu nine         ";
  else if (a == 10) return "menu ten          ";
  else if (a == 11) return "menu eleven       ";
  else if (a == 12) return "Straight          ";
}

byte menu() {
  oled.set1X();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 12;
  while (1) {
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8 ; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        text(String(i + base) + ") " + menu_list(i + base), 0, i);
      }
    }

    if (push(INCREMENT_BUTTON) == 1) {
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

    if (push(DECREMENT_BUTTON) == 1) {
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

    byte r = push(SELECT_BUTTON);
    if (r) {
      if (r == 1) return ind;
      else return 0;
    }
  }
}
