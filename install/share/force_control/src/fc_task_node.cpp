#include <ros/ros.h>
#include <eigen3/Eigen/Core>

#include "geometry_msgs/WrenchStamped.h"
#include "geometry_msgs/TwistStamped.h"
#include "std_msgs/Float32.h"

using namespace Eigen;
typedef Matrix<double, 6, 1> Vector6d;

Vector6d wrench_external;

void wrenchExternalCB(const geometry_msgs::WrenchStampedConstPtr &msg)
{
    wrench_external << msg->wrench.force.x, msg->wrench.force.y,
                    msg->wrench.force.z, msg->wrench.torque.x,
                    msg->wrench.torque.y, msg->wrench.torque.z;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "wiping_task_node");

    ros::NodeHandle nh;
    ros::Rate loop_rate(100.0);

    ros::Publisher pub_wrench_desired = nh.advertise<geometry_msgs::WrenchStamped>("force_control/wrench_desired", 5);

    ros::Publisher pub_desired_velocity = nh.advertise<geometry_msgs::TwistStamped>("force_control/twist_desired", 5);

    ros::Subscriber sub_wrench_external = nh.subscribe("wrench_external", 5,
                                                        &wrenchExternalCB,
                                                        ros::TransportHints().reliable().tcpNoDelay());
    
    /**
     * Wiping Task Process:
     *  - pub desired wrench
     *  - check external wrench > 0.9*wrench_desired
     *  - stand still for 1s
     *  - start moving until path finished
     *  - leave surface
     *      - pub -wrench_desired
     *      - wait for 2s
     *      - pub wrench_desired=0   
     */
    geometry_msgs::WrenchStamped wrench_desired;
    wrench_desired.wrench.force.y = -5.0;

    while(ros::ok()) {
        if(wrench_external(1) < 0.9*wrench_desired.wrench.force.y) {
            ROS_INFO("Has achived Fd! :)");
            ros::Duration(2).sleep();
            break;
        } else {
            pub_wrench_desired.publish(wrench_desired);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }


    geometry_msgs::TwistStamped velocity_desired;
    geometry_msgs::TwistStamped velocities[7];
    velocities[0].twist.linear.z = 0.015;
    velocities[1].twist.linear.x = 0.015;
    velocities[2].twist.linear.z = -0.015;
    velocities[3].twist.linear.x = 0.015;
    velocities[4].twist.linear.z = 0.015;
    velocities[5].twist.linear.x = 0.015;
    velocities[6].twist.linear.z = -0.015;

    ros::Time last_time = ros::Time::now();
    ros::Time polish_time = last_time + ros::Duration(5);
    int traj_index = 0;
    while(ros::ok()) {
        if(ros::Time::now() < polish_time) {
            ROS_INFO_STREAM_THROTTLE(1, "Executing trajectory: "<<traj_index);
            velocity_desired = velocities[traj_index];
            pub_desired_velocity.publish(velocity_desired);
        } else {
            ROS_INFO_STREAM("Has completed trajectory: "<<traj_index);
            traj_index ++;
            ros::Duration(1).sleep();
            if(traj_index > 6) {
                break;
            }
            polish_time = ros::Time::now() + ros::Duration(4);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    last_time = ros::Time::now();
    ros::Time retreat_time = last_time + ros::Duration(4);
    while(ros::ok()) {
        if(ros::Time::now() < retreat_time) {
            ROS_INFO_STREAM_THROTTLE(1, "Retreating trajectory... ");
            wrench_desired.wrench.force.y = 5.0;
        } else {
            ROS_INFO_STREAM("Polish tast finished! :) ");
            wrench_desired.wrench.force.y = 0.0;
        }

        pub_wrench_desired.publish(wrench_desired);

        ros::spinOnce();
        loop_rate.sleep();
    }
}