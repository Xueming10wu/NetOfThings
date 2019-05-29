#!/usr/bin/env python         
# -*- coding: utf-8 -*-

import time
import serial
from Agreement import Agreement

class Node(object):
    '''
    id_;//设备编号，固定8位二进制
    function_;//提供的功能，固定16位二进制
    length_;  //表长度，保存网络中当前节点的个数
    routingList_;//路由表，保存网络中的设备地址 8位二进制
    functionList_;//功能表，保存网络中，某个设备的功能编号 16位二进制

    Agreement agreement = Agreement();//协议对象，用于通讯的解码和编码
    '''

    def __init__(self, id, func, portname, baudrate = 9600):
        #初始化，需要节点编号，功能编号，串口名称，与波特率信息
        self.__id = id
        self.__function = func
        self.__length = 0
        self.__routingList = []
        self.__functionList = []
        self.__agreement = Agreement()
        try:
            self.__serialPort = serial.Serial(portname, baudrate)
            self.__serialPort.writeTimeout = 2 #写超时
            print ("串口打开成功%s"%portname),
            print (",波特率为%s"%baudrate)
        except Exception as e:
            print ("串口被占用，请重新启动")


    def getSerialPort(self):
        #返回串口对象
        print ("getSerialPort")
        return self.__serialPort
    

    def update(self):
        #主节点将会发送刷新命令，将会把所有的路由表、功能表都清空，长度设置为0
        print ("update")
        self.__length = 0
        self.__routingList = []
        self.__functionList = []
        #给type字段设置为 0x00编号
        self.__agreement.source = self.__id
        self.__agreement.destination = 0xff
        self.__agreement.type = 0x00
        self.__agreement.data_0 = 0x00
        self.__agreement.data_1 = 0x00
        #发送消息
        self.__serialPort.write(self.__agreement.encode())
        time.sleep(0.01)


    def addList(self):
        #主节点收到设备的信息(0x01)后，先把信息添加到主节点的表中，然后将会把设备的信息添加到表中，并广播给所有的节点(0x02)
        print ("addList")
        if self.__agreement.source in self.__routingList:
            #如果表中已经有这个设备的信息的话，就不再添加
            pass
        else:
            #添加到表中
            self.__length += 1
            self.__routingList.append(self.__agreement.source)
            self.__functionList.append((self.__agreement.data_0 << 4) + self.__agreement.data_1 )
            #广播信息，更新到别的设备
            self.__agreement.type = 0x02
            self.translate()
    

    def sendInfo(self):
        #将主节点的设备信息发布出去
        print ("sendInfo")
        self.__agreement.source = self.__id
        self.__agreement.destination = 0xff
        self.__agreement.type = 0x02
        self.__agreement.data_0 = self.__function & 0x1100
        self.__agreement.data_1 = self.__function & 0x0011
        self.__serialPort.write(self.__agreement.encode())
        time.sleep(0.01)
    

    def translate(self):
        #数据转发，当收到sendData之后，将数据转发走
        print ("translate")
        self.__serialPort.write(self.__agreement.encode())
        time.sleep(0.01)


    def listening(self, s):
        #监听，并判断该执行什么任务
        self.__agreement.decode(s)
        if( self.__agreement.type == 0x01 ):
            #添加路由表
            self.addList()
        elif( self.__agreement.type == 0xff ):
            #转发数据
            self.translate()
        else:
            print("其他消息")
            
    
    def printInfo(self):
        #打印出当前节点状态
        print("\n\nid %s,"%self.__id),
        print("function %s"%self.__function)
        print("routingList :"),
        for i in range(0, self.__length):
            print(self.__routingList[i]),
        print("\nfunctionList :"),
        for i in range(0, self.__length):
            print(self.__routingList[i]),
        print("")