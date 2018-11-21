/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/
#include <Wire.h>
#include <SeeedOLED.h>
#include <PID_v1.h>



//Define Variables we'll be connecting to
double Setpoint_A, Input_A, Output_A, Setpoint_B, Input_B, Output_B;

//Specify the links and initial tuning parameters
double Kp=0.01, Ki=0.01, Kd=0;
PID PID_A(&Input_A, &Output_A, &Setpoint_A, Kp, Ki, Kd, DIRECT);

long unsigned int counter_A = 0;
bool   prev_state_A = false;
long unsigned int prev_millis = 0;

void Motor_A_counter(){
  bool state_A = digitalRead(12);
  if(state_A != prev_state_A){counter_A++; prev_state_A = state_A;}
}

PID PID_B(&Input_B, &Output_B, &Setpoint_B, Kp, Ki, Kd, DIRECT);

long unsigned int counter_B = 0;
bool   prev_state_B = false;


void Motor_B_counter(){
  bool state_B = digitalRead(5);
  if(state_B != prev_state_B){counter_B++; prev_state_B = state_B;}
}

void setup()
{
  //initialize the variables we're linked to
  Input_A = 1500;
  Setpoint_A = 1500;
  PID_A.SetMode(AUTOMATIC);
  PID_A.SetSampleTime(1000);
  PID_A.SetOutputLimits(9,150);
  
  Input_B = 1500;
  Setpoint_B = 1500;
  PID_B.SetMode(AUTOMATIC);
  PID_B.SetSampleTime(1000);
  PID_B.SetOutputLimits(9,150);
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0); 
  SeeedOled.putString("  set read pow");
  SeeedOled.setTextXY(2,0); 
  SeeedOled.putString("A");
  SeeedOled.setTextXY(4,0); 
  SeeedOled.putString("B");
 
  analogWrite(10,0);
  analogWrite(11,0);
  
}



void loop()
{
  prev_millis = millis();
  counter_A = 0;
  counter_B = 0;
  while(prev_millis > millis() - 100){ //run sub loop for 100 ms
    Motor_A_counter();
    Motor_B_counter();
  }
  Input_A = int(round(float(counter_A *150)/100)*10);
  Input_B = int(round(float(counter_B *150)/100)*10);

//  get setpoint A
  Setpoint_A = int(round(float(analogRead(A1)*3)/100)*10);
  Setpoint_B = int(round(float(analogRead(A0)*3)/100)*10);
  PID_A.Compute();
  PID_B.Compute();
  analogWrite(10, Output_A);
  analogWrite(11, Output_B);

  SeeedOled.setTextXY(2,2);  
  SeeedOled.putString("     ");
  SeeedOled.setTextXY(2,2);
  SeeedOled.putNumber(Setpoint_A);
  SeeedOled.setTextXY(2,6);  
  SeeedOled.putString("     ");
  SeeedOled.setTextXY(2,6);
  SeeedOled.putNumber(Input_A);
  SeeedOled.setTextXY(2,11);  
  SeeedOled.putString("     ");
  SeeedOled.setTextXY(2,11);
  SeeedOled.putNumber(Output_A);
  
  SeeedOled.setTextXY(4,2);  
  SeeedOled.putString("     ");
  SeeedOled.setTextXY(4,2);
  SeeedOled.putNumber(Setpoint_B);
  SeeedOled.setTextXY(4,6);  
  SeeedOled.putString("     ");
  SeeedOled.setTextXY(4,6);
  SeeedOled.putNumber(Input_B);
  SeeedOled.setTextXY(4,11);  
  SeeedOled.putString("     ");
  SeeedOled.setTextXY(4,11);
  SeeedOled.putNumber(Output_B);
}


