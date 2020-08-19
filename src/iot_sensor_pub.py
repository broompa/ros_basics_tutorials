#!/usr/bin/env python

import rospy
from ros_basics_tutorials.msg import IoTSensor
import random

def PublishData():
	pub = rospy.Publisher('iot_sensor_topic',IoTSensor,queue_size = 10)
	rospy.init_node('iot_senor_pub',anonymous= True)
	rate = rospy.Rate(1)

	i = 0
	while not rospy.is_shutdown():
		iot_sensor = IoTSensor()
		iot_sensor.id = i
		iot_sensor.name = 'iot_parking_01'
		iot_sensor.temperature = 25 + random.random()*3
		iot_sensor.humidity = 30 + random.random()*4

		rospy.loginfo("I publish:")
		rospy.loginfo(iot_sensor)
		pub.publish(iot_sensor)
		rate.sleep()
		i = i+ 1
		









if __name__ == '__main__':
	try:
		PublishData()
	except rospy.ROSInterruptException:
		pass