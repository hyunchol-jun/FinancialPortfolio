#include "CurlHttp.h"
#include <gmock/gmock.h>
using namespace testing;

TEST(CurlHttp, DISABLED_ReturnsResponseFromGetRequest)
{
    const std::string ValidTicker{"ibm"};
    const std::string ValidTimestamp{"201646451256"};
    const std::string ValidInterval{"1d"};
    CurlHttp curlHttp;
    curlHttp.initialize();
    std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/"
                    + ValidTicker + "?period1=%" + ValidTimestamp + "&period2=" 
                    + ValidTimestamp + "&interval=" + ValidInterval 
                    + "&events=history";
    
    auto response = curlHttp.get(url);

    ASSERT_THAT(response, HasSubstr("regularMarketPrice"));
}


