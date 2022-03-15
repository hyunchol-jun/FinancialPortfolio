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
    appendRowToTableWith({"IBM", "5", "100.0", "125.0", "125.0"});
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

std::vector<std::string> WtPortfolioDisplayer::convertHoldingToStringVector(
        const std::string& ticker, const Holding& holding)
{
    std::vector<std::string> resultVector{ticker};
    int shareCount{holding.shareCount()};
    double avgPrice{holding.averagePurchasePrice()}, currPrice{125.0};
    resultVector.push_back(std::to_string(shareCount));
    resultVector.push_back(std::to_string(avgPrice));
    resultVector.push_back(std::to_string(currPrice));
    resultVector.push_back(
            std::to_string((currPrice-avgPrice) * shareCount));
    return resultVector;
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

const std::string WtPortfolioDisplayer::Title("Financial Portfolio");
const std::string WtPortfolioDisplayer::Subtitle("My Portfolio");
const std::vector<std::string> WtPortfolioDisplayer::TableHeader{
    "Ticker",
    "Position",
    "Avg Price",
    "Current Price",
    "P&L"
};
