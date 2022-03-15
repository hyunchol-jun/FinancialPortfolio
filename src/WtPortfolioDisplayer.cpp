#include "WtPortfolioDisplayer.h"

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
    appendRowToTableWith(TableHeader);
    makeFirstRowOfTableHeader();
}

void WtPortfolioDisplayer::makeFirstRowOfTableHeader()
{
    m_currentPortfolioTable->setHeaderCount(1);
}

void WtPortfolioDisplayer::appendRowToTableWith(
                    const std::vector<std::string>& recordRow)
{
    int rowNumber = nextRowNumber();
    int i{0};
    while (i < recordRow.size())
    {
        addCellToPortfolioTable(rowNumber, i, recordRow[i]);
        ++i;
    }
}

int WtPortfolioDisplayer::nextRowNumber()
{
    return m_currentPortfolioTable->rowCount();
}

void WtPortfolioDisplayer::addCellToPortfolioTable(
                int row, int column, const std::string& cellData)
{
    m_currentPortfolioTable->elementAt(row, column)
        ->addWidget(std::make_unique<Wt::WText>(cellData));
}

const std::string WtPortfolioDisplayer::Title("Financial Portfolio");
const std::string WtPortfolioDisplayer::Subtitle("My Portfolio");
const std::vector<std::string> WtPortfolioDisplayer::TableHeader{
    "Date",
    "Ticker",
    "Position",
    "Avg Price",
    "Current Price",
    "P&L"
};
