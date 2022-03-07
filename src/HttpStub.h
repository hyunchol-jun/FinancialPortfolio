#ifndef HttpStub_h
#define HttpStub_h
#include "Http.h"
#include <gmock/gmock.h>
class HttpStub: public Http
{
public:
    MOCK_METHOD(void, initialize, ());
    MOCK_METHOD(std::string, get, (const std::string&), (const, override));
};
#endif
