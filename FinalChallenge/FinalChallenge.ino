#include <ros.h>
#include <ESP32Encoder.h>
#include <final_challenge/motor_input.h> //Motor input
#include <final_challenge/motor_output.h> //Motor output

#define PWM 4
#define INPUT_MOTOR1 18
#define INPUT_MOTOR2 15
#define ENCODER1 34
#define ENCODER2 36
#define PI 3.1415926535

const int MED_TIME = 100; //Cada cuanto se calcula la velocidad (milis)
const double TICKS_PER_LAP = 744; //Ticks que da la rueda por cada vuelta, se saca corriendo un ejemplo de la librería del encoder
const double RADIANS_PER_TICK = (2*PI)/TICKS_PER_LAP; //Relación para obtener los radianes por cada tick de la rueda

long position = 0; //Posición actual
long prevPos = 0; //Posición previa
long timing = millis(); //Tiempo transcurrido


ESP32Encoder encoder;
ros::NodeHandle nh;
final_challenge::motor_output encoder1_vel;

void pwmCall(const final_challenge::motor_input &pwmMsg) {
  ledcWrite(0, abs((int)(pwmMsg.input) * 255));
  if (pwmMsg.input > 0) {
    digitalWrite(INPUT_MOTOR1, 1);
    digitalWrite(INPUT_MOTOR2, 0);
  } else {
    digitalWrite(INPUT_MOTOR1, 0);
    digitalWrite(INPUT_MOTOR2, 1);
  }
}


void speed(int updatedPos) {
  if (updatedPos != position){
    position = updatedPos;
  }
  encoder1_vel.output = float(((RADIANS_PER_TICK*(position - prevPos))*1000)/MED_TIME );
  prevPos = position;  
}

ros::Subscriber<final_challenge::motor_input> pwm("/motor_input", pwmCall); //Recibe pwm
ros::Publisher motorVel("/motor_output", &encoder1_vel); //El output del motor sera la velocidad del mismo


void setup() {
  ledcSetup(0, 980, 8);
  pinMode(INPUT_MOTOR1, OUTPUT);
  pinMode(INPUT_MOTOR2, OUTPUT);
  pinMode(PWM, OUTPUT);

  //Inicializando ros
  ledcAttachPin(PWM, 0);
  nh.initNode();
  nh.subscribe(pwm);
  nh.advertise(motorVel);
}


void loop() {
  if(millis()-timing >= MED_TIME){
    speed(position);
    motorVel.publish(&encoder1_vel);
    timing = millis();
  }
  nh.spinOnce();
}
