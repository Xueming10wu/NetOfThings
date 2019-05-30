#include "Node.h"

//节点初始化
Node::Node(int id, int func) : id_(id), function_(func)
{
    //std::cout << "Node\n";
    length_ = 0;
    update();
    Serial.begin(9600);
}

/*
void Node::printInfo() const
{
    std::cout << id_ << " " << length_ << " " << function_ << std::endl;
    for(int i = 0 ; i < length_ ; i ++)
    {
        std::cout << routingList_[i] << " " << functionList_[i] << std::endl;
    }
}
*/

//0x00 收到刷新命令，因为代价比较小，所以定时刷新，并重新建表
void Node::update()
{
    //std::cout << "update\n";
    if(length_ > 0)
    {
        delete routingList_;
        delete functionList_;
        length_ = 0;
    }
    routingList_ = NULL;
    functionList_ = NULL;
}


//0x01 收到刷新组网指令后，执行 发送信息指令
void Node::sendInfo()
{
    //std::cout << "sendInfo\n";
    agreement.source = id_;
    agreement.destination = 0x00;
    agreement.type = 0x01;
    agreement.data_0 = function_ >> 8;
    agreement.data_1 = function_ & 0x00ff;
    //std::cout << agreement.encode() << std::endl;
    const char *s = agreement.encode();
    for(int i = 0 ; i < agreement.sumSize ; i ++)
    {
        Serial.write(s[i]);
    }
    Serial.println();
    delete s;
    s = NULL;
}


//0x02 收到添加路由表指令后，执行
void Node::addList()
{
    //std::cout << "addList\n";

    bool flag = true;
    //表中不应包含自身节点的节点号
    if(agreement.source == id_ )
    {
        flag = false;
    }
    //检查是否重复
    for(int i = 0 ; i < length_; i++)
    {
        if(agreement.source == routingList_[i])
        {
            flag = false;
        }
    }

    if(flag)
    {
        //申请新的指针以及内存空间
        int *rp = new int[length_ + 1];
        int *fp = new int[length_ + 1];

        for(int i = 0; i < length_; i++)
        {
            //复制之前的数据
            rp[i] = routingList_[i];
            fp[i] = functionList_[i];
        }
        //在表末尾添加最新数据
        rp[length_] = agreement.source;
        fp[length_] = (agreement.data_0 << 8 )+ agreement.data_1;

        //改变表长
        length_ += 1;

        //删除之前的表
        delete routingList_;
        delete functionList_;

        //把指针指向新的内存地址
        routingList_ = rp;
        functionList_ = fp;
    }

    /*
    std::cout << "rout func\n";
    for(int i = 0 ; i < length_ ; i ++)
    {
        std::cout << routingList_[i] << " " << functionList_[i] << std::endl;
    }
    */
}


//监听消息，并解析消息来委派的组网任务，如果有特殊任务，那么返回true，正常任务返回false
bool Node::listening(unsigned char *s)
{
    //用协议解码
    agreement.decode(s);
    //根据type字段来进行判断到底执行哪些动作
    switch(agreement.type)
    {
    case 0x00:
        //刷新
        update();
        //发送节点信息
        sendInfo();
        break;
    case 0x02:
        //向表里添加信息
        addList();
        break;
    default:
        //属于正常任务
        return false;
    }
    return true;
}


void Node::sendData(int data)
{
    //std::cout << "sendData\n";
    agreement.source = id_;
    agreement.type = 0xff;
    agreement.data_0 = data >> 8;
    agreement.data_1 = data & 0x00ff;
    for(int i = 0 ; i < length_; i++)
    {
        if( function_ & functionList_[i] > 0 )
        {
            agreement.destination = routingList_[i];
            //std::cout << agreement.encode() << std::endl;
            const char *s = agreement.encode();
            for(int i = 0 ; i < agreement.sumSize ; i ++)
            {
                Serial.write(s[i]);
            }
            Serial.println();
            delete s;
            s = NULL;
        }
    }
}


void Node::sendData(int data, int destination)
{
    //std::cout << "sendData\n";
    agreement.source = id_;
    agreement.destination = destination;
    agreement.type = 0xff;
    agreement.data_0 = data >> 8;
    agreement.data_1 = data & 0x00ff;
    const char *s = agreement.encode();
    for(int i = 0 ; i < agreement.sumSize ; i ++)
    {
        Serial.write(s[i]);
    }
    Serial.println();
    delete s;
    s = NULL;
}


//过滤器，如果消息源头有效，那么返回true。否则返回false
bool Node::filter() const
{
    if(getAgreement().source != id_)
    {//判断是否出现链路回传。如果是回传，那么拒收消息
        if((getAgreement().destination == id_ )|| (getAgreement().destination == 0xff))
        {//如果消息的目标是本节点编号，或者是多播类型，那么接收消息
            return true;
        }
    }
    return false;
}

//测试


Node::~Node()
{
}
