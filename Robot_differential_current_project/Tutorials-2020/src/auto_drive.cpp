#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <cstdlib>
#include <std_msgs/Float32.h>

bool obstacle_detected = false;
ros::Publisher cmd_vel_pub;

void rotateRobot(int i) {
//    int i = -1;
//    int x = rand() % 100;
//    if (x > 50) {
//        i = -1;
//    }

    geometry_msgs::Twist twist_cmd;
    twist_cmd.angular.z = i * 0.5;  // Rotate left or right
    cmd_vel_pub.publish(twist_cmd);
    ros::Duration(0.3).sleep();
}

void moveForward(int l) {
    geometry_msgs::Twist twist_cmd;
    twist_cmd.linear.x =l* 0.2;  // Move forward
    cmd_vel_pub.publish(twist_cmd);
    ros::Duration(0.2).sleep();
}/*
float min_dis;
void minCallback(const std_msgs::Float32::ConstPtr& min){
	min_dis = min->data;
	int p = -1;
    	int x = rand() % 100;
    	if (x > 50) { p = -1;}

	if(min->data<0.50){moveForward(0);ros::Duration(0.1).sleep();
	rotateRobot(-1);
	ROS_INFO("rotating lol!");
	ros::Duration(0.2).sleep();}
}*/

void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
    if (obstacle_detected) {
        if (scan->ranges[scan->ranges.size() / 2] > 1.0) {
            obstacle_detected = false;
            ROS_INFO("Object Detected");
            moveForward(1);
        }
    } else {
        double fourfive_angle = 0 * (M_PI / 180.0);
        int num_points = scan->ranges.size();
        int index_L = static_cast<int>((fourfive_angle - scan->angle_min) / scan->angle_increment);
        if (index_L >= 0 && index_L < num_points) {
            double distance_L = scan->ranges[index_L];
            if (!std::isinf(distance_L) && distance_L < 0.7) {
                // Object detected at the desired angle
                ROS_INFO("Object detected at %.2f meters at index %d. Rotating...", distance_L, index_L);
                obstacle_detected = true;
                int turn = 1;
                if(scan->ranges[index_L+5]<scan->ranges[index_L - 5]){turn = -1;}
                rotateRobot(turn);
            }
	    if (!std::isinf(distance_L) && distance_L > 1.3){
                ROS_INFO("No object ahead");
                obstacle_detected = false;
                moveForward(1); }
    // Process LIDAR data and find the minimum range
    float min_range = std::numeric_limits<float>::max();
    int min_index;
    for (size_t i = 0; i < scan->ranges.size(); ++i)
    {
        if (!std::isnan(scan->ranges[i]) && scan->ranges[i] < min_range)
        {
            min_range = scan->ranges[i];
            min_index = i;
        }
    }	
	int p =1;
         if(min_range<0.5){
		if(scan->ranges[min_index+5]<scan->ranges[min_index-5]){p=-1;}
	moveForward(-1);ros::Duration(0.8).sleep();
	rotateRobot(p);ros::Duration(0.6).sleep();

	}
	 
        
	}
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "travel");
    ros::NodeHandle nh;

    // Create a subscriber for the Hokuyo LIDAR data
    ros::Subscriber lidar_sub = nh.subscribe<sensor_msgs::LaserScan>("/scan", 1, lidarCallback);
   // ros::Subscriber min_dis = nh.subscribe<>("/min_distance",1,minCallback);
    // Create a publisher for the cmd_vel topic
    cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
   
    ros::spin();

    return 0;
}
