#include "nn.hpp"

#include <nanomsg/pair.h>
#include <cassert>
#include <iostream>
using namespace std;

#define ADDRESS1 "inproc://test"
#define ADDRESS2 "tcp://127.0.0.1:8000"
#define ADDRESS3 "ipc:///tmp/reqrep.ipc"

int main ()
{
    nn::socket s2 (AF_SP, NN_PAIR);
    s2.connect (ADDRESS3);
    
    while(1)
    {
        s2.send ("...ABC,client", 13, 0);
    }
    return 0;
}