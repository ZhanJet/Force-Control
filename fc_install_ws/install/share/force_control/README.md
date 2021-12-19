# force_control

Force control package for velocity commanded robots with Force/Torque sensor mounted on wrist.

This package implements force control functions including *Admittance Control*, *Force Tracking* and *Hybrid Force/Impedance Control*. Based on *kdl_parser* and *orocos_kdl*, it can be used out of box for any robot with wrist-mounted F/T sensor and robot's URDF file, merely with simple work of definition of the names of some topics and link frames in the launch file.

## Interfaces

The name of the following subscribed & published topics may need to be modified or remapped in the launch file according to the interfaces of the robot you use.

### Subscribed Topics

* **joint_states (sensor_msgs/JointState)**
  * to receive joints' position and velocity
* **robotiq_ft_wrench (geometry_msgs/WrenchStamped)**
  * to receive F/T sensor's data (in F/T sensor frame)
* **force_control/wrench_desired (geometry_msgs/WrenchStamped)**
  * to receive desired wrench (in task frame)
* **force_control/twist_desired (geometry_msgs/TwistStamped)**
  * to receive desired twist of end effector (in arm's base frame)
* ~~**force_contorl/pose_desired (geometry_msgs/PoseStamped)**~~
  * ~~to receive desired equilibrium pose of end effector (in arm's base frame)~~

### Published Topics

* **arm_velocity_controller/cmd_twist (geometry_msgs/Twist)****
  * to send velocity command to robot (must have cartesian twist commanded interface)
  * for robot with only joint velocity interface, an inverse kinematic solver is required
* **ee_pose_desired(geometry_msgs/PoseStamped)***
  * desired pose in base frame(computed through desired twist)
* **ee_pose_real(geometry_msgs/PoseStamped)**
  * real pose in base frame
* **ee_twist_real(geometry_msgs/TwistStamped)***
  * robot twist in base frame

### Services

* ~~**force_contorl/setFcFrame (force_control/SetString)**~~
  * ~~to set force control task frame (in world frame or tool frame)~~
* **force_control/enableFc (std_srvs/SetBool)**
  * to enable force control controller
* **force_control/setFcKp (force_contorl/SetFloat64)**
  * to set the value of kp of fc
  * for tracking desired wrench
* ~~**force_control/setFcKi (force_contorl/SetFloat64)**~~
  * ~~to set the value of ki of fc~~
* **force_control/setFcKd (force_contorl/SetFloat64)**
  * to set the value of kd of fc
  * for improving FC stability
