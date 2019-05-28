#!/usr/bin/env python         
# -*- coding: utf-8 -*-
import time
import serial

class Agreement(object):
    "消息字段结构"
    __sourceSize = 1
    __destinationSize = 1
    __typeSize = 1
    __dataSize = 2
    __sumSize = __sourceSize + __destinationSize + __typeSize + __dataSize

    def __init__(self):
        self.source = 0x00
        self.destination = 0x00
        self.type = 0x00
        self.data_0 = 0x00
        self.data_1 = 0x00


    def encode(self):
        "编码 转成 字符串类型"
        message = ["" for _ in range(self.__sumSize)]
        message[0] = (chr(self.source))
        message[1] = (chr(self.destination))
        message[2] = (chr(self.type))
        message[3] = (chr(self.data_0))
        message[4] = (chr(self.data_1))
        s = ""
        for i in range(0,5): 
            s = s + message[i]
        '''
        print("encode %s"%message)
        print(self.source),
        print(self.destination),
        print(self.type),
        print(self.data_0),
        print(self.data_1)
        print(len(s))
        '''
        return s

    def decode(self, s):
        "解码 把 字符串数据解析存放在各个字段"
        message = list(s)
        #print(message)
        self.source = ord(message[0])
        self.destination = ord(message[1])
        self.type = ord(message[2])
        self.data_0 = ord(message[3])
        self.data_1 = ord(message[4])
        
        

def main():
    "测试代码"
    agreement = Agreement()
    serialport = serial.Serial("/dev/ttyACM0", 9600)

    agreement.source = 0x03
    agreement.destination = 0x0f
    agreement.type = 0xff
    agreement.data_0 = 0x36
    agreement.data_1 = 0x0f

    s = agreement.encode()
    print(s)
    time.sleep(2)

    t0 = time.clock()
    serialport.write(s)
    t1 = time.clock()
    print ( t1 - t0 )
    a = serialport.readline()
    print (a)
    agreement.decode(a)

    t2 = time.clock()
    
    print (t2 - t1)

    print(type(agreement.data_1))


if __name__ == '__main__':
    main()

