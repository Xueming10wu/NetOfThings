#include "Agreement.h"

Agreement::Agreement() : sourceSize(1), destinationSize(1), typeSize(1), dataSize(2)
{
    sumSize = sourceSize + destinationSize + typeSize + dataSize;
    source = 0x00;
    destination = 0x00;
    type = 0x0;
    data_0 = 0x00;
    data_1 = 0x00;
};

unsigned char * Agreement::encode()
{
    unsigned char * message = new unsigned char[sumSize];
    message[0] = char(source);
    message[1] = char(destination);
    message[2] = char(type);
    message[3] = char(data_0);
    message[4] = char(data_1);
    return message;
};

void Agreement::decode(unsigned char * message)
{
    source = int(message[0]);
    destination = int(message[1]);
    type = int(message[2]);
    data_0 = int(message[3]);
    data_1 = int(message[4]);
};

