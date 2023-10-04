cmake_minimum_required(VERSION 3.0.2)
project(iron_hide)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
#  cv_brigde
  geometry_msgs
#  message_generation
  nav_msgs
  roscpp
  rospy
  rviz
  std_msgs
  tf
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   geometry_msgs
   nav_msgs  
   std_msgs
 )

###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES iron_hide
  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp rospy rviz std_msgs tf #cv_bridge message_generation
  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/iron_hide.cpp
# )

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )


#Publisher.py
#add_executable(talker_node src/publisher.py)

#Publisher.cpp
add_executable(talker_node src/testes/ROS/publisher.cpp)
target_link_libraries(talker_node ${catkin_LIBRARIES})

#Subscriber.cpp
add_executable(listener_node src/testes/ROS/subscriber.cpp)
target_link_libraries(listener_node ${catkin_LIBRARIES})

#Aestrela_pub.cpp
add_executable(rota_node src/sources/Aestrelapub.cpp)
target_link_libraries(rota_node ${catkin_LIBRARIES})

#main.cpp
add_executable(movimentation_main_node src/sources/main.cpp)
target_link_libraries(movimentation_main_node ${catkin_LIBRARIES})





