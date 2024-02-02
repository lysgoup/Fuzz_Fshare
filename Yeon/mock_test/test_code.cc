#include "gmock/gmock.h"
#include "target_program.h"

using namespace testing;

class SimpleClass {

public:
    virtual int send(int a, int b) = 0;
    virtual int simpleFirstFunction(int a, int b) { return (a + simpleSecondFunction(b)); }
    virtual int simpleSecondFunction(int b) { return (2 * b); }
    // virtual ~SimpleClass();
};


class MockSimpleClass : public SimpleClass {
public:
    // MOCK_METHOD1(int, simpleSecondFunction, (int b), (override));
    MOCK_METHOD2(simpleFirstFunction, int(int a, int b));
    MOCK_METHOD2(send, int(int a, int b));
};

TEST(ServerUnit__TEST, ServerUnit__directory_check){
    MockSimpleClass mymock;
    EXPECT_CALL(mymock, send(2, 3))
        .WillOnce(::testing::Return(5));
    char a [100] = "Hi";
    get_list(a);
}

int main(int argc, char *argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}