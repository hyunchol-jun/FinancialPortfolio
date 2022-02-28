cmake_minimum_required(VERSION 3.14)
project(FinancialPortfolio)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/0e402173c97aea7a00749e825b194bfede4f2e45.zip
    )

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    FinancialPortfolioTest
    src/FinancialPortfolioTest.cpp
    src/FinancialPortfolio.cpp
    )

target_link_libraries(
    FinancialPortfolioTest
    gmock_main
    )

include(GoogleTest)
gtest_discover_tests(FinancialPortfolioTest)
