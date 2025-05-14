void PID_display() {
  oled.clear();
  oled.set2X();
  text("P", 24, 4);
  text("I", 59, 4);
  text("D", 94, 4);
  while (!push(SELECT_BUTTON)) {
    //reading();
    PID_Follow();
    //digitalWrite(LED,HIGH);
  }
  motor(-lastRightSpeed, -lastLeftSpeed);
  motor(0,0);
  oled.set1X();
}

void SimplePID_display() {
  oled.clear();
  oled.set2X();
  text("Simple",30,1);
  text("P", 24, 4);
  text("I", 59, 4);
  text("D", 94, 4);
  while (!push(SELECT_BUTTON)) {
    //reading();
    simplePID();
  }
  motor(-lastRightSpeed, -lastLeftSpeed);
  motor(0,0);
  oled.set1X();
}