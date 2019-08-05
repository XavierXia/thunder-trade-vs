#include "nn.hpp"

#include <nanomsg/pair.h>
#include <cassert>
#include <iostream>
#include <unistd.h>
using namespace std;

#define SOCKET_ADDRESS1 "inproc://test"
#define SOCKET_ADDRESS2 "tcp://47.105.111.100:8000"
#define SOCKET_ADDRESS3 "tcp://*:8000"
#define SOCKET_ADDRESS4 "ipc:///tmp/pair.ipc"

int main ()
{
    nn::socket s2;
    s2.socket_s(AF_SP, NN_PAIR);
    s2.connect(SOCKET_ADDRESS2);
    char buf[25];

    //s2.send ("...CCC,client send\n", 19, 0);
    // int rc = s2.recv (buf, sizeof (buf), 0);
    // cout<<"...buf,client recv: " << buf << endl;

    while(1)
    {
        // s2.send ("...CCC,client send\n", 19, 0);
        // sleep(2);
        int rc = s2.recv (buf, sizeof (buf), 0);
        cout<<"...buf,client recv: " << buf << endl;
        sleep(1);
    }
    return 0;
}