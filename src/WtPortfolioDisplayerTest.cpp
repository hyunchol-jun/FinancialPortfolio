#include <string>

class WtPortfolioDisplayer
{
public:
    WtPortfolioDisplayer(std::string title){}

    std::string title()
    {
        return "Title";
    }
};

#include <gmock/gmock.h>
using namespace testing;

TEST(AWtPortfolioDisplayer, SetsUpTitleWhenCreated)
{
    WtPortfolioDisplayer displayer("Title");

    ASSERT_THAT(displayer.title(), Eq("Title"));
}
