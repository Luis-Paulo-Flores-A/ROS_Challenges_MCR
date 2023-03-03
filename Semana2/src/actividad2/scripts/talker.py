#!/usr/bin/env python
import rospy
from std_msgs.msg import String

if __name__=='__main__':
    pub=rospy.Publisher("chatter",String, queue_size=10)
    rospy.init_node("talker")
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        hello_str = "Global parameter: " + rospy.get_param("/Message", "No Parameter Found")  + " " + str(rospy.get_time())
        rospy.loginfo(hello_str)
        pub.publish(hello_str)

	hello_str = "Local parameter: " + rospy.get_param("/Message", "No Parameter Found")  + " " + str(rospy.get_time())
        rospy.loginfo(hello_str)
        pub.publish(hello_str)

	hello_str = "Private parameter: " + rospy.get_param("/Message", "No Parameter Found")  + " " + str(rospy.get_time())
        rospy.loginfo(hello_str)
        pub.publish(hello_str)

        rate.sleep()
