#include "nn.hpp"

#include <nanomsg/pair.h>
#include <cassert>
#include <iostream>
#include <unistd.h>
using namespace std;

#define SOCKET_ADDRESS1 "inproc://test"
#define SOCKET_ADDRESS2 "tcp://192.168.1.36:7766"
#define SOCKET_ADDRESS3 "tcp://*:8000"
#define SOCKET_ADDRESS4 "ipc:///tmp/pair.ipc"

int main ()
{
    nn::socket s1;
    s1.socket_set(AF_SP, NN_PAIR);
    s1.bind (SOCKET_ADDRESS3);

    char buf [5000];
    while(1)
    {
        // int rc = s1.recv (buf, sizeof (buf), 0);
        // cout<<"...buf,server recv: " << buf << endl;
        // sleep(1);
        s1.send ("...SSS,server send\n", 19, 0);
        sleep(1);
    };
    return 0;
}