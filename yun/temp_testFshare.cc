#include "gmock/gmock.h"

using namespace testing;

class SimpleClass {

public:
    virtual int simpleFirstFunction(int a, int b) { return (a + simpleSecondFunction(b)); }
    virtual int simpleSecondFunction(int b) { return (2 * b); }
    // virtual ~SimpleClass();
};


class MockSimpleClass : public SimpleClass {
public:
    // MOCK_METHOD1(int, simpleSecondFunction, (int b), (override));
    MOCK_METHOD2(simpleFirstFunction, int(int a, int b));
};

TEST(ServerUnit__TEST, ServerUnit__directory_check){
    MockSimpleClass mymock;
    EXPECT_CALL(mymock, simpleFirstFunction(_,_)).WillOnce(Return(100));
    // int result = mymock.simpleFirstFunction(2,1);
    SimpleClass a;
    int result = a.simpleFirstFunction(2,1);
    EXPECT_EQ(result, 4);
}

int main(int argc, char *argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}