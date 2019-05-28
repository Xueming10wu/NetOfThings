#ifndef NODE_H
#define NODE_H

//在arduino中，换成#include <Arduino.h>
#include <Arduino.h>
//#include <iostream>


#include <Agreement.h>


class Node
{
    public:
        Node(int id, int func);
        virtual ~Node();

        int getId() const{return id_;}
        int getFunc() const{return function_;}
        int getLength() const{return length_;}
        Agreement getAgreement() const{return agreement;}

        //void printInfo() const;

        //因为代价比较小，所以定时刷新，并重新建表
        void update();

        //收到刷新组网指令后，执行
        void sendInfo();

        //收到添加路由表指令后，执行
        void addList();

        //监听消息，并解析消息来的特殊委派任务
        bool listening(unsigned char *s);

        //发送常规传感器数据，data字段的前4位确定传感器类型，后12位，确定具体的数据
        void sendData(int data);

    private:
        const int id_;//设备编号，固定8位二进制
        const int function_;//提供的功能，固定16位二进制

        int length_;  //表长度，保存网络中当前节点的个数
        int * routingList_;//路由表，保存网络中的设备地址 8位二进制
        int * functionList_;//功能表，保存网络中，某个设备的功能编号 16位二进制

        Agreement agreement = Agreement();//协议对象，用于通讯的解码和编码

};

#endif // NODE_H
