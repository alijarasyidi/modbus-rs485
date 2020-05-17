//////////////////////////////////////
//  PEMBACAAN SENSOR tEKANAN UDARA  //
//////////////////////////////////////
long sentP_psi = 0;
void readPressureSensor(){
  int sensorVal=analogRead(A1);
  float voltage = (sensorVal*5.0)/1024.0;
  float pressure_pascal = (3.0*((float)voltage-0.475))*1000000.0;                       //calibrate here
  float pressure_bar = pressure_pascal/10e5;
  float pressure_psi = pressure_bar*14.5038;
  sentP_psi = pressure_psi*1000;
}

//////////////////////////////////////
// PEMBACAAN SENSOR ROTARY ENCODER  //
//////////////////////////////////////
//encoder
#define enc1A 2 
#define enc1B 3 
void ISR_INT0(){
  int pinA,pinB;
  pinA=digitalRead(enc1A);
  pinB=digitalRead(enc1B);
  if(pinA==LOW && pinB==LOW){
    pos1--; // CCW
  } else if(pinA==LOW && pinB==HIGH){
    pos1++;
  }else if(pinA==HIGH&& pinB==LOW){
    pos1++; // CCW
  } else if(pinA==HIGH && pinB==HIGH){
    pos1--; // CCW
  }
}
void ISR_INT1(){
  int pinA,pinB;
  pinA=digitalRead(enc1A);
  pinB=digitalRead(enc1B);
  if(pinA==LOW && pinB==LOW){
    pos1++; // CCW
  } else if(pinA==LOW && pinB==HIGH){
    pos1--;
  } else if(pinA==HIGH&& pinB==LOW){
    pos1--; // CCW
  } else if(pinA==HIGH && pinB==HIGH){
    pos1++; // CCW
  }
}

//////////////////////////////////////
//               MOTOR              //
//////////////////////////////////////
//motor driver BTS7960
void motor_power(int pwm, int fwd, int rev, int power){
  int pwm_value;
  pwm_value=abs(power);
  if (pwm_value>255)
    pwm_value=255;
  if(power>0){
    digitalWrite(rev,LOW); // turn off
    digitalWrite(fwd,HIGH); // turn off
    analogWrite(pwm, pwm_value);
  } else if(power<0){
    digitalWrite(rev,HIGH); // turn off
    digitalWrite(fwd,LOW); // turn off
    analogWrite(pwm, pwm_value);
  }else{
    digitalWrite(fwd,LOW); // turn off
    digitalWrite(rev,LOW); // turn off
    analogWrite(pwm, 0);
  }
}


//////////////////////////////////////
//PEMBACAAN INPUT DIGITAL DAN ANALOG//
//////////////////////////////////////
//digital pin
#define din1 7
#define din2 8
#define din3 9
#define din4 13
//analog pin
#define ain1 9 
#define ain2 13

int input[6];
void readInput(){
  input[0] = digitalRead(din1);
  input[1] = digitalRead(din2);
  input[2] = digitalRead(din3);
  input[3] = digitalRead(din4);
  input[4] = digitalRead(ain1);
  input[5] = digitalRead(ain2);
}
