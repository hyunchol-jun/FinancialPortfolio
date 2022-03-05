#ifndef JsonParser_h
#define JsonParser_h
#include <nlohmann/json.hpp>
#include <string>
using namespace nlohmann;

class JsonParser
{
public:
    JsonParser(const std::string& jsonString) 
            : m_jsonData{json::parse(jsonString)}
    {
    }
    double currentPrice() 
    {
        return static_cast<double>(
               m_jsonData["chart"]["result"][0]["meta"]["regularMarketPrice"]); 
    }
private:
    json m_jsonData{};
};
#endif
