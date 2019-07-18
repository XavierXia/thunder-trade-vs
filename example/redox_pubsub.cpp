#include <stdlib.h>
#include <iostream>
#include "redox.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp> 

using namespace std;
using namespace boost::property_tree;

/*
说明：本demo主要作用是 说明Zpquant与三方策略通信的数据格式实现细节
编译方式：
g++ redox_pubsub.cpp -o hello -std=c++11 -lredox -lev -lhiredis
./hello
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

      ptree c_Config;
      std::stringstream jmsg(msg.c_str());  
      try {
          boost::property_tree::read_json(jmsg, c_Config);
      }
      catch(std::exception & e){
          fprintf(stdout, "...client,cannot parse from string 'msg' \n");
          return 0;
      }

      auto msgType = c_Config.find("msgType");
      if(msgType != c_Config.not_found())
      {
        string msgT = msgType->second.data();
      }
 
      string securityID;
      string tradeBSFlag;
      string tradePrice;
      string tradeQty;
      char sendJsonDataStr[400];

      auto mktD = c_Config.find("mktData");
      if(mktD != c_Config.not_found())
      {
        auto securityIDNode = mktD->second.find("SecurityID");
        securityID = securityIDNode->second.data();

        auto tradeBSFlagNode = mktD->second.find("TradeBSFlag");
        tradeBSFlag = tradeBSFlagNode->second.data(); 

        auto tradePriceNode = mktD->second.find("TradePrice");
        tradePrice = tradePriceNode->second.data();

        auto tradeQtyNode = mktD->second.find("TradeQty");
        tradeQty = tradeQtyNode->second.data();            
      }

      if(cnt >= 20 && cnt < 79) //查询持仓实例
      {
        switch(cnt)
        {
          case 20: //查询 客户端总览信息
          {
            str = "{\"type\":\"query\",\"category\":\"clientOverview\",\"code\":\"\",\"sclb\":\"\"}";
            cout << "...query...cnt: " << cnt << endl;
            publisher.publish("order2server", str);
            break;
          }
          case 35: //查询 所有关联资金账户的资金信息
          {
            str = "{\"type\":\"query\",\"category\":\"cashAsset\",\"code\":\"\",\"sclb\":\"\"}";
            cout << "...query...cnt: " << cnt << endl;
            publisher.publish("order2server", str);
            break;
          }
          case 50: //查询 指定上证 601881 的产品信息
          {
            if(securityID == "601881")
            {
              sprintf(sendJsonDataStr, 
                "{\"type\":\"query\",\"category\":\"stkInfo\",\"code\":\"%s\",\"sclb\":\"2\"}",
                securityID.c_str());
            }
            cout << "...query...cnt: " << cnt << endl;
            publisher.publish("order2server", sendJsonDataStr);

            break;
          }
          case 65: //查询 上证 601881 股票的持仓
          {
            if(securityID == "601881")
            {
              sprintf(sendJsonDataStr, 
                "{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"%s\",\"sclb\":\"2\"}",
                securityID.c_str());
            }
            cout << "...query...cnt: " << cnt << endl;
            publisher.publish("order2server", sendJsonDataStr);

            break;
          }
          case 78: //查询 沪深两市 所有股票持仓
          {
            str = "{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"\",\"sclb\":\"\"}";
            cout << "...query...cnt: " << cnt << endl;
            publisher.publish("order2server", str);
            break;
          }
          default:
            break;
        }
      }

      if(cnt >= 80 && cnt < 121)
      {
          switch(cnt)
          {
            case 80: //* 以 12.67元 购买 601881 200股 */
            {
              if(securityID == "601881")
              {
                sprintf(sendJsonDataStr, 
                  "{\"type\":\"buy\",\"code\":\"%s\",\"sclb\":\"1\",\"wtfs\":\"0\",\"amount\":\"100\",\"price\":\"%s\"}",
                  securityID.c_str(),tradePrice.c_str());
              }
              cout << "...query...cnt: " << cnt << endl;
              publisher.publish("order2server", sendJsonDataStr);
              break;
            }
            case 90: //* 以 市价 卖出  601881 100股 */
            {
              if(securityID == "601881")
              {
                sprintf(sendJsonDataStr, 
                  "{\"type\":\"sell\",\"code\":\"%s\",\"sclb\":\"1\",\"wtfs\":\"0\",\"amount\":\"100\",\"price\":\"0\"}",
                  securityID.c_str());
              }
              cout << "...query...cnt: " << cnt << endl;
              publisher.publish("order2server", sendJsonDataStr);
              break;
            }
            case 100: ///* 以 12.67元 购买 平安银行(000001) 200股 */
            {
              if(securityID == "000001")
              {
                sprintf(sendJsonDataStr, 
                  "{\"type\":\"buy\",\"code\":\"%s\",\"sclb\":\"2\",\"wtfs\":\"0\",\"amount\":\"200\",\"price\":\"%s\"}",
                  securityID.c_str(),tradePrice.c_str());
              }
              cout << "...query...cnt: " << cnt << endl;
              publisher.publish("order2server", sendJsonDataStr);
              break;
            }
            case 110: ///* 以 市价 卖出 平安银行(000001) 100股 */
            {
              if(securityID == "000001")
              {
                sprintf(sendJsonDataStr, 
                  "{\"type\":\"sell\",\"code\":\"%s\",\"sclb\":\"2\",\"wtfs\":\"1\",\"amount\":\"100\",\"price\":\"0\"}",
                  securityID.c_str());
              }
              cout << "...query...cnt: " << cnt << endl;
              publisher.publish("order2server", sendJsonDataStr);

              break;
            }
            default:
              break;
          }
      }

      if(cnt > 150)
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
