void PID_display() {
  oled.clear();
  oled.set2X();
  text("P", 24, 4);
  text("I", 59, 4);
  text("D", 94, 4);
  while (1)PID_Follow();
      }

void SimplePID_display() {
  oled.clear();
  oled.set2X();
  text("Simple",30,1);
  text("P", 24, 4);
  text("I", 59, 4);
  text("D", 94, 4);
  while (!push(SELECT_BUTTON))simplePID();
    //reading();
  motor(-lastRightSpeed, -lastLeftSpeed);
  motor(0,0);
  oled.set1X();
}

void PID_display2() {
  oled.clear();
  oled.set2X();
  text("P", 24, 4);
  text("I", 59, 4);
  text("2", 94, 4);
  if (push(SELECT_BUTTON))PID_Follow2();
      }