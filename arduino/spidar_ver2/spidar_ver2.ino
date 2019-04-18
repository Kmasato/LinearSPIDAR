#include <Arduino.h>
#include <Encoder.h>

#define TIMER_WIDTH
#define COUNT_LOW 0
#define COUNT_HIGH 65535
#define SAMPLE_TIME 0.0000005

int motor1a = 21; //モータ1正転
int motor1b = 22; //モータ1逆転
int enc1a = 26;   //モータ1エンコーダa相
int enc1b = 25;   //モータ1エンコーダb相

int motor2a = 32; //モータ1正転
int motor2b = 33; //モータ1逆転
int enc2a = 18;   //モータ1エンコーダa相
int enc2b = 19;   //モータ1エンコーダb相

Encoder myEnc1(enc1a,enc1b);
Encoder myEnc2(enc2a,enc2b);

/* 目標値 */
float targetPos = 0.0f;

/* PD制御の係数*/
float kp = 300;
float kd = 10;
float ki = 0.005;
float iErr1 = 0.0f;
float iErr2 = 0.0f;
float prevErr1 = 0.0f;
float prevErr2 = 0.0f;

/*関数のプロトタイプ宣言*/
void driveMotor(int, float);
void motorOut(int, float);
int SelectOutChannel(int, float);


void setup() {
  pinMode(motor1a,OUTPUT);
  pinMode(motor1b,OUTPUT);
  pinMode(motor2a,OUTPUT);
  pinMode(motor2b,OUTPUT);
  ledcSetup(1,1000,16);
  ledcSetup(2,1000,16);
  ledcSetup(3,1000,16);
  ledcSetup(4,1000,16);
  ledcAttachPin(motor1a,1);
  ledcAttachPin(motor1b,2);
  ledcAttachPin(motor2a,3);
  ledcAttachPin(motor2b,4);

  Serial.begin(9600);
}

void loop() {
  driveMotor(1,targetPos);
  driveMotor(2,targetPos);
}

void driveMotor(int motor, float target){
  
  float ctrlPos = 0.0f;
  float currentPos = 0.0f;
  float errPos = 0.0f;
  float dErrPos = 0.0f;

  switch (motor)
  {
    case 1:
      currentPos = myEnc1.read();       //現在の値
      errPos = target - currentPos; //目標値とのズレ
      dErrPos = (errPos - prevErr1);    //前回とのズレ
      iErr1 += (dErrPos + errPos)/2.0 * SAMPLE_TIME;
      ctrlPos = kp*errPos + kd*dErrPos + ki*iErr1;   //制御値
      prevErr1 = errPos;
      break;
    
    case 2:
      currentPos = myEnc2.read();       //現在の値
      errPos = target - currentPos; //目標値とのズレ
      dErrPos = (errPos - prevErr2);    //前回とのズレ
      iErr2 += (dErrPos + errPos)/2.0 * SAMPLE_TIME;
      ctrlPos = kp*errPos + kd*dErrPos + ki*iErr2;   //制御値
      prevErr2 = errPos;
      break;

    default:
      ctrlPos = 0.0f;
      break;
  }
  motorOut(motor, ctrlPos);
}

/*
モータを動かす関数 
motor:動かすモータ番号，value:値
*/
void motorOut(int motor, float value){
  int ch1 = SelectOutChannel(motor, value);
  int ch2 = SelectOutChannel(motor, -value); //正負が逆転したvalueを与えることで対のチャンネルを得られる
  ledcWrite(ch1,constrain(fabs(value),COUNT_LOW,COUNT_HIGH));
  ledcWrite(ch2,0);
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
      case 2:
        return 3;
        break;
    
      default:
        return 0;
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
      case 2:
        return 4;
        break;
    
      default:
        return 0;
        break;
    }
  }
}

//ちんこ
