#!/usr/bin/env python3
# -*- coding: utf-8 -*-



import rclpy                        
from rclpy.node import Node     
from sensor_msgs.msg import Image,CompressedImage   
from cv_bridge import CvBridge      
import cv2                         

"""
创建一个发布者节点
"""
class ImagePublisher(Node):

    def __init__(self, name):
        super().__init__(name)                                          
        self.publisher_ = self.create_publisher(CompressedImage, 'carcam', 1) 
        self.timer = self.create_timer(0.03, self.timer_callback)         
        self.cap = cv2.VideoCapture(0)                                 
        self.cv_bridge = CvBridge()                                      

    def timer_callback(self):
        ret, frame = self.cap.read()                                     

        if ret == True:                                                  
            self.publisher_.publish(
            self.cv_bridge.cv2_to_compressed_imgmsg(frame, 'jpg'))
            self.get_logger().info('Publishing video frame')                  
        else:
            self.get_logger().info('capture fault')
                 

def main(args=None):                                
    rclpy.init(args=args)                           
    node = ImagePublisher("cam_pub")       
    rclpy.spin(node)                                 
    node.destroy_node()                              
    rclpy.shutdown()                                 
