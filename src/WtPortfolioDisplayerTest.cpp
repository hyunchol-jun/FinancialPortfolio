#include <Wt/Test/WTestEnvironment.h>
#include <Wt/WEnvironment.h>
#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <memory>
#include <string>

class WtPortfolioDisplayer : public Wt::WApplication
{
public:
    static const std::string Title;
    static const std::string Subtitle;
    static const std::string DateColumn;
    WtPortfolioDisplayer(const Wt::WEnvironment& env);

    Wt::WText* m_subTitle;
    Wt::WTable* m_currentPortfolioTable;
private:
};

WtPortfolioDisplayer::WtPortfolioDisplayer(const Wt::WEnvironment& env)
    : WApplication(env)
{
    setTitle(Title);
    m_subTitle = 
        root()->addWidget(std::make_unique<Wt::WText>(Subtitle));
    m_currentPortfolioTable =
        root()->addWidget(std::make_unique<Wt::WTable>());
    m_currentPortfolioTable->elementAt(0, 0)
              ->addWidget(std::make_unique<Wt::WText>(DateColumn));
};
const std::string WtPortfolioDisplayer::Title("Financial Portfolio");
const std::string WtPortfolioDisplayer::Subtitle("My Portfolio");
const std::string WtPortfolioDisplayer::DateColumn("Date");


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

    ASSERT_THAT(displayer.m_subTitle->text(), 
                            Eq(WtPortfolioDisplayer::Subtitle));
}

TEST(AWtPortfolioDisplayer, HasTableToShowCurrentPortfolio)
{
    Wt::Test::WTestEnvironment testEnvironment;
    WtPortfolioDisplayer displayer(testEnvironment);

    auto firstColumnTextPointer = dynamic_cast<Wt::WText*>(
            displayer.m_currentPortfolioTable->elementAt(0,0)->widget(0));

    ASSERT_THAT(firstColumnTextPointer->text(), 
                        Eq(WtPortfolioDisplayer::DateColumn));
}
