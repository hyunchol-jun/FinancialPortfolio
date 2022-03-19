cmake_minimum_required(VERSION 3.14)
project(FinancialPortfolio)

link_directories(/usr/local/lib)

include_directories($ENV{HOMEBREW_CELLAR}/boost/1.78.0/include/ 
                    $ENV{HOMEBREW_CELLAR}/nlohmann-json/3.10.4/include
                    /usr/local/include)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/0e402173c97aea7a00749e825b194bfede4f2e45.zip
    )

FetchContent_MakeAvailable(googletest)

enable_testing()

set(sources
    src/FinancialPortfolioTest.cpp
    src/FinancialPortfolio.cpp
    src/HoldingTest.cpp
    src/Holding.cpp
    src/CurlHttpTest.cpp
    src/CurlHttp.cpp
    src/DateTest.cpp
    src/WtPortfolioDisplayerTest.cpp
    src/WtPortfolioDisplayer.cpp
    src/Account.cpp
    src/AccountTest.cpp
    )
    
add_executable(
    FinancialPortfolioTest
    ${sources}
    )

find_package(CURL REQUIRED)

target_link_libraries(
    FinancialPortfolioTest
    gmock_main
    CURL::libcurl
    wthttp
    wt
    wttest
    )

include(GoogleTest)
gtest_discover_tests(FinancialPortfolioTest)
