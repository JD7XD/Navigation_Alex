#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"

class LaserScanSubscriber {
public:
    LaserScanSubscriber() {
        // Initialize ROS node
        ros::NodeHandle nh;

        // Subscribe to the /scan topic
        scan_subscriber = nh.subscribe("/scan", 1, &LaserScanSubscriber::scanCallback, this);

        // Advertise on the /cmd_vel topic to control the robot's velocity
        cmd_vel_publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    }

    void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan_msg) {
        // Process the laser scan data
        float max_distance = 0.0;
        int max_distance_index = -1;

        for (int i = 0; i < scan_msg->ranges.size(); ++i) {
            if (!std::isnan(scan_msg->ranges[i]) && scan_msg->ranges[i] > max_distance) {
                max_distance = scan_msg->ranges[i];
                max_distance_index = i;
            }
        }

        // Move toward the direction with the furthest distance
        if (max_distance_index != -1) {
            float angle = scan_msg->angle_min + max_distance_index * scan_msg->angle_increment;

            geometry_msgs::Twist cmd_vel;
            cmd_vel.linear.x = 0.15;  // Adjust the linear velocity as needed
            cmd_vel.angular.z = angle * 0.1;  // Adjust the angular velocity as needed

            cmd_vel_publisher.publish(cmd_vel);
        }
    }

private:
    ros::Subscriber scan_subscriber;
    ros::Publisher cmd_vel_publisher;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "lidar_controller_node");

    LaserScanSubscriber laser_scan_subscriber;

    ros::spin();

    return 0;
}
