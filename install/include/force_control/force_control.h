// Copyright (C) 2021 Zhangjie Tu, zhjtu@buaa.edu.cn
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <ros/ros.h>
#include <ros/time.h>

#include <math.h>
// #include <limits>
#include <vector>
#include <eigen3/Eigen/Dense>

#include <tf/transform_datatypes.h>
#include <tf_conversions/tf_eigen.h>
#include <tf/transform_listener.h>
#include <kdl_parser/kdl_parser.hpp>
// #include <tf2_kdl/tf2_kdl.h>
#include <kdl_conversions/kdl_msg.h>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainfksolvervel_recursive.hpp>

#include <geometry_msgs/WrenchStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/Bool.h>

#include "force_control/SetString.h"
#include "force_control/SetFloat64.h"

using namespace Eigen;
using namespace std;
using namespace KDL;

namespace force_control
{
typedef Matrix<double, 7, 1> Vector7d;
typedef Matrix<double, 6, 1> Vector6d;
typedef Matrix<double, 6, 6> Matrix6d;

class FCController
{
public:
    FCController(const ros::NodeHandle& nh, const ros::NodeHandle& pnh);
    FCController():
        FCController(ros::NodeHandle(), ros::NodeHandle("~")){};
    ~FCController();

    void running(); // explicitly autorun the controller
    void update();  // run the controller in the outer while-loop

private:
    ros::NodeHandle nh_;
    ros::NodeHandle pnh_;
    bool auto_run_;
    double ctrl_rate_;

    ros::Timer ctrl_timer_;
    ///> Subscribers:
    ros::Subscriber sub_arm_state_;
    ros::Subscriber sub_wrench_external_;
    ros::Subscriber sub_wrench_desired_;
    ros::Subscriber sub_twist_desired_;
    // ros::Subscriber sub_pose_desired_;

    // Names of subscribed topics
    string topic_arm_state_;
    string topic_wrench_external_;
    string topic_wrench_desired_;
    string topic_twist_desired_;
    // string topic_pose_desired_;

    ///> Publishers:
    ros::Publisher pub_ee_twist_cmd_;   //for arm's velocity interface
    // ros::Publisher pub_ee_pose_cmd_;    //for arm's position interface
    ros::Publisher pub_ee_pose_real_;
    ros::Publisher pub_ee_pose_desired_;
    ros::Publisher pub_ee_twist_real_;
    ros::Publisher pub_wrench_external_;
    ros::Publisher pub_wrench_external_raw_;
    // ros::Publisher pub_wrench_tool_;

    // Names of published topics
    string topic_ee_twist_cmd_;
    // string topic_ee_pose_cmd_;

    ///> Services:
    ros::ServiceServer server_fc_enable_;
    bool fc_enabled_ = false;
    ros::ServiceServer server_fc_frame_;
    string fc_frame_;
    ros::ServiceServer server_fc_kp_;
    ros::ServiceServer server_fc_kd_;
    ros::ServiceServer server_fc_ki_;

    ///> Input signals
    Vector6d wrench_external_raw_;
    Vector6d wrench_external_;
    Vector6d wrench_world_;
    Vector6d wrench_tool_;
    Vector6d wrench_desired_;
    
    Vector3d twist_desired_;
    Vector3d position_desired_new_;
    Quaterniond orientation_desired_new_;
    ///> Output signals
    Vector6d twist_cmd_final_;
    Vector6d twist_cmd_adm_;
    Vector6d twist_cmd_msd_;

    ///> Force control settings
    // FT sensor filter
    double wrench_filter_factor_;
    double force_dead_zone_;
    double torque_dead_zone_;

    // Desired equilibrium pose
    Vector3d position_desired_;
    Quaterniond orientation_desired_;

    Matrix6d M_, D_, K_;

    // PID force tracking
    double kp_, ki_, kd_;
    // Select matrix
    //s=0:force tracking; s=1:impedance control
    Matrix6d select_matrix_;

    // limits of the workspace
    Vector6d workspace_limits_;
    double arm_max_vel_;
    double arm_max_acc_;
    
    ///> State variables
    // TF Listeners
    tf::TransformListener listener_arm_;
    // Frame
    string frame_arm_base_link_;
    string frame_arm_end_link_;
    string frame_arm_tool_link_;
    string frame_ft_sensor_;
    string frame_world_;
    // Transform from base_link to world (const for fixed robot)
    Matrix6d rotation_world_to_base_;
    // Transform from ee_frame to ft_link (const)
    Matrix6d rotation_ee_to_ft_;
    // Transform from ee_link to tool_link (const)
    Matrix6d rotation_ee_to_tool_;
    // Transform from base_link to ee_link
    Matrix6d rotation_base_to_ee_;
    // Transform from base_link to ft_link
    Matrix6d rotation_base_to_ft_;

    //Guards
    bool arm_state_received_;
    bool ft_data_received_;
    bool ee_ft_ready_;
    bool ee_tool_ready_;
    bool world_base_ready_;

    // Arm state: position, orientation, and twist
    Chain kdl_chain_;
    boost::shared_ptr<ChainFkSolverVel> fk_vel_solver_;
    boost::shared_ptr<ChainFkSolverPos> fk_pos_solver_;
    int first_jnt_index_;
    int jnt_num_;
    JntArrayVel q_qdot_;
    FrameVel x_dot_;
    Frame x_;
    geometry_msgs::Pose x_pose_;
    geometry_msgs::Twist x_twist_;
    //for FC computation
    Vector3d position_real_;    //in arm's base frame
    Quaterniond orientation_real_;
    Vector6d arm_real_twist_;
    Vector7d ee_pose_world_;    //in world frame
    Vector6d ee_twist_world_;

private:
    bool loadParams();
    void waitForTransformation();
    void forceControlLoop();
    void limitToWorkspace();
    void sendCommandToRobot();
    void publishRobotStates();

    void updateLoopCB(const ros::TimerEvent&);
    
    void armStateCB(const sensor_msgs::JointStateConstPtr& msg);
    void wrenchExternalCB(const geometry_msgs::WrenchStampedConstPtr& msg);
    void wrenchDesiredCB(const geometry_msgs::WrenchStampedConstPtr& msg);
    void twistDesiredCB(const geometry_msgs::TwistStampedConstPtr& msg);
    // void poseDesiredCB(const geometry_msgs::PoseStampedConstPtr& msg);

    bool enableFcCB(std_srvs::SetBool::Request& req, std_srvs::SetBool::Response& res);
    bool setFcFrameCB(force_control::SetString::Request& req, force_control::SetString::Response& res);
    bool setFcKpCB(force_control::SetFloat64::Request& req, force_control::SetFloat64::Response& res);
    bool setFcKiCB(force_control::SetFloat64::Request& req, force_control::SetFloat64::Response& res);
    bool setFcKdCB(force_control::SetFloat64::Request& req, force_control::SetFloat64::Response& res);    

    bool getRotationMatrix(Matrix6d &rotation_matrix, 
                            tf::TransformListener &listener,
                            string from_frame, string to_frame);  

};
} //namespace force_control