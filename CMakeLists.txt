cmake_minimum_required(VERSION 3.14)
project(FinancialPortfolio)

include_directories($ENV{HOMEBREW_CELLAR}/boost/1.78.0/include/)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

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
    )
    
add_executable(
    FinancialPortfolioTest
    ${sources}
    )

target_link_libraries(
    FinancialPortfolioTest
    gmock_main
    )

include(GoogleTest)
gtest_discover_tests(FinancialPortfolioTest)
