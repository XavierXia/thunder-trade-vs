#include "nn.hpp"

#include <nanomsg/pair.h>
#include <cassert>
#include <iostream>
using namespace std;

int main ()
{
    nn::socket s2 (AF_SP, NN_PAIR);
    s2.connect ("inproc://a");
    s2.send ("...ABC,client", 13, 0);
    return 0;
}