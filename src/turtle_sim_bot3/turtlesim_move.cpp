#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "turtlesim/Pose.h"

void move(double speed , double dist, bool forward);
void relativeRotate(double ang_speed , double angle , bool clockwise ); // ang_speed -> rad/s , angle - degree
double degree2radians(double degree);
void absoluteRotate(double desired_angle_radians);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);

double radians2Degree(double ra);
void goToGoal(turtlesim::Pose goal_pose, double distance_tolerance);

const double PI = 3.14159;

ros::Publisher pub ; // publisher for cmd_vel
ros::Subscriber Pose; // 
turtlesim::Pose curr_pose;

int main(int argc, char **argv){
	ros::init(argc,argv,"robot_cleaner_self");
	ros::NodeHandle n;
	// std::string topicToPublish = "/cmd_vel"; // for burger robot
	std::string topicToPublish = "/turtle1/cmd_vel";
	pub = n.advertise<geometry_msgs::Twist>(topicToPublish,10);
	Pose = n.subscribe("/turtle1/pose",10,poseCallback);
	double speed=0, dist=0 , ang_speed = 0, angle = 0;;
	bool forward = 1, clockwise = 1;	
	// ROS_INFO("x:%f y:%f ang:%f vl:%f va:%f \n",curr_pose.x,curr_pose.y,curr_pose.theta,curr_pose.linear_velocity,curr_pose.angular_velocity);
		
	// std::cout << "\nspeed: "; std::cin >> speed; 
	// std::cout << "\ndistance: " ; std::cin >> dist ; 
	// std::cout << "\nForward: " ; std::cin >> forward;
	// move(speed, dist , forward);

	// ROS_INFO("x:%f y:%f ang:%f vl:%f va:%f \n",curr_pose.x,curr_pose.y,curr_pose.theta,curr_pose.linear_velocity,curr_pose.angular_velocity);
	// std::cout << "\nangular speed: "; std::cin >> ang_speed ; 
	// std::cout << "\nangle: " ; std::cin >> angle ; 
	// std::cout << "\nclockwise: " ; std::cin >> clockwise;
	// relativeRotate(ang_speed, angle, clockwise);
	turtlesim::Pose goal_pose; 
	// while(true){

	// 	ros::spinOnce();
	// 	// std::cout << "\nangle: " ; std::cin >> angle ; 
	// 	// absoluteRotate(degree2radians(angle));
		
	// 	ROS_INFO("x : %f, y: %f,theta:%f\n",curr_pose.x,curr_pose.y,curr_pose.theta);
	// 	// std::cout << "\nDesired X:" ; std::cin >> goal_pose.x;
	// 	// std::cout << "\nDesired Y:" ; std::cin >> goal_pose.y ;
		
	// 	goToGoal(goal_pose,0.25);

	// 	// std::cout << "\nangular speed: "; std::cin >> ang_speed ; 
	// 	// std::cout << "\nangle: " ; std::cin >> angle ; 
	// 	// std::cout << "\nclockwise: " ; std::cin >> clockwise;
	// 	// relativeRotate(ang_speed, angle, clockwise);
	// }
	goal_pose.x = 1; goal_pose.y = 1;
	goToGoal(goal_pose,0.25);
	goal_pose.x = 10; goal_pose.y = 10;
	goToGoal(goal_pose,0.25);
	goal_pose.x = 1; goal_pose.y = 10;
	goToGoal(goal_pose,0.25);
	goal_pose.x = 10; goal_pose.y = 1;
	goToGoal(goal_pose,0.25);
	


}


void goToGoal(turtlesim::Pose goal_pose, double distance_tolerance){
	geometry_msgs::Twist twist ;
	ros::Rate loop_rate(100);
	double dist = sqrt(pow(goal_pose.x -curr_pose.x,2)+pow(curr_pose.y - goal_pose.y,2));
	double kp = 2, kAngle = 3*kp;
	twist.linear.x = 0; 
	twist.linear.y = 0;
	twist.linear.z = 0;
	twist.angular.x = 0 ;
	twist.angular.y = 0 ;
	twist.angular.z = 0 ;
	double E = 0;
	while(dist > distance_tolerance){
		E += dist;
		twist.linear.x = kp*dist+ E*0.0002 ;
		double e_ang = atan2((goal_pose.y - curr_pose.y), (goal_pose.x - curr_pose.x))-curr_pose.theta;
		twist.angular.z = kAngle*(atan2(sin(e_ang),cos(e_ang))); // restriction b/w [-pi, pi]
		pub.publish(twist);
		ros::spinOnce();
		loop_rate.sleep();
		dist = sqrt(pow(goal_pose.x -curr_pose.x,2)+pow(curr_pose.y - goal_pose.y,2));
	}
	twist.linear.x = 0; 
	twist.angular.z = 0;
	pub.publish(twist);
}	

void absoluteRotate(double desired_angle_radians){
	double ang = desired_angle_radians - curr_pose.theta;
	relativeRotate(1,abs(radians2Degree(ang)), ang<0);
}

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	curr_pose.x = pose_message->x;
	curr_pose.y = pose_message->y;
	curr_pose.theta = pose_message->theta;
}
void move(double speed , double dist, bool forward){
	double duration = dist / speed ;
	speed *= (forward==1)?1 : -1;
	double t0 = ros::Time::now().toSec();
	geometry_msgs::Twist twist ;
	twist.linear.x= speed;
	twist.linear.y= 0;
	twist.linear.z= 0;
	twist.angular.x= 0;
	twist.angular.y= 0;
	twist.angular.z= 0;
	ros::Rate loop_rate(10); // 10hz
	while(ros::Time::now().toSec() - t0 < duration && ros::ok()){
		pub.publish(twist);
		loop_rate.sleep();
	}
	twist.linear.x = 0;
	pub.publish(twist);
}
void relativeRotate(double ang_speed , double angle , bool clockwise ){
	angle = abs(angle);
	angle = degree2radians( angle - 360*((int)angle/360));
	double duration = angle / ang_speed ;
	ang_speed *= (clockwise==1)?-1 : 1;
	double t0 = ros::Time::now().toSec();
	geometry_msgs::Twist twist ;
	twist.linear.x= 0;
	twist.linear.y= 0;
	twist.linear.z= 0;
	twist.angular.x= 0;
	twist.angular.y= 0;
	twist.angular.z= ang_speed;
	ros::Rate loop_rate(10); // 10hz
	while(ros::Time::now().toSec() - t0 < duration ){
		pub.publish(twist);
		loop_rate.sleep();
	}
	twist.angular.z = 0;
	pub.publish(twist);
}






double degree2radians(double degree){	
	return  degree*PI/ 180;
}
double radians2Degree(double rad){
	return rad*180/PI;
}