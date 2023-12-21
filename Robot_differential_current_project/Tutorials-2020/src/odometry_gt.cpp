#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>

class MoveByEncoderTicks
{
public:
    MoveByEncoderTicks() : nh("~"), target_ticks(1000)
    {
        cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
        left_encoder_sub = nh.subscribe("/motor_left/encoder", 10, &MoveByEncoderTicks::leftEncoderCallback, this);
        right_encoder_sub = nh.subscribe("/motor_right/encoder", 10, &MoveByEncoderTicks::rightEncoderCallback, this);
    }

    void leftEncoderCallback(const std_msgs::Int32::ConstPtr &msg)
    {
        left_ticks = msg->data;
    }

    void rightEncoderCallback(const std_msgs::Int32::ConstPtr &msg)
    {
        right_ticks = msg->data;
    }

    void moveByEncoderTicks()
    {
        ros::Rate loop_rate(10); // 10 Hz

        while (ros::ok())
        {
            // Check if the target encoder ticks are reached for both wheels
            if (left_ticks >= target_ticks && right_ticks >= target_ticks)
            {
                // Stop the robot by sending zero velocities
                cmd_vel_msg.linear.x = 0.0;
                cmd_vel_msg.angular.z = 0.0;
                cmd_vel_pub.publish(cmd_vel_msg);

                ROS_INFO("Robot reached the target encoder ticks. Stopping.");
                break;
            }

            // Set linear velocity for both wheels based on the difference from the target ticks
            double left_velocity = (target_ticks - left_ticks) * linear_velocity_factor;
            double right_velocity = (target_ticks - right_ticks) * linear_velocity_factor;

            cmd_vel_msg.linear.x = (left_velocity + right_velocity) / 2.0;
            cmd_vel_msg.angular.z = 0.0; // No angular velocity for forward motion

            // Publish the Twist message
            cmd_vel_pub.publish(cmd_vel_msg);

            // Sleep to maintain the loop rate
            loop_rate.sleep();
        }
    }

private:
    ros::NodeHandle nh;
    ros::Publisher cmd_vel_pub;
    ros::Subscriber left_encoder_sub;
    ros::Subscriber right_encoder_sub;

    int target_ticks;
    double linear_velocity_factor = 0.001; // Adjust this factor to control the linear velocity based on encoder ticks
    int left_ticks = 0;
    int right_ticks = 0;

    geometry_msgs::Twist cmd_vel_msg;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "move_by_encoder_ticks");
    MoveByEncoderTicks move_by_encoder_ticks;
    move_by_encoder_ticks.moveByEncoderTicks();
    return 0;
}
