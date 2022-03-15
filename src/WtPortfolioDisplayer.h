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

    Wt::WText* m_subtitle;
    Wt::WTable* m_currentPortfolioTable;
private:
    void AddColumnHeadingToPortfolioTable();
    void addCellToPortfolioTable(int row, int column);
    void makeFirstRowOfTableHeader();
    int newRowNumberToAppend();
    void fillInTableCellsOf(int rowNumber);
};

#endif