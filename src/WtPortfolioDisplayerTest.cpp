#include "Date.h"
#include "FinancialPortfolio.h"
#include "WtPortfolioDisplayer.h"
#include <gmock/gmock.h>
using namespace testing;

TEST(AWtPortfolioDisplayer, SetsUpTitleOfApplication)
{
    Wt::Test::WTestEnvironment testEnvironment;
    WtPortfolioDisplayer displayer(testEnvironment);

    ASSERT_THAT(displayer.title(), Eq(WtPortfolioDisplayer::Title));
}

TEST(AWtPortfolioDisplayer, HasTextWidget)
{
    Wt::Test::WTestEnvironment testEnvironment;
    WtPortfolioDisplayer displayer(testEnvironment);

    ASSERT_THAT(displayer.m_subtitle->text(), 
                            Eq(WtPortfolioDisplayer::Subtitle));
}

TEST(AWtPortfolioDisplayer, HasTableToShowCurrentPortfolio)
{
    Wt::Test::WTestEnvironment testEnvironment;
    WtPortfolioDisplayer displayer(testEnvironment);

    auto pointerToFirstHeader = dynamic_cast<Wt::WText*>(
            displayer.m_currentPortfolioTable->elementAt(0,0)->widget(0));

    ASSERT_THAT(pointerToFirstHeader->text(), 
                        Eq(WtPortfolioDisplayer::TableHeader[0]));
}

