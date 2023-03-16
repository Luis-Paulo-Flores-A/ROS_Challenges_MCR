import rospy
from std_msgs.msg import Int16, Float32
from math import sin, fmod
from final_challenge.msg import sender

set_point = 0.0
time = 0.0
reference = 10
no_signal = 0
step_signal = 1
square_signal = 2
sine_signal = 3

def def_signal(signal):
    global set_point, time
    if signal == step_signal:
        set_point = 1 * reference if time > 0 else 0
    elif signal == square_signal:
        set_point = 1 * reference if fmod(time * 0.5, 2) > 1 else -1 * reference
    elif signal == sine_signal:
        set_point = sin(time * 2) * reference

def main():
    global set_point, time

    rospy.init_node('sender')
    node_rate = 100
    signal_pub = rospy.Publisher('/set_point', sender, queue_size=10)
    rate = rospy.Rate(node_rate)
    set_point_out = sender()
    signal = 0
    set_point_out.set_point_data = 0.0
    while not rospy.is_shutdown():
        cmd = rospy.get_param("/sp_type")
        time = rospy.get_time()
        def_signal(signal)
        set_point_out.set_point_data = set_point
        signal_pub.publish(set_point_out)
        rate.sleep()

if __name__ == '__main__':
    main()
