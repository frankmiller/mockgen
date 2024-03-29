
#include "MockGenAction.h"

#include <clang/Tooling/Tooling.h>

#define BOOST_TEST_MODULE "MockGenTest"
#include <boost/test/included/unit_test.hpp>

#include <regex>

std::string input1 = R"__(
class Dum
{
public:
    Dum(int i) : _i(i) { }
    virtual ~Dum() {}
    int get() const { return _i; }
    void set(int i) { _i = i; }
    virtual void print() = 0;
    virtual void dum1( int a ) = 0;
    virtual void dum2( int a, double b ) = 0;
    virtual int const & dum3( int* a, const double& b ) = 0;
private:
    int _i;
};
)__";

std::string output1 = R"__(
MOCK_BASE_CLASS(MockDum, Dum)
{
    MOCK_METHOD(print, 0);
    MOCK_METHOD(dum1, 1);
    MOCK_METHOD(dum2, 2);
    MOCK_METHOD(dum3, 2);
};
)__";

void checkEqual(const std::string & lhs, const std::string & rhs)
{
    BOOST_CHECK_EQUAL(lhs, rhs);

    //if ( lhs != rhs )
    //{
    //    std::ofstream flhs( "lhs.txt" );
    //    std::ofstream frhs( "rhs.txt" );

    //    flhs << lhs;
    //    frhs << rhs;

    //    if ( flhs.is_open() && frhs.is_open() )
    //        std::cout << "comparison wirtten to lhs.txt and rhs.txt" << std::endl;
    //}

    std::regex linesRegex("^.*$");
    auto lit = std::sregex_iterator(lhs.begin(), lhs.end(), linesRegex);
    auto rit = std::sregex_iterator(rhs.begin(), rhs.end(), linesRegex);
    auto eit = std::sregex_iterator();

    for (; lit != eit && rit != eit; ++lit, ++rit)
    {
        std::string l = lit->str();
        std::string r = rit->str();
        BOOST_CHECK_EQUAL(l, r);
    }
}

BOOST_AUTO_TEST_CASE(Simple)
{
    std::ostringstream os;
    BOOST_CHECK(clang::tooling::runToolOnCode(new MockGenAction(os), input1));

    checkEqual(os.str(), output1);
}

