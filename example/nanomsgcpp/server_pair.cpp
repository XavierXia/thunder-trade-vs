#include "nn.hpp"

#include <nanomsg/pair.h>
#include <cassert>
#include <iostream>
#include <unistd.h>
using namespace std;
#define ADDRESS2 "ipc:///tmp/reqrep.ipc"
#define SOCKET_ADDRESS1 "inproc://test"
#define SOCKET_ADDRESS2 "tcp://192.168.1.36:7766"
#define SOCKET_ADDRESS3 "tcp://*:7766"

int main ()
{
    nn::socket s1 (AF_SP, NN_PAIR);
    s1.bind (ADDRESS2);

    char buf [25];
    while(1)
    {
        int rc = s1.recv (buf, sizeof (buf), 0);
        cout<<"...buf,server recv: " << buf << endl;
        sleep(1);
        s1.send ("...SSS,server send\n", 19, 0);
    };
    return 0;
}