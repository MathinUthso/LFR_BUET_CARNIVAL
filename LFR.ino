void simplePID() {
  PID_reading();
  if (sum == 0) {
    delay(50);
    if (turn != 's') {
      motor(turn == 'r' ? -turnSpeed : turnSpeed, turn == 'r' ? turnSpeed : -turnSpeed);
      while (!s[2] && !s[3]) PID_reading(); // Wait until a line is detected
      turn = 's';
    }
    return; // Exit early as no further action is needed
  }

  if (sum==1||sum==2) {
    PID_reading();
    erorr = 35 - avg; 
    PID = kp * erorr + (last_erorr - erorr) * kd;
    last_erorr = erorr;
    rightMotor = rBase + PID;  
    leftMotor = lBase - PID;   
                               //  Serial.print(leftMotor);
                               //  Serial.println(rightMotor);
    //=====================
    //setting the speed
    motor(rightMotor, leftMotor);
    //=====================
  }

 if(sum>=3&&sum<=5){
       if (s[0] && !s[5]) turn = 'l';
  if (!s[0] && s[5]) turn = 'r';
   if (s[0] && s[5] && sum < 6) {  //detection of iLine
      uint32_t m = millis();

      while (s[0] || s[5] && sum < 6) {
        PID_reading();
       

        if (millis() - m > iModeDelay) {  //Making the bot work in Y sectiomm too
          inverseMode = !inverseMode;
          cross = 's';
          break;
        }
      }
      turn = hand;
    }
 }

  if (sum == 6) {
    delay(50);
    PID_reading();
  if (sum == 6) {
      delay(100);
      motor(-lastRightSpeed,-lastLeftSpeed);
      motor(0, 0);
      while (sum == 6) PID_reading();
    } else if (sum == 0) turn = 'r';
  }
}