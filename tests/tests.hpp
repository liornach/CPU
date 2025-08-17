#include <iostream>

static const int SUCCESS = 0;
static const int ERROR = 1;

struct TestContext
{
    size_t Pass, Err = 0;
};

static TestContext TextContext = TestContext();

static int AssertTrueImpl(bool res, int line, const std::string& file)
{
    if (!res)
    {
        std::cerr << file << ": Expected true, got false, (line " << line << ")" << std::endl;
        return ERROR;
    }

    return SUCCESS;
}

static int AssertFalseImpl(bool res, int line, const std::string& file)
{
    if (res)
    {
        std::cerr << file << ": Expected false, got true, (line " << line << ")" << std::endl;
        return ERROR;
    }

    return SUCCESS;
}

#define AssertTrue(expr) AssertTrueImpl((expr), __LINE__, __FILE__)
#define AssertFalse(expr) AssertFalseImpl((expr), __LINE__, __FILE__)
