![image](https://github.com/JD7XD/Navigation_Alex/assets/132233669/cc3a6ba5-c05a-414d-915b-12d2c939c2c8)

![image](https://github.com/JD7XD/Navigation_Alex/assets/132233669/aa4ebd18-0504-4d64-b71a-58da0cdab087)

Complete steps For localization and navigation \n

roslaunch hub_driver motor_drivers.launch \n
roslaunch tutorial make_a_map.launch \n
roslaunch velodyne_pointcloud VLP16_points.launch \n 
roslaunch tutorial amcl.launch \n
rosrun hector_mapping hector_mapping _base_frame:=base_link \n
roslaunch navros_pkg move_base.launch \n


rostopic pub /move_base/goal move_base_msgs/MoveBaseActionGoal ---> To publish goals
