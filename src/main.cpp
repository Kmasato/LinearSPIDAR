#include <Arduino.h>
#include <Encoder.h>

#define TIMER_WIDTH
#define COUNT_LOW 0
#define COUNT_HIGH 65535

int motor1a = 21; //モータ1正転
int motor1b = 22; //モータ1逆転
int enc1a = 25;
int enc1b = 26;

Encoder myEnc1(enc1a,enc1b);

/* 目標値 */
float commandPos = 0.0f;

/* PD制御の係数*/
float kp = 480;
float kd = 1500;
float prevPos = 0.0f;

void motorOut(int, float);
int SelectOutChannel(int, float);


void setup() {
  pinMode(motor1a,OUTPUT);
  pinMode(motor1b,OUTPUT);
  ledcSetup(1,1000,16);
  ledcSetup(2,1000,16);
  ledcAttachPin(motor1a,1);
  ledcAttachPin(motor1b,2);

  Serial.begin(9600);
}

void loop() {
  float currentPos = myEnc1.read();       //現在の値
  float errPos = commandPos - currentPos; //目標値とのズレ
  float dPos = (currentPos - prevPos);    //前回とのズレ
  float ctrlPos = kp*errPos - kd*dPos;    //制御値

  motorOut(1,ctrlPos);
  
}

/*
モータを動かす関数 
motor:動かすモータ番号，value:値
*/
void motorOut(int motor, float value){
  int ch = SelectOutChannel(motor, value);
  ledcWrite(ch,constrain(fabs(value),COUNT_LOW,COUNT_HIGH));
}

/*
出力に使うpwmの出力チャンネルを選ぶ関数
 */
int SelectOutChannel(int motor,float value){
  //正転のとき
  if(value >= 0){
    switch (motor)
    {
      case 1:
        return 1; //pwmの出力チャンネル
        break;
    
      default:
        break;
    }
  }

  //逆転のとき
  else{
    switch (motor)
    {
      case 1:
        return 2;
        break;
    
      default:
        break;
    }
  }
}