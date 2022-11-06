


import rclpy                                     
from rclpy.node   import Node                    
from std_msgs.msg import String                 
import serial

class SubscriberNode(Node):

    def __init__(self, name):
        super().__init__(name)                                    
        self.sub = self.create_subscription(\
            String, "movecontrol", self.movecontrol_callback, 10)        
        self.uart = serial.Serial(port="/dev/ttyAMA2", baudrate=115200)

    def movecontrol_callback(self, msg):                             
        self.get_logger().info('I heard: "%s"' % msg.data)        
        self.uart.write(str(msg.data).encode('utf_8'))

def main(args=None):                                 

    rclpy.init(args=args)                            
    node = SubscriberNode("vehiclemovecontrol_sub")    
    rclpy.spin(node)                                 
    node.destroy_node()                              
    rclpy.shutdown()                                 

