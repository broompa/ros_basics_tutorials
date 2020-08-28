#!/usr/bin/env python

### assignment self: write another script that implements go to goal behavior

import rospy
from std_msgs.msg import String 
from geometry_msgs.msg import Twist

def moveTurtle():
	pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)
	rospy.init_node('MOVE_TURTLE', anonymous = True) 
	rate = rospy.Rate(1)
	spin_rate = 0.9
	prev_Spin = 2
	while not rospy.is_shutdown():
		# hello_str = "hello world %s" %rospy.get_time()
		twist_msg = Twist()
		twist_msg.linear.x = 2
		twist_msg.angular.z = prev_Spin
		rospy.loginfo(twist_msg ) # ?????
		pub.publish(twist_msg)
		prev_Spin *= spin_rate
		rate.sleep()


if __name__ == '__main__':
	try:    
		moveTurtle()
	except rospy.ROSInterruptException:
		pass
 
 