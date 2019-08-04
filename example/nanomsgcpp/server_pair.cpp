#include "nn.hpp"

#include <nanomsg/pair.h>
#include <cassert>
#include <iostream>
using namespace std;

int main ()
{
    nn::socket s1 (AF_SP, NN_PAIR);
    s1.bind ("inproc://a");

    char buf [20];
    int rc = s1.recv (buf, sizeof (buf), 0);
    cout<<"...buf: " << buf << endl;

    while(1)
    {

    };
    return 0;
}