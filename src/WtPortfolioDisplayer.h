#ifndef WtPortfolioDisplayer_h
#define WtPortfolioDisplayer_h
#include <Wt/Test/WTestEnvironment.h>
#include <Wt/WEnvironment.h>
#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <memory>
#include <string>
#include <utility>

class WtPortfolioDisplayer : public Wt::WApplication
{
public:
    static const std::string Title;
    static const std::string Subtitle;
    static const std::string FirstColumnName;
    WtPortfolioDisplayer(const Wt::WEnvironment& env);
    void populateWindow();
    void writeSubtitle(const std::string& subtitle);
    void createEmptyPortfolioTable();
    void AddColumnHeadingToPortfolioTable();
    void appendRowToPortfolioTable(
                std::unique_ptr<Wt::WTableRow> tableRowPointer);

    Wt::WText* m_subtitle;
    Wt::WTable* m_currentPortfolioTable;
private:
};

WtPortfolioDisplayer::WtPortfolioDisplayer(const Wt::WEnvironment& env)
    : WApplication(env)
{
    setTitle(Title);
    populateWindow();
};

void WtPortfolioDisplayer::populateWindow()
{
    writeSubtitle(Subtitle);
    createEmptyPortfolioTable();
    AddColumnHeadingToPortfolioTable();
}

void WtPortfolioDisplayer::writeSubtitle(const std::string& subtitle)
{
    m_subtitle = 
        root()->addWidget(std::make_unique<Wt::WText>(subtitle));
}

void WtPortfolioDisplayer::createEmptyPortfolioTable()
{
    m_currentPortfolioTable =
        root()->addWidget(std::make_unique<Wt::WTable>());
}

void WtPortfolioDisplayer::AddColumnHeadingToPortfolioTable()
{
    m_currentPortfolioTable->elementAt(0, 0)
              ->addWidget(std::make_unique<Wt::WText>(FirstColumnName));
    m_currentPortfolioTable->elementAt(0, 1)
              ->addWidget(std::make_unique<Wt::WText>("Ticker"));
    m_currentPortfolioTable->elementAt(0, 2)
              ->addWidget(std::make_unique<Wt::WText>("Paid price"));
    m_currentPortfolioTable->elementAt(0, 3)
              ->addWidget(std::make_unique<Wt::WText>("Current Price"));
}

void WtPortfolioDisplayer::appendRowToPortfolioTable(std::unique_ptr<Wt::WTableRow> tableRowPointer)
{
    m_currentPortfolioTable->insertRow(0, std::move(tableRowPointer));

}

const std::string WtPortfolioDisplayer::Title("Financial Portfolio");
const std::string WtPortfolioDisplayer::Subtitle("My Portfolio");
const std::string WtPortfolioDisplayer::FirstColumnName("Date");

#endif
