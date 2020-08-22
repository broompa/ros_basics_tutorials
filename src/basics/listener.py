#!/usr/bin/env python 
# Must add
import rospy
from std_msgs.msg import String

def callback(message):
	rospy.loginfo(rospy.get_caller_id() + 'I heard %s',message.data)

def listener():
	rospy.init_node('listener', anonymous= True)
	rospy.Subscriber('chatter',String, callback)
	rospy.spin()

if __name__ == '__main__':
	listener()  