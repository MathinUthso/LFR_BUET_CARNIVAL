#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <EEPROM.h>

SSD1306AsciiWire oled;

// //==========================================================================================================
// // ADC buffer and control variables
// volatile uint16_t adc_buffer[6];
// volatile uint8_t current_channel = 0;
// volatile bool adc_complete = false;

// // Channel mapping for A0–A3, A6–A7
// const uint8_t channel_map[6] = {0, 1, 2, 3, 6, 7};

// void setupADC() {
//   // Disable digital input on analog pins to reduce noise
//   DIDR0 = (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D) | (1 << ADC3D);

//   // Configure ADC: prescaler = 16 (~1 MHz ADC clock)
//   ADCSRA = (1 << ADEN)  // Enable ADC
//          | (1 << ADPS2); // Prescaler = 16 (ADPS2 = 1, ADPS1 = 0, ADPS0 = 0)
//   ADCSRA |= (1 << ADATE); // Enable auto-trigger (free-running)
//   ADCSRA |= (1 << ADIE);  // Enable ADC interrupt
//   ADMUX = (1 << REFS0);   // AVcc reference, start with channel 0
//   ADCSRA |= (1 << ADSC);  // Start first conversion
// }

// ISR(ADC_vect) {
//   adc_buffer[current_channel] = ADC; // Store result
//   current_channel = (current_channel + 1) % 6;
//   // Set next channel (A0–A3, A6–A7)
//   ADMUX = (1 << REFS0) | channel_map[current_channel];
//   if (current_channel == 0) {
//     adc_complete = true; // Signal completion of all 6 reads
//   }
// }
//========================================================================================

//================ Motor Pin Definitions ==================
#define rms 11   // Right motor speed (PWM)
#define lms 3    // Left motor speed (PWM)
#define RmP 7    // Right motor positive
#define RmN 8    // Right motor negative
#define LmP 4    // Left motor positive
#define LmN 5    // Left motor negative

// Enhanced stopping mechanism
int lastLeftSpeed;
int lastRightSpeed;

//================ Inverse Line Mode =====================
int inverseMode = 0;    // 1 = black line, 0 = white line
int iModeDelay;

//================ Maze Solving ==========================
char hand; 
byte haat;       // 'l' = left-hand rule, 'r' = right-hand rule
char cross = 's';        // State of cross/intersection
unsigned int junctionDelay;           // Delay for T-section traversal
unsigned int nodeDelay;           // Delay for T-section traversal
unsigned int stopTimer;           // Delay after black box detection
unsigned int uTurnDelay;          // Delay for dead-end handling
unsigned int breakTime;
unsigned int turnReset;

//================ Timer ================================
uint32_t m1, m2,m3;

//================ Line Following Sensors ===============
int s[6];                 // Sensor array
int sum;
int threshold = 450;       //depreciated 
double lerorr = 30.5;     // Left error adjustment
double rerorr = 0.5;      // Right error adjustment

//================ Line Follow Variables ================
int lineProp = 3;
int pos;
int leftSpeed = 15 - lerorr;
int rightSpeed = 15 - rerorr;
int base[] = { 1, 2, 4, 8, 16, 32 };
char turn = 's';

//================ PID Control Variables ================
int PID;
int rightMotor, leftMotor;
int kp, kd, ki;
int rBase, lBase;
int erorr, last_erorr;
int baseSpeed;
int turnSpeed;
int position[] = { 10, 20, 30, 40, 50, 60 };
int avg;
int Sensor;
int error;               // Motor balancing error

//================ Buttons and LED =======================
#define INCREMENT_BUTTON 10
#define DECREMENT_BUTTON 12
#define SELECT_BUTTON    9
#define LED              13

int counter, obstacle_distance, wall_distance,turn90_delay,turnBrake;

//================ Auto Calibration ======================
int maxVal[6], minVal[6], tHold[6];

//=============== EEPROM Refresh Function ===============
void eeprom_refresh() {
  for (byte i = 0; i < 6; i++) {
    tHold[i] = EEPROM.read(i) * 4;
    maxVal[i] = EEPROM.read(i + 6) * 4;
    minVal[i] = EEPROM.read(i + 12) * 4;
  }
  //=====================================
  counter     = EEPROM.read(19);
  baseSpeed   = EEPROM.read(21);
  error       = EEPROM.read(22) - 30;
  turnSpeed   = EEPROM.read(23);
  //=====================================
  junctionDelay   = EEPROM.read(24);
  stopTimer   = EEPROM.read(25)*10;
  uTurnDelay  = EEPROM.read(26)*10;
  iModeDelay  = EEPROM.read(27);
  breakTime   = EEPROM.read(28)*10;
  turn90_delay= EEPROM.read(29)*10;
  turnBrake   = EEPROM.read(30);
  nodeDelay   = EEPROM.read(31)*10;
  turnReset   = EEPROM.read(32)*100;
  //================================
  kp          = EEPROM.read(101);
  kd          = EEPROM.read(102);
  ki          = EEPROM.read(103);
  haat    = EEPROM.read(104);
  //===============================
   Serial.println(haat);
  Serial.println("Haat marr");
   Serial.println(turnReset);
  // Serial.println(uTurnDelay);
  // Serial.println(iModeDelay);

 

  (haat==1)?hand='r':hand='l';
  Serial.println(hand);
  Serial.println("=====");

  rBase = (error > 0) ? baseSpeed - rerorr : baseSpeed;
  lBase = (error < 0) ? baseSpeed - lerorr : baseSpeed;
}

//=============== Setup Function ========================
void setup() {
  Serial.begin(9600);
  //setupADC();
  pinMode(INCREMENT_BUTTON, INPUT_PULLUP);
  pinMode(DECREMENT_BUTTON, INPUT_PULLUP);
  pinMode(SELECT_BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(System5x7);

  eeprom_refresh();
  TeamOne();  // Initial state or startup routine
}

//=============== Main Loop =============================
void loop() {
  byte r = push(SELECT_BUTTON);
  if (r) {
    if (r == 1) {
      r = menu();
      if (r) {
        if      (r == 1) counter_adjust();
        else if (r == 2) adjustment_panel();
        else if (r == 3) analog_display();
        else if (r == 4) digital_display();
        else if (r == 5) calibration_display();
        else if (r == 6) PID_display();
        else if (r == 7) SimplePID_display();
        else if (r == 8) PIDAdjust_panel();
        else if (r == 9) PID_display2();
        else if (r == 10) motor(rBase, lBase);
        else if (r == 11) turn90('l');
        else if (r == 12) turn90('r');
        else if (r == 13) motor(-rBase,-lBase);
      }
    }
    TeamOne();  // Loop back to main control after menu exit
  }
}
