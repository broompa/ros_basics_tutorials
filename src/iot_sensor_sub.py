#!/usr/bin/env python
import rospy

from  ros_basics_tutorials.msg import IoTSensor

def iot_sensor_callback(message):
	rospy.loginfo("new IoT data received: (%d %s %.2f %.2f)" ,
		message.id , message.name , message.temperature,
		message.humidity)



rospy.init_node("iot_sensor_sub",anonymous = True)
rospy.Subscriber('iot_sensor_topic', IoTSensor , iot_sensor_callback)

rospy.spin()