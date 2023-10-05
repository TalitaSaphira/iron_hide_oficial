#!/usr/bin/env python3

"""
Universidade Federal de Minas Gerais (UFMG) - 2023
Laboratorio CORO
Contact:
Joao Baiao, <baiaojfr.eng@gmail.com>
"""

# Teleoperação de robô ominidirecional por meio de um jostick

import rospy
import tf
from std_msgs.msg import Float32MultiArray
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Joy
from scipy.spatial.transform import Rotation as R
from Navigator_class import Navigation_class
import numpy as np


class Teleoperator:
    def __init__(self):
        rospy.init_node('Teleoperator_Node')
        self.freq = 30
        self.time_last = rospy.Time.now().to_sec()
        self.time_step = 0.1
        self.desired_velocity_b = [0.0 , 0.0]
        self.yaw = 0.0

        # Pose
        self.pos = np.array([0.0, 0.0])
        self.quaternion = np.array([1.0, 0.0, 0.0, 0.0])

        self.read_params()

        self.odom_sub = rospy.Subscriber(self.odometry_topic_name, Odometry, self.pose_cb)
        self.teleop_sub = rospy.Subscriber("/joy", Joy, self.callback_joy)
        self.inputs_pub = rospy.Publisher(self.input_topic_name, Float32MultiArray, queue_size=10)

    def read_params(self):
        # Obtain the parameters
        # try:
        self.l = float(rospy.get_param("~l", 1.235145)) 
        self.w = float(rospy.get_param("~w", 0.12854)) 
        self.r = float(rospy.get_param("~r", 0.050)) 
        self.input_topic_name = (rospy.get_param("~input_topic_name", "robot/input")) 
        self.odometry_topic_name = (rospy.get_param("~odometry_topic_name", "gt")) 

    def pose_cb(self, msg):
        # Obtain pose
        self.pos = np.array([msg.pose.pose.position.x, msg.pose.pose.position.y])
        self.quaternion = np.array([msg.pose.pose.orientation.x,msg.pose.pose.orientation.y,msg.pose.pose.orientation.z,msg.pose.pose.orientation.w])

    def callback_joy(self, msg):
        change_x = msg.axes[1] + msg.buttons[3] - msg.buttons[1]
        change_y = msg.axes[0] + msg.buttons[0] - msg.buttons[2]

        self.yaw = 0.2*msg.axes[3]

        self.desired_velocity_b = [0.1*change_x, 0.1*change_y]

    def setMovement(self, vector):

        # Convert the quaternion to a rotation object
        rotation = R.from_quat(self.quaternion) 

        # Get the rotation axis vector (x-hat)  
        rotation_matrix = rotation.as_dcm()

        x_now = rotation_matrix[:, 0]

        theta_dot = self.yaw

        v = np.array([[vector[0]], [vector[1]], [theta_dot]])

        v = 0.1*v / (np.linalg.norm(v) + 0.00001)

        M = [[1, -1, -(self.l + self.w)], [1, 1, (self.l + self.w)],
             [1, -1, (self.l + self.w)],[1, 1, -(self.l + self.w)]]

        u = (1/self.r) * (M @ v)

        return u

    def run(self):
        rate = rospy.Rate(self.freq)

        # Wait a bit
        rate.sleep()

        listener = tf.TransformListener()

        while not rospy.is_shutdown():

            # Create an Input message and publish it
            input_msg = Float32MultiArray()

            input_msg.data = self.setMovement(self.desired_velocity_b)

            # Publish the odometry message
            self.inputs_pub.publish(input_msg)

            rate.sleep()

if __name__ == '__main__':
    try:
        navegator = Teleoperator()
        navegator.run()
    except rospy.ROSInterruptException:
        pass
