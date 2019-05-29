
#include <dht11.h>
#include <Arduino.h>
#include <Node.h>

const int DHT11_Pin = 6;
const int Light_Pin = A0;
const int Wine_Pin = A1;

void setup()
{
  Serial.begin(9600);
  pinMode(DHT11_Pin, INPUT);
}

int main()
{
  init();
  setup();
  dht11 DHT11;


  int id = 0x01, func = 0x001b;
  Node node = Node(id, func);
  char *s = new char[node.getAgreement().sumSize];
  int downloaddata = 0x0000;
  while (true)
  {
    if (Serial.available() > 0) //如果有网络数据来的话
    { //下行数据代码区
      for (int i = 0 ; i < 5 ; i ++)
      { //读取5个字符
        char a = Serial.read();
        if (a == '\n')
        {
          break;
        }
        s[i] = a;
      }
      if ( (!node.listening( s )))
      { //如果接收的数据不是用于组网的，那么就把数据部分保存下来
        //检查一下数据源是否通过过滤器
        if (node.filter())
        {
          downloaddata = node.getAgreement().data_0 << 4 + node.getAgreement().data_1;
        }
        else
        {
          node.sendData(0);
        }
      }
    }
    else
    {
      //上行数据代码区域和普通执行区域
      int chk = DHT11.read(DHT11_Pin);//读数据
      
      //无限制广播到所有节点上
      int temperature = DHT11.temperature;//温度0x0
      int t = temperature + (0x0 << 10) + (0x0 << 12);
      node.sendData(t, 0x00);

      int humidity = DHT11.humidity;//湿度0x1
      int h = humidity + (0x0 << 10) + (0x1 << 12);
      node.sendData(h, 0x01);

      //测试节点表长
      //int len = node.getLength();
      //node.sendData(l, 0x02);

      int wine = analogRead(Wine_Pin);
      int w = wine + (0x0 << 10) + (0x4 << 12);
      node.sendData(w, 0x02);
      
      int light = analogRead(Light_Pin);
      int l = light + (0x0 << 10) + (0x3 << 12);
      node.sendData(l, 0x03);
      delay(2000);
    }
    

  }
  return 0;
}
