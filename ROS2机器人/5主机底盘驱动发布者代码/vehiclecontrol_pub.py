




from itertools import count
import rclpy                                     
from rclpy.node import Node                      
from std_msgs.msg import String                  

str=['P x0 y1000 r0\r\n',
'P x0 y-1000 r0\r\n',
'P x1000 0 r0\r\n',
'P x-1000 y0 r0\r\n']


class PublisherNode(Node):

    def __init__(self, name):
        super().__init__(name)                                    
        self.pub = self.create_publisher(String, "movecontrol", 10)   
        self.timer = self.create_timer(5, self.timer_callback)  
        self.count=0
    def timer_callback(self):                                     


        msg = String()                                            
        msg.data = str[self.count % 4]                                  
        self.pub.publish(msg)                                     
        self.get_logger().info('Publishing: "%s"' % msg.data)     
        self.count+=1

def main(args=None):                                 
    rclpy.init(args=args)                            
    node = PublisherNode("movecontrol_pub")     
    rclpy.spin(node)                                 
    node.destroy_node()                              
    rclpy.shutdown()                                 
