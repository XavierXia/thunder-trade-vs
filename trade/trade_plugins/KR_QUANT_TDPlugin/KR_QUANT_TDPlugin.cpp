#include "KR_QUANT_TDPlugin.h"
#include <stdarg.h>
#include <thread>
#include "OrderRefResolve.h"
#include "AutoPend.h"
extern char ProcessName[256];
#define NOTIFY_LOGIN_SUCCEED {m_boolIsOnline = true; std::unique_lock<std::mutex> lk(m_mtxLoginSignal);m_cvLoginSignalCV.notify_all();}
#define NOTIFY_LOGIN_FAILED  {m_boolIsOnline = false;std::unique_lock<std::mutex> lk(m_mtxLoginSignal);m_cvLoginSignalCV.notify_all();}

const string CKR_QUANT_TDPlugin::s_strAccountKeyword = "maxcancelperday;";
#define NAME ("template_td")
date CKR_QUANT_TDPlugin::GetTradeday(ptime _Current)
{
	if (_Current.time_of_day() < time_duration(12, 0, 0, 0))//这个地方不要卡的太死
		return _Current.date();
	else
	{
		if (_Current.date().day_of_week().as_enum() == Friday)
			return _Current.date() + days(3);
		else
			return _Current.date() + days(1);
	}
}
CKR_QUANT_TDPlugin::CKR_QUANT_TDPlugin():m_abIsPending(false)
{

}

CKR_QUANT_TDPlugin::~CKR_QUANT_TDPlugin()
{

}

bool CKR_QUANT_TDPlugin::IsPedding()
{
	return m_abIsPending.load();
}

bool CKR_QUANT_TDPlugin::IsOnline()
{
	return true;
}

void CKR_QUANT_TDPlugin::IncreaseRefCount()
{
	m_intRefCount++;
}

void CKR_QUANT_TDPlugin::DescreaseRefCount()
{
	m_intRefCount--;
}

int CKR_QUANT_TDPlugin::GetRefCount()
{
	return m_intRefCount;
}

void CKR_QUANT_TDPlugin::CheckSymbolValidity(const unordered_map<string, string> & insConfig)
{
	
}

string CKR_QUANT_TDPlugin::GetCurrentKeyword()
{
	return NAME;
}

string CKR_QUANT_TDPlugin::GetProspectiveKeyword(const ptree & in)
{
	string retKey = NAME;
	return retKey;
}

void CKR_QUANT_TDPlugin::GetState(ptree & out)
{
	out.put("online", "true");
}

void CKR_QUANT_TDPlugin::TDInit(const ptree & in, MTradePluginContextInterface * pTradePluginContext, unsigned int AccountNumber)
{
	auto MaxCancelPerDayNode = in.find("maxcancelperday");
	if (MaxCancelPerDayNode != in.not_found())
		m_intInitAmountOfCancelChancesPerDay = atoi(MaxCancelPerDayNode->second.data().c_str());
	else
		m_intInitAmountOfCancelChancesPerDay = 400;

	m_dateTradeDay = GetTradeday(second_clock::universal_time());
	map<string, int>().swap(m_mapCancelAmount);
}

void CKR_QUANT_TDPlugin::TDHotUpdate(const ptree &)
{

}

void CKR_QUANT_TDPlugin::TimerHandler(boost::asio::deadline_timer* timer, const boost::system::error_code& err)
{
	
}

bool CKR_QUANT_TDPlugin::Start()
{
	CAutoPend pend(m_abIsPending);
	return true;
}

bool CKR_QUANT_TDPlugin::Stop()
{

	CAutoPend pend(m_abIsPending);
	return true;
}

void CKR_QUANT_TDPlugin::TDUnload()
{
	
	Stop();
}

void CKR_QUANT_TDPlugin::ShowMessage(severity_levels lv, const char * fmt, ...)
{
	char buf[512];
	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buf, 512, fmt, arg);
	va_end(arg);
	boost::log::sources::severity_logger< severity_levels > m_Logger;
	BOOST_LOG_SEV(m_Logger, lv) << ProcessName << ": " << buf << " [" << to_iso_string(microsec_clock::universal_time()) << "]";
}

TOrderRefIdType CKR_QUANT_TDPlugin::TDBasicMakeOrder(
	TOrderType ordertype,
	unordered_map<string, string> & instrument,
	TOrderDirectionType direction,
	TOrderOffsetType offset,
	TVolumeType volume,
	TPriceType LimitPrice,
	TOrderRefIdType orderRefBase)
{
	
	return orderRefBase;
}

TLastErrorIdType CKR_QUANT_TDPlugin::TDBasicCancelOrder(TOrderRefIdType Ref, unordered_map<string, string> & instrument, TOrderSysIdType orderSysId)
{
	return LB1_NO_ERROR;
}

int CKR_QUANT_TDPlugin::TDGetRemainAmountOfCancelChances(const char * ins)
{
	return std::numeric_limits<int>::max();
}

