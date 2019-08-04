#include "nn.hpp"

#include <nanomsg/pair.h>
#include <cassert>
#include <iostream>
using namespace std;
#define ADDRESS2 "ipc:///tmp/reqrep.ipc"

int main ()
{
    nn::socket s1 (AF_SP, NN_PAIR);
    s1.bind (ADDRESS2);

    char buf [20];
    while(1)
    {
        int rc = s1.recv (buf, sizeof (buf), 0);
        cout<<"...buf: " << buf << endl;
    };
    return 0;
}