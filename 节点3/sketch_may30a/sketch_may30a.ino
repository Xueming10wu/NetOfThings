#include <Arduino.h>
#include <Node.h>
//土壤传感器节点
const int earth_Pin = A1;

void setup()
{
  Serial.begin(9600);
}

int main()
{
  init();
  setup();

  int id = 0x03, func = 0x0004;
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
          downloaddata = (node.getAgreement().data_0 << 8) | node.getAgreement().data_1 ;
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
      //无限制广播到所有节点上
      //测试节点表长
      //int len = node.getLength();
      //node.sendData(l, 0x02);
      
      int earth_val = 1023 - analogRead(earth_Pin);
      int e = earth_val + (0x3 << 10) + (0x2 << 12);

      //Serial.println(earth_val);
      node.sendData(e, 0xff);
      delay(2000);
    }
    

  }
  return 0;
}
