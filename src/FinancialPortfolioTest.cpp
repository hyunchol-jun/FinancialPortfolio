#include <gtest/gtest.h>

class FinancialPortfolio
{
public:
    bool isEmpty() const
    {
        return true;
    }
};

TEST(AFinancialPortfolio, IsEmptyWhenCreated)
{
    FinancialPortfolio portfolio;
    ASSERT_TRUE(portfolio.isEmpty());
}
