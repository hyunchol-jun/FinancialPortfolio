#include <gtest/gtest.h>

class FinancialPortfolio
{
public:
    FinancialPortfolio()
    {
        m_empty = true;
    }

    bool isEmpty() const
    {
        return m_empty;
    }

    void purchase(const std::string& ticker, int quantity)
    {
        m_empty = false;
    }
private:
    bool m_empty{};
};

class AFinancialPortfolio: public ::testing::Test
{
protected:
    FinancialPortfolio portfolio;
};

TEST_F(AFinancialPortfolio, IsEmptyWhenCreated)
{
    ASSERT_TRUE(portfolio.isEmpty());
}

TEST_F(AFinancialPortfolio, IsNotEmptyAfterPurchase)
{
    portfolio.purchase("IBM", 1);

    ASSERT_FALSE(portfolio.isEmpty());
}
