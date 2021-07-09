# Summary
Universal force control package for velocity commanded robots with Force/Torque sensor mounted on wrist.

This package implements force control functions including *Admittance Control*, *Force Tracking* and *Hybrid Force/Impedance Control*. Based on *kdl_parser* and *orocos_kdl*, it can be used out of box for any robot with wrist-mounted F/T sensor and robot's URDF file, merely with simple work of definition of the names of some topics and link frames in the launch file.

# Installation
## install on project_intall_ws

make folder on ~/
```sh
cd /home/${username}
mkdir -p ${project}/${project}_install_ws
```

clone git repository
```sh
cd ~/${project}/${project}_install_ws 
git clone <https://github.com/*.git>
```
source the setup.bash in install folder and add it into ~/.bashrc
```sh
source ~/${project}/${project}_install_ws/install/setup.bash
echo 'source ~/${project}/${project}_install_ws/install/setup.bash' >> ~/.bashrc
```

## intall on /opt/ros/melodic
install by .deb package
```sh
sudo dpkg -i ros-melodic-force-control_0.0.0-0bionic_amd64.deb
roscd force_control
```

# Usage
- Firstly, you should have a robot with velocity command interface of ROS and a 6-D F/T sensor on end-effector mounted.
- Sencondly, you should prepare a URDF file of the robot, and load it through launch file.
- Then, modify the topic names and frame names in the force_control.launch(in install/share/force_control/launch) according the using robot.
- Lastly, after  adjust the parameters in config/fc_params.yaml, launch the force_control.launch

