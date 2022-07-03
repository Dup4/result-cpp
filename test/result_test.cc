#include "gtest/gtest.h"
#include "snapshot/snapshot.h"

class ResultTest : public testing::Test {
protected:
    virtual void SetUp() override {}
};

TEST_F(ResultTest, hello_world_test) {
    EXPECT_EQ("Hello World", "Hello World");
}
