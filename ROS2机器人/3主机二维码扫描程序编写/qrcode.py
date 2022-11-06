#!/home/seanlu/anaconda3/bin/ python
# -*- coding: utf-8 -*-




from tokenize import String
import rclpy                            
from rclpy.node import Node            
from sensor_msgs.msg import Image,CompressedImage      
from std_msgs.msg import String as STR
from cv_bridge import CvBridge          
import cv2                              
import numpy as np                      
import pyzbar.pyzbar as pyzbar
import pymysql
import datetime,time
lastvalues=[0,0,0,0]
lasttime=time.perf_counter()
save_file="/home/seanlu/data/qrcode/qrcode_"
class node_QRSCAN(Node):
    def __init__(self, name):
        super().__init__(name)                                  
        self.sub = self.create_subscription(CompressedImage, 'carcam', self.listener_callback,1)
        self.pub = self.create_publisher(STR, "QR_DATA", 1)    
        
        self.msg = STR()    
        self.cv_bridge = CvBridge()                            

    def listener_callback(self, data):
      
        image = self.cv_bridge.compressed_imgmsg_to_cv2(data, 'passthrough')     
        
        qr_detect(self,self.msg,image)      
        self.pub.publish(self.msg)      
        self.get_logger().info('QR_STATUS: "%s"' % self.msg.data)                               


def qr_detect(self,msg,frame):
    global lasttime
    if(int(time.perf_counter()-lasttime)>2):
        barcodes = pyzbar.decode(frame)
        for barcode in barcodes:
            (x, y, w, h) = barcode.rect
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
            barcodeType = barcode.type
            barcodeData = barcode.data.decode("utf-8")
            text = "{0}: {1} ".format(barcodeType, barcodeData)
            cv2.putText(frame, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX,\
                        .5, (0, 0, 250), 1)

            t=datetime.datetime.now()
            path=str(save_file)+str(t)[:-8]+'.png'
            insert(self,barcodeType, barcodeData,path)
            cv2.imwrite(path,frame)
            self.get_logger().info("[INFO] Detected! :{}  data:  {} ".format(barcodeType, barcodeData))
            msg.data=barcodeData
            self.pub.publish(msg)
            lasttime=time.perf_counter()

    cv2.imshow("qr-scan", frame)                             
    cv2.waitKey(1)


def insert(self,type, data,path):
        global lastvalues
        conn = pymysql.connect(
            host="127.0.0.1",
            user="root",
            password="konososstd",
            db="STARK",
            port=3306,
            charset="utf8"
        )
        cursor = conn.cursor()
        query="insert into STARK(EVENT,TIME,LOCATION,DATA,PATH)\
                values(%s,%s,%s,%s,%s)"
        time=str(datetime.datetime.now())[:-7]
        values=(type,time,"LOCATION",data,path)
        if lastvalues[3]!=values[3]:
            try:
                if cursor.execute(query,values)!=-1:
                    flag = False
                conn.commit()
                lastvalues=values
            except Exception as e:
                conn.rollback()
                print("error!")
                print(e)
        else:
            self.get_logger().info("SAME QRDATA!!!")
        cursor.close()
        conn.close()
def main(args=None):                                        
    rclpy.init(args=args)                                   
    node = node_QRSCAN("topic_webcam_sub")              
    rclpy.spin(node)                                        
    node.destroy_node()                                     
    rclpy.shutdown()                                        
