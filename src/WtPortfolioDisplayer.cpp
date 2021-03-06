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
    addColumnHeadingToPortfolioTable();
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

void WtPortfolioDisplayer::addColumnHeadingToPortfolioTable()
{
    appendRowToTableWith(TableHeader);
    makeFirstRowOfTableHeader();
}

void WtPortfolioDisplayer::makeFirstRowOfTableHeader()
{
    m_currentPortfolioTable->setHeaderCount(1);
}

void WtPortfolioDisplayer::addHoldingVectorToTable(
        const std::vector<std::vector<std::string>>& holdings)
{
    for (const auto& holding: holdings)
    {
        appendRowToTableWith(holding);
    }
}

void WtPortfolioDisplayer::appendRowToTableWith(
                    const std::vector<std::string>& recordRow)
{
    int rowNumber{nextRowNumber()};
    for (int i{0}; i < recordRow.size(); ++i)
    {
        addCellToPortfolioTable(rowNumber, i, recordRow[i]);
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

void WtPortfolioDisplayer::setPortfolioPointerWith(
        std::unique_ptr<FinancialPortfolio> portfolioPointer)
{
    m_portfolioPtr = std::move(portfolioPointer);
}

void WtPortfolioDisplayer::updateScreen()
{
    addHoldingVectorToTable(m_portfolioPtr->holdingsInAllAccounts());
}

const std::string WtPortfolioDisplayer::Title("Financial Portfolio");
const std::string WtPortfolioDisplayer::Subtitle("My Portfolio");
const std::vector<std::string> WtPortfolioDisplayer::TableHeader{
    "Ticker",
    "Position",
    "Avg Price",
    "Current Price",
    "P&L"
};
