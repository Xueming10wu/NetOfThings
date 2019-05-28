#!/usr/bin/env python         
# -*- coding: utf-8 -*-
import time
from Node import Node

def main():
    "主函数"
    id = 0x0d
    func = 0x0000
    portname = '/dev/ttyACM0'
    baudrate = 9600
    node = Node(id, func, portname, baudrate)
    serialPort = node.getSerialPort()
    time.sleep(2)
    t0 = time.clock()
    updateDuration = 300
    duration = 1000
    while True:
        if (duration >= updateDuration):
            #定时刷新
            t0 = time.clock()
            node.update()
            node.sendInfo()
        if serialPort.in_waiting:
            #接收到数据的话，进行任务处理
            node.listening(serialPort.readline())
        time.sleep(1)
        #获得计时器数据
        duration = int((time.clock() - t0) * 10000)
        print("duration = %s"%duration)
    

if __name__ == '__main__':
    main()