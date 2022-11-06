import os
import sys
path = os.path.abspath(".")
sys.path.insert(0, '/home/seanlu/Desktop/ros2/workspace/src/func/func')

import time
import datetime
from sensor_msgs.msg import Image, CompressedImage
import numpy as np
import cv2
from cv_bridge import CvBridge
from rclpy.node import Node
import rclpy
from utils.torch_utils import select_device, smart_inference_mode
from utils.plots import Annotator, colors, save_one_box
from utils.general import (LOGGER, Profile, check_file, check_img_size, check_imshow, check_requirements, colorstr, cv2,
                           increment_path, non_max_suppression, print_args, scale_coords, strip_optimizer, xyxy2xywh)
from utils.dataloaders import IMG_FORMATS, VID_FORMATS, LoadImages
from models.common import DetectMultiBackend
import torch.backends.cudnn as cudnn
import torch
from pathlib import Path
import platform
import argparse
import time as timeee
import pymysql
from std_msgs.msg import String as STR
from tokenize import String

from turtle import update

save_file = "/home/seanlu/data/detection/yolo_"
FILE = Path(__file__).resolve()
ROOT = FILE.parents[0]
if str(ROOT) not in sys.path:
    sys.path.append(str(ROOT))
ROOT = Path(os.path.relpath(ROOT, Path.cwd()))
carimg = None
stride, pt = 0, 0
class node_yolo(Node):
    def __init__(self, name):
        super().__init__(name)
        self.sub = self.create_subscription(CompressedImage, 'carcam', self.listener_callback, 1)
        self.yolo_pub = self.create_publisher(Image, "yolo_pub", 1)
        self.timer = self.create_timer(2, self.timer_callback)
        self.msg = STR()
        self.yoloimg = Image()
        self.cv_bridge = CvBridge()
        self.carimage = None
        self.update_flag = True
        self.update_flag = False
        self.data = ROOT / 'data/coco128.yaml'
        self.weights = '/home/seanlu/Desktop/ros2/workspace/install/func/lib/python3.10/site-packages/func/yolov5s.pt'
        self.source = ROOT / 'a.jpg'
        self.update = False,
        self.project = ROOT / 'runs/detect'
        self.name = 'exp'
        self.exist_ok = False
        self.half = True
        self.dnn = False
        self.data = ROOT / 'data/coco128.yaml'
        self.imgsz = (640, 640)
        self.conf_thres = 0.55
        self.iou_thres = 0.45
        self.max_det = 1000
        self.device = ''
        self.view_img = False
        self.save_txt = False
        self.save_conf = False
        self.save_crop = False
        self.classes = None
        self.agnostic_nms = False
        self.augment = False
        self.visualize = False
        self.line_thickness = 3
        self.hide_labels = False
        self.hide_conf = False
        self.lastinserttime = timeee.perf_counter()
        self.conn = pymysql.connect(
            host="127.0.0.1",
            user="root",
            password="konososstd",
            db="STARK",
            port=3306,
            charset="utf8"
        )
        check_requirements(exclude=('tensorboard', 'thop'))
        self.source = str(self.source)

        self.device = select_device(self.device)
        self.model = DetectMultiBackend(self.weights, self.device, self.dnn, self.data, self.half)
        stride, self.names, pt = self.model.stride, self.model.names, self.model.pt
        self.imgsz = check_img_size(self.imgsz, s=stride)
        self.model.warmup(imgsz=(1 if pt else 1, 3, *self.imgsz))
        if self.view_img:
            self.view_img = check_imshow()
        cudnn.benchmark = True

    def insert(self, data, path):
        try:
            self.conn.ping()
        except:
            self.conn = pymysql.connect(
                host="127.0.0.1",
                user="root",
                password="konososstd",
                db="STARK",
                port=3306,
                charset="utf8"
            )
        cursor = self.conn.cursor()
        query = "insert into STARK(EVENT,TIME,LOCATION,DATA,PATH)\
                values(%s,%s,%s,%s,%s)"
        submit_time = str(datetime.datetime.now())[:-7]
        values = ('detection', submit_time, "LOCATION", data, path)
        try:
            if cursor.execute(query, values) != -1:
                pass
            self.conn.commit()
        except Exception as e:
            pass
            print("error!")
            print(e)
            self.conn.rollback()
        self.lastinserttime = timeee.perf_counter()
        cursor.close()

    def listener_callback(self, imgdata):
        self.carimage = self.cv_bridge.compressed_imgmsg_to_cv2(imgdata, 'passthrough')

        dataset = LoadImages(self.source, img_size=self.imgsz, stride=stride, auto=pt, carimg=self.carimage)

        bs = len(dataset)
        self.get_logger().info('Receiving video frame')

        seen, windows, dt = 0, [], (Profile(), Profile(), Profile())
        for path, im, im0s, vid_cap, s in dataset:
            with dt[0]:
                im = torch.from_numpy(im).to(self.device)
                im = im.half() if self.model.fp16 else im.float()
                im /= 255
                if len(im.shape) == 3:
                    im = im[None]

            with dt[1]:

                pred = self.model(im, augment=self.augment, visualize=self.visualize)

            with dt[2]:
                pred = non_max_suppression(pred, self.conf_thres, self.iou_thres,
                                           self.classes, self.agnostic_nms, max_det=self.max_det)

            for i, det in enumerate(pred):
                label = None
                seen += 1
                if False:
                    p, im0, frame = path[i], im0s[i].copy(), dataset.count
                    s += f'{i}: '
                else:
                    p, im0, frame = path, im0s.copy(), getattr(dataset, 'frame', 0)

                p = Path(p)
                s += '%gx%g ' % im.shape[2:]
                gn = torch.tensor(im0.shape)[[1, 0, 1, 0]]
                imc = im0.copy() if self.save_crop else im0
                annotator = Annotator(im0, line_width=self.line_thickness, example=str(self.names))

                if len(det):

                    det[:, :4] = scale_coords(im.shape[2:], det[:, :4], im0.shape).round()

                    for c in det[:, -1].unique():
                        n = (det[:, -1] == c).sum()
                        s += f"{n} {self.names[int(c)]}{'s' * (n > 1)}, "

                    for *xyxy, conf, cls in reversed(det):
                        if self.save_txt:
                            xywh = (xyxy2xywh(torch.tensor(xyxy).view(1, 4)) / gn).view(-1).tolist()
                            line = (cls, *xywh, conf) if self.save_conf else (cls, *xywh)
                            with open(f'{self.txt_path}.txt', 'a') as f:
                                f.write(('%g ' * len(line)).rstrip() % line + '\n')

                        c = int(cls)
                        if c == 0:
                            label = None if self.hide_labels else (
                                self.names[c] if self.hide_conf else f'{self.names[c]} {conf:.2f}')
                            annotator.box_label(xyxy, label, color=colors(c, True))

                im0 = annotator.result()
                if self.view_img:
                    if platform.system() == 'Linux' and p not in windows:
                        windows.append(p)
                        cv2.namedWindow(str(p), cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)
                        cv2.resizeWindow(str(p), im0.shape[1], im0.shape[0])
                    cv2.imshow('detection', im0)
                    cv2.waitKey(1)

                if not (label == None):
                    t = datetime.datetime.now()
                    result_path = str(save_file) + str(t)[:-7] + '.png'
                    self.t = timeee.perf_counter()

                    if (self.t - self.lastinserttime > 2):
                        self.insert(label, result_path)
                        cv2.imwrite(result_path, im0)
                self.yoloimg = self.cv_bridge.cv2_to_imgmsg(im0, 'bgr8')
                self.yolo_pub.publish(self.yoloimg)

        t = tuple(x.t / seen * 1E3 for x in dt)

    def timer_callback(self):
        pass


def run():

    rclpy.init()
    yolo_node = node_yolo("topic_webcam_yolo_sub")
    rclpy.spin(yolo_node)
    yolo_node.destroy_node()
    rclpy.shutdown()
