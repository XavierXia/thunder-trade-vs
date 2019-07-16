#include <stdlib.h>
#include <iostream>
#include "redox.hpp"

using namespace std;

int main(int argc, char *argv[]) {


  int cnt = 0;
  redox::Subscriber subscriber;
  if(!subscriber.connect()) return 1;
  
  redox::Redox publisher;
  if(!publisher.connect()) return 1;

  subscriber.subscribe("mds_data", [&cnt,&publisher](const string& topic, const string& msg) {
     cout << "...subscribe,topic:" << topic << ",msg: " << msg << endl;
     cout << "...cnt: " << cnt << endl;
     if(cnt == 20)
     {
     	cout << "...querycc" << endl;
	publisher.publish("order2server", "querycc");
     }
     if(cnt == 100)
     {
     	cout << "...buy" << endl;
  	publisher.publish("order2server", "buy"); 
     }

     if(cnt == 160)
	{
     	cout << "...sell" << endl;
	publisher.publish("order2server", "sell");
	}
  
     if(cnt > 180)
     {
	cnt = 0;
     }
     else
     {
     	cnt++; 
     }
  });


  while(1)
  {
  }

  subscriber.disconnect();
  publisher.disconnect();
  return 0;
}
