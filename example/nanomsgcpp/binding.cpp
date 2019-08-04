#include "nn.hpp"

#include <nanomsg/pair.h>

#include <cassert>

int main ()
{
    nn::socket s1 (AF_SP, NN_PAIR);
    s1.bind ("inproc://a");
    nn::socket s2 (AF_SP, NN_PAIR);
    s2.connect ("inproc://a");

    s2.send ("ABC", 3, 0);
    char buf [3];
    int rc = s1.recv (buf, sizeof (buf), 0);
    assert (rc == 3);

    return 0;
}