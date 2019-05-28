#include <Arduino.h>
#include <Node.h>



void setup() 
{
  Serial.begin(9600);
}

int main()
{
  init();
  setup();
  
  int id = 0x05, func= 0x0001;
  Node node = Node(id, func);
  char *s = new char[5];
  while(true)
  {
    if(Serial.available() > 0) //如果有网络数据来的话
    {//下行数据代码区
      for(int i = 0 ; i < 5 ; i ++)
      {
        char a = Serial.read();
        if(a == '\n')
        {
          break;
        }
        s[i] = a;
      }
      node.listening( s );
    }
    //上行数据代码区域和普通执行区域
    

    
  }
  return 0;
}
