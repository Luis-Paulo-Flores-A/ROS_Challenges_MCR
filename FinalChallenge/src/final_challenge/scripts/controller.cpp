#include <cmath>
#include <string>
#include <iostream>
#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include "final_challenge/motor_input.h"
#include "final_challenge/motor_output.h"
#include "final_chalenge/sender.h"

float motor_out = 0, reference = 0;
bool motor_init = false;

void feedback(const final::motor_output::ConstPtr &msg) {
  motor_out = msg->output;
  motor_init = true;
}

void setpoint_getter(const final::sender::ConstPtr &setpoint) {
  ref = setpoint->set_point_data;
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "controller");
  ros::NodeHandle n;
  ros::NodeHandle n2;
  float Kp, Ti, Td;
  ros::param::get("/Kp",Kp);
  ros::param::get("/Ti",Ti);
  ros::param::get("/Td",Td);
  
  ros::Subscriber systemFeedback = n.subscribe("/motor_output", 10, feedback);
  ros::Publisher controllerOutput = n2.advertise<final::motor_input>("/motor_input", 10);
  ros::Subscriber setPointSubscr = n.subscribe("/set_point", 10, setpoint_getter);
  ros::Rate rate(100); 
  double time = ros::Time::now().toSec();
  float error, rateError, time;
  float lastError = 0.0;
  float totError = 0.0;
  final::motor_input motor_in;
  motor_in.input = 0.0;
  controllerOutput.publish(motor_in);
  while (ros::ok()) {
      if(motor_init){
        init_time = ros::Time::now().toSec()- time;
        if(init_time > 0) {
          error = reference - motor_out;
          totError += error*init_time;
          rateError = (error - lastError) / time;
          lastError = error;
          motor_in.input = (Kp*error)+(Td*rateError)+(Ti*totError);
          if(abs(motor_in.input)>1){
            motor_in.input = motor_in.input/abs(motor_in.input);
          }
          controllerOutput.publish(motor_in);
        } else {
          final::motor_input motor_in;
          motor_in.input = 0;
          controllerOutput.publish(motor_in);
        }
      }
      init_time = ros::Time::now().toSec();
      ros::spinOnce();
      rate.sleep();
    }
}
