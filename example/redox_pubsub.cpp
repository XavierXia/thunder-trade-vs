#include <stdlib.h>
#include <iostream>
#include "redox.hpp"

using namespace std;


/*
说明：本demo主要作用是 说明Zpquant与三方策略通信的数据格式实现细节
*/
int main(int argc, char *argv[]) {


  int cnt = 0;
  redox::Subscriber subscriber;
  if(!subscriber.connect()) return 1;
  
  redox::Redox publisher;
  if(!publisher.connect()) return 1;

/*
...subscribe,topic:mds_data,msg: {"msgType":22, "sendDCT":52505850, "LastRecvT":1563258905, 
"mktData":{"exchId":1,"securityType":1,"tradeDate":20190430,"TransactTime":131510450,"ChannelNo":6,
"ApplSeqNum":1661235,"SecurityID":"601881","ExecType":"F","TradeBSFlag":"S","TradePrice":116000,
"TradeQty":28300,"TradeMoney":3282800000,"BidApplSeqNum":2855252,"OfferApplSeqNum":2855966,
"__origTickSeq":87804806,"__channelNo":4,"__origNetTime":131512288,"__recvTime":1563258905.839050,
"__collectedTime":1563258905.839050,"__processedTime":1563258905.839084,"__pushingTime":1563258905.839124}}
*/
  subscriber.subscribe("mds_data", [&cnt,&publisher](const string& topic, const string& msg) {
      cout << "...client...subscribe,topic:" << topic << ",msg: " << msg << endl;
      cout << "...cnt: " << cnt << endl;
      string str;

      if(cnt >= 20 && cnt < 41) //查询持仓实例
      {
          switch(cnt)
          {
            case 20: //查询 客户端总览信息
            {
              str = "{\"type\":\"query\",\"category\":\"clientOverview\",\"code\":\"\",\"sclb\":\"\"}";
              break;
            }
            case 25: //查询 所有关联资金账户的资金信息
            {
              str = "{\"type\":\"query\",\"category\":\"cashAsset\",\"code\":\"\",\"sclb\":\"\"}";
              break;
            }
            case 30: //查询 指定上证 600000 的产品信息
            {
              str = "{\"type\":\"query\",\"category\":\"stkInfo\",\"code\":\"600000\",\"sclb\":\"2\"}";
              break;
            }
            case 35: //查询 上证 600000 股票的持仓
            {
              str = "{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"600000\",\"sclb\":\"2\"}";
              break;
            }
            case 40: //查询 沪深两市 所有股票持仓
            {
              str = "{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"\",\"sclb\":\"\"}";
              break;
            }
            default:
              break;
          }
      	  cout << "...query" << endl;
          publisher.publish("order2server", str);
      }

      if(cnt >= 80 && cnt < 121)
      {
          switch(cnt)
          {
            case 80: //* 以 12.67元 购买 浦发银行(600000) 200股 */
            {
              str = "{\"type\":\"buy\",\"code\":\"600000\",\"sclb\":\"1\",\"wtfs\":\"0\",\"amount\":\"100\",\"price\":\"126700\"}";
              break;
            }
            case 90: //* 以 市价 卖出  浦发银行(600000) 100股 */
            {
              str = "{\"type\":\"sell\",\"code\":\"600000\",\"sclb\":\"1\",\"wtfs\":\"1\",\"amount\":\"100\",\"price\":\"0\"}";
              break;
            }
            case 100: ///* 以 12.67元 购买 平安银行(000001) 200股 */
            {
              str = "{\"type\":\"buy\",\"code\":\"000001\",\"sclb\":\"2\",\"wtfs\":\"0\",\"amount\":\"200\",\"price\":\"126700\"}";
              break;
            }
            case 110: ///* 以 市价 卖出 平安银行(000001) 100股 */
            {
              str = "{\"type\":\"sell\",\"code\":\"000001\",\"sclb\":\"2\",\"wtfs\":\"1\",\"amount\":\"100\",\"price\":\"0\"}";
              break;
            }
            default:
              break;
          }
          cout << "...buy" << endl;
          publisher.publish("order2server", str);
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
