void PID_Follow() {
  //if(push(SELECT_BUTTON )) break;
a:
  PID_reading();
  if (sum == 0) {
    if (turn != 's') {
      (turn == 'r') ? motor(-turnSpeed, turnSpeed) : motor(turnSpeed, -turnSpeed);
      while (!s[2] && !s[3]) PID_reading();
      turn = 's';
      cross = 's';
      erorr = 0;
    } 
    // else if (erorr > -20 && erorr < 20) {
    //   // digitalWrite(LED,HIGH);
    //   m2 = millis();
    //   while (sum == 0) {
    //     if (millis() - m2 > uTurnDelay) {
    //       motor(-lastRightSpeed, -lastLeftSpeed);
    //       motor(0, 0);
    //       delay(30);
    //       (hand == 'r') ? motor(-turnSpeed, turnSpeed) : motor(turnSpeed, -turnSpeed);
    //       while (!s[2] && !s[3]) PID_reading();
    //       erorr = 0;
    //       break;
    //     }
    //   }
    // }
  }

  else if (sum == 1 || sum == 2) {
    if (cross != 's') {
      (cross == 'r') ? motor(-turnSpeed, turnSpeed) : motor(turnSpeed, -turnSpeed);
      while (s[4] + s[3] + s[2] + s[1] > 0) PID_reading();
      while (s[2] == 0 && s[3] == 0) PID_reading();
      cross = 's';
      turn = 's';
      erorr = 0;
    }
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

  else if (sum >= 3 && sum <= 5) {
    //iMode();
    //Code for Inverse line
    //===========================================
    //check if the code works with function call only if it works in the current state
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
    //Code for inverse line ends here
    //=============================================
    if (s[0] && !s[5] && s[2] + s[3] > 0) {
      turn = 'l';
      while (!s[5] && s[0]) PID_reading();
      if (!s[5]) {
        delay(nodeDelay);
        PID_reading();
        if (sum != 0)
          if (hand == 'l') cross = 'l';
      }
    }
    if (!s[0] && s[5] && s[2] + s[3] > 0) {
      turn = 'r';
      while (s[5] && !s[0]) PID_reading();
      if (!s[0]) {
        delay(nodeDelay);
        PID_reading();
        if (sum != 0)
          if (hand == 'r') cross = 'r';
      }
    }
    m1 = millis();
  }

  else if (sum == 6) {
    turn = hand;
    m2 = millis();
    while (s[5] && s[1]) {
      PID_reading();
      if (millis() - m2 > stopTimer) {
        motor(-lastRightSpeed, -lastLeftSpeed);
        motor(0, 0);
        while (sum == 6) PID_reading();
        goto a;
      }
    }
    // digitalWrite(LED,HIGH);
    delay(nodeDelay);
    PID_reading();
    if (sum != 0) cross = hand;
    m1 = millis();
  }


  if (millis() - m1 > 500) {
    turn = 's';
    //digitalWrite(LED,LOW);
  }
}