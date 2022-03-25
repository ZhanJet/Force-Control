# Summary

This package implements force control functions including *Admittance Control*, *Force Tracking* and *Hybrid Force/Impedance Control*. Based on *kdl_parser* and *orocos_kdl*, it can be used out of box for any robot with wrist-mounted F/T sensor and robot's URDF file, merely with simple work of defining the names of some essential topics and link frames in the launch file.

[Here](./fc_install_ws/install/share/force_control/README.md) is the reference manual about the provided topics and services.

A usage demo of this package can be found at [here](https://youtu.be/0Q9AND165co).  

# Installation

Two ways to install the package.

## Install on fc_intall_ws

make folder on ~/ (or directory you want)
```sh
$ cd ~
$ mkdir fc_install_ws
```

clone git repository
```sh
$ cd ~/fc_install_ws 
$ git clone <https://github.com/*.git>
```
source the setup.bash in install folder and add it into ~/.bashrc
```sh
$ source ~/fc_install_ws/install/setup.bash
$ echo 'source ~/fc_install_ws/install/setup.bash' >> ~/.bashrc
```

## Intall on /opt/ros/melodic

install by .deb package
```sh
$ sudo dpkg -i ros-melodic-force-control_0.0.0-0bionic_amd64.deb
$ roscd force_control
```

# Usage

- Firstly, you should have a robot with Cartesian velocity command interface for ROS and a 6-D F/T sensor mounted on end-effector.
- Sencondly, you need to prepare a URDF file of the robot, and load it through the launch file [here](./fc_install_ws/install/share/force_control/launch/robot_bringup.launch).
- Then, modify the names of topics and frames in [force_control.launch](./fc_install_ws/install/share/force_control/launch/force_control.launch) according to the used robot.
- Lastly, after adjusting the parameters in [config/fc_params.yaml](./fc_install_ws/install/share/force_control/config/fc_params.yaml), launch the force_control.launch

