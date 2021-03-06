#ifndef WtPortfolioDisplayer_h
#define WtPortfolioDisplayer_h
#include "FinancialPortfolio.h"
#include <Wt/Test/WTestEnvironment.h>
#include <Wt/WEnvironment.h>
#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class WtPortfolioDisplayer : public Wt::WApplication
{
public:
    static const std::string Title;
    static const std::string Subtitle;
    static const std::vector<std::string> TableHeader;
    WtPortfolioDisplayer(const Wt::WEnvironment& env);
    void populateWindow();
    void writeSubtitle(const std::string& subtitle);
    void createEmptyPortfolioTable();
    void setPortfolioPointerWith(
                    std::unique_ptr<FinancialPortfolio> portfolioPointer);
    void updateScreen();

    Wt::WText* m_subtitle;
    Wt::WTable* m_currentPortfolioTable;
private:
    void addColumnHeadingToPortfolioTable();
    void addCellToPortfolioTable(
                        int row, int column, const std::string& cellData);
    void makeFirstRowOfTableHeader();
    int nextRowNumber();
    void fillInTableCellsOf(int rowNumber);
    void appendRowToTableWith(const std::vector<std::string>& recordRow);
    void addHoldingVectorToTable(
                    const std::vector<std::vector<std::string>>& holdings);

    std::unique_ptr<FinancialPortfolio> m_portfolioPtr;
};

#endif
