#ifndef AGREEMENT_H
#define AGREEMENT_H

class Agreement
{
  public:
    Agreement();
    unsigned char* encode();
    void decode(unsigned  char * s );

    //每个数据都必须在 0~255的范围之内，否则将会出现溢出的现象
    int source;
    int destination;
    int type;
    int data_0;
    int data_1;

    const int sourceSize;
    const int destinationSize;
    const int typeSize;
    const int dataSize;
    int sumSize;

  private:
    
};

#endif // AGREEMENT_H
