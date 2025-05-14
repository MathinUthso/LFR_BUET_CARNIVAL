#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <EEPROM.h>
SSD1306AsciiWire oled;

//Motor Pins
#define rms 11  //pwmA->rightMotorSpeed
#define lms 3   //pwmB ->leftMotorSpeed
#define RmP 7   //Postive end of Motor1
#define RmN 8   //Negative end of Motor1
#define LmP 4   //Postive end of Motor2
#define LmN 5   //Negative end of Motor

//Enhanced stopping mechanism
int lastLeftSpeed;
int lastRightSpeed;


//================================================
//   For inverse line
  int inverseMode=0;
  //inverseMode=1 for Black line and ==0 for white line
  int iModeDelay;
//================================================

//Maze solving
//Left Hand || Right Hand ( 'l' for left hand and 'r' for right hand)
char hand = 's';
char cross = 's';
int nodeDelay;  //adjust this delay so that upon detecting a T section the robot moves ahead atleast 90% of it body
int  stopTimer;   // adjust it so that atleast >50% of the robot's body goes beyond the the blackBox
int uTurnDelay;  //adjust this so that the bot goes atleast 130% ahead of its body from the deadend

//Timer
uint32_t m1, m2;



//Sensor reading variables
int s[6], sum, threshold = 450;  //[6],minValues[6],maxValues[6];
double lerorr = 30.5;
double rerorr = .5;


//line follow variables
int lineProp = 3;
int pos;
int leftSpeed = 15 - lerorr, rightSpeed = 15 - rerorr;
int base[] = { 1, 2, 4, 8, 16, 32 };
char turn = 's';


//==============================================
//PID variables
int PID;
int rightMotor, leftMotor;
int kp, kd, ki;
int rBase,lBase;
int erorr, last_erorr;
int baseSpeed;
int turnSpeed;
int position[] = { 10, 20, 30, 40, 50, 60 };
int avg;
int Sensor;
int error;
//=============================================
#define INCREMENT_BUTTON 10
#define DECREMENT_BUTTON 12
#define SELECT_BUTTON 9
#define LED 13



int counter, brake, obstacle_distance, wall_distance;
// int spl, spr;
//int sensor, sum;
//int s[6], base[6] = {1, 2, 4, 8, 16, 32};

//Auto calibration variables
int maxVal[6], minVal[6];
int tHold[6];

//int sf = 10, sl = 7, sr = 5;

void eeprom_refresh() {
  for (byte i = 0; i < 6; i++) {
    tHold[i] = EEPROM.read(i) * 4;
    maxVal[i] = EEPROM.read(i + 6) * 4;
    minVal[i] = EEPROM.read(i + 12) * 4;
  }
  counter = EEPROM.read(19);
  baseSpeed = EEPROM.read(21);
  error = EEPROM.read(22) - 30;
  turnSpeed = EEPROM.read(23);
  nodeDelay = EEPROM.read(24);
  stopTimer = EEPROM.read(25);
  uTurnDelay = EEPROM.read(26);
  kp=EEPROM.read(101);
  kd=EEPROM.read(102);
  ki=EEPROM.read(103);
(error > 0) ? rBase = baseSpeed - rerorr : rBase = baseSpeed;
(error < 0) ? lBase = baseSpeed - lerorr : lBase = baseSpeed;
 
}

void setup() {
  pinMode(INCREMENT_BUTTON, INPUT_PULLUP);
  pinMode(DECREMENT_BUTTON, INPUT_PULLUP);
  pinMode(SELECT_BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(System5x7);
  eeprom_refresh();
  TeamOne();
}

void loop() {
  byte r = push(SELECT_BUTTON);
  if (r) {
    if (r == 1) {
      r = menu();
      if (r) {
        if (r == 1) counter_adjust();
        else if (r == 2) adjustment_panel();
        else if (r == 3) analog_display();
        else if (r == 4) digital_display();
        else if (r == 5) calibration_display();
        else if(r==6) PID_display();
        else if(r==7) SimplePID_display();
        else if(r==8) PIDAdjust_panel() ;
        else if(r==12) motor(rBase,lBase) ;
      }
    }
    TeamOne();
  }
}