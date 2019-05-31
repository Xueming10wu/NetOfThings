#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Node.h>

//土壤传感器
const int earth_Pin = A1;
//水泵针脚
const int pump_pin = 13;

const uint8_t rs = 2 , rw = 3 , enable = 4 ,
         d0 = 5,  d1 = 6,  d2 = 7,  d3 = 8,
         d4 = 9,  d5 = 10,  d6 = 11,  d7 = 12;
LiquidCrystal lcd = LiquidCrystal( rs,  rw,  enable, d0,  d1,  d2,  d3, d4,  d5,  d6,  d7);

void setup()
{
  Serial.begin(9600);
  pinMode(pump_pin, OUTPUT);
  digitalWrite(pump_pin, LOW);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("OK");
}

int main()
{
  init();
  setup();

  int id = 0x04, func = 0x1004;
  Node node = Node(id, func);
  char *s = new char[node.getAgreement().sumSize];
  int downloaddata = 0x0000;
  int goal = 0;
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
          
          //lcd.clear();
          //lcd.setCursor(0, 0);
          downloaddata = (node.getAgreement().data_0 << 8) | node.getAgreement().data_1 ;
          //lcd.print(downloaddata,BIN);
          /*
          lcd.setCursor(0, 1);
          lcd.print(node.getAgreement().data_1,BIN);
          */
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
      /*
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("goal: ");
      lcd.print(goal);
      lcd.setCursor(0, 1);
      lcd.print("native: ");
      lcd.print(earth_val);
      */

      if(goal > earth_val)
      { 
        digitalWrite(pump_pin, HIGH);
      }
      else
      {
        digitalWrite(pump_pin, LOW);
      }

      //lcd.clear();
      //lcd.setCursor(0, 0);
      //lcd.print(downloaddata,BIN);
      
      if((downloaddata & 0xf000) == 0x2000 )
      { 
        lcd.setCursor(0, 1);
        lcd.print("t ");
        if( (downloaddata & 0x0c00 ) == 0x0c00  )
        { 
          goal = (downloaddata & 0x03ff);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("goal: ");
          lcd.print(goal);
          lcd.setCursor(0, 1);
          lcd.print("real: ");
          lcd.print(earth_val);
        }
      }
      //int e = earth_val + (0x0 << 10) + (0x2 << 12);
      //node.sendData(e, 0xff);
      delay(2000);
    }
    

  }
  return 0;
}
