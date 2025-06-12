void PID_Follow() {
while(true){
  a:
  PID_reading();  // Read the sensor values
if (sum == 0) {
    // Line completely lost
    if (turn != 's') {
      // Execute the saved turn direction to attempt recovery
      (turn == 'r') ? motor(-turnSpeed, turnSpeed) : motor(turnSpeed, -turnSpeed);
      while (!s[2] && !s[3]) PID_reading();  // Wait until central sensors detect line again
      turn = 's';cross = 's';avg=35;PID=0;last_erorr=0;
    } 
    else if (last_erorr>-15&&last_erorr<15) {
      m2 = millis();
      while (sum == 0) {
          PID_reading();
        if ((millis() - m2) > uTurnDelay) {
          brake();
          motor(-turnSpeed,turnSpeed); delay(250); //this is added to account for the mechanical error in the first bot
          (hand == 'r') ? motor(-turnSpeed, turnSpeed) : motor(turnSpeed, -turnSpeed);
          while (!s[2] && !s[3]) PID_reading();
          avg = 35;
          turn='s';
          cross='s';
          PID=0;
          last_erorr=0;
          break;
        }
      }
    }
  }

else if (sum == 1 || sum == 2) {
    // If a cross was detected earlier, complete the turn
    if (cross != 's') {
      brake();
      (cross == 'r') ? motor(-turnSpeed, turnSpeed) : motor(turnSpeed, -turnSpeed);
      while ((s[4] + s[3] + s[2] + s[1]) > 0) PID_reading();  // Exit current node
      while (!s[2] && !s[3]) PID_reading();  // Wait until center sensors see line
      cross = 's';
      turn = 's';
      avg = 35;
      last_erorr=0;
      PID=0;
    }

    //PID_reading();  // Update sensor values again
    erorr = 35 - avg;  // Calculate position error
    PID = kp * erorr + (last_erorr - erorr) * kd;  // PID control calculation
    last_erorr = erorr;

    rightMotor = rBase + PID;  // Adjust right motor speed
    leftMotor  = lBase - PID;  // Adjust left motor speed

    //  Serial.print(leftMotor);
    //  Serial.println(rightMotor);

    //=====================
    // setting the speed
    motor(rightMotor, leftMotor);  // Move the robot forward with calculated speeds
    //=====================
  }

else if (sum == 3 || sum==4 || sum == 5) {
    // iMode();
    // Code for Inverse line
    // ===========================================
    // check if the code works with function call only if it works in the current state
    if ((s[0] && s[5]) && sum < 6) {  // detection of iLine
      uint32_t m = millis();

      while ((s[0] || s[5]) && (sum < 6)) {
        PID_reading();

        if (millis() - m > iModeDelay) {  // Making the bot work in Y section too
          inverseMode = !inverseMode;  // Toggle inverse mode
          cross = 's';
          break;
        }
      }
      turn = hand;
    }
    // Code for inverse line ends here
    // =============================================
     //==========================================================================================
    // Detect left turn
    //===========================================================================================
    if ((s[0]||s[1]) && !s[5] && s[2] + s[3] > 0) {
      turn = 'l';
      while (!s[5] && (s[0]||s[1])) PID_reading();  // Wait for rightmost sensor to detect line
      if (!s[5]) {
        delay(nodeDelay);  // Move forward slightly into the node
        PID_reading();
        if (sum != 0)if (hand == 'l') cross = 'l';  // Register left cross
      }
    }
   //===========================================================================================
    // Detect right turn
    //==========================================================================================
    else if (!s[0] && (s[5]||s[4]) && s[2] + s[3] > 0) {
      turn = 'r';
      while (s[5] && !s[0]) PID_reading();  // Wait for leftmost sensor to detect line
      if (!s[0]) {
        delay(nodeDelay);  // Move forward slightly into the node
        PID_reading();
        if (sum != 0)if (hand == 'r') cross = 'r';  // Register right cross
      }
    }
  //============================================================================================
    m1 = millis();  // Reset timer for idle detection
  }

else if (sum == 6) {
    // Dead end or solid block detection
    turn = hand;
    m2 = millis();

    while ((s[5]) && (s[0])) {
      PID_reading();
      if (millis() - m2 > stopTimer) {
        // Reached a confirmed dead-end
        brake();  // Stop
        while (sum == 6) PID_reading();  // Wait until sensors detect a valid path again
        goto a;  // Retry the loop from the beginning
      }
    }
    delay(junctionDelay);  // Move a little ahead
    PID_reading();
    if (sum != 0) cross = hand;  // Save direction
    m1 = millis();
  }
m3=millis();
  // Reset turn after 300ms of no updates
if ((m3 - m1) > turnReset) {
    turn = 's';
    // digitalWrite(LED,LOW);
  }
}
}
