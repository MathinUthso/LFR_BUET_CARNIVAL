void motor(int a, int b) {  //75 225
  lastRightSpeed = a;
  lastLeftSpeed = b;
  if (a > 0) {
    digitalWrite(RmP, 1);
    digitalWrite(RmN, 0);
  } else {
    a = -a;
    digitalWrite(RmP, 0);
    digitalWrite(RmN, 1);
  }

  if (b > 0) {
    digitalWrite(LmP, 1);
    digitalWrite(LmN, 0);
  } else {
    b = -b;
    digitalWrite(LmP, 0);
    digitalWrite(LmN, 1);
  }
  if (a > 250) a = 250;
  if (b > 250) b = 250;

  analogWrite(rms, a);  //sets the pwm pins
  analogWrite(lms, b);  //sets the speed
}