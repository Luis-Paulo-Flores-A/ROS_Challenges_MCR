#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include "final_challenge/sender.h"

float set_point = 0.0;
double time = 0.0;
float reference = 10;
enum Signal {
    no_signal,
    step_signal,
    square_signal,
    sine_signal
};

void def_signal(int signal) {
  switch (command) {
    case step_signal:
        set_point = time > 0 ? 1 * reference : 0;
        break;
    case square_signal:
        set_point = std::fmod(time*0.5,2) > 1 ? 1 * reference : -1 * reference;
        break;
    case sine_signal:
        set_point = sin(time*2)*reference;
        break;
  }
}

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "sender");
    ros::NodeHandle handler;
    int nodeRate = 100;
    ros::Publisher signalPub = handler.advertise<final_challenge::sender>("/set_point",10);
    ros::Rate rate(nodeRate);
    final::sender set_point_out;
    int signal = 0;
    set_point_out.set_point_data = 0.0;
    while (ros::ok()) {
        ros::param::get("/sp_type", cmd);
        time = ros::Time::now().toSec();
        def_signal(signal);
        set_point_out.set_point_data = set_point;
        signalPub.publish(set_point_out);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
