#include "gtest/gtest.h"
#include "snapshot/snapshot.h"

#include "./custom_result/custom_result.h"

#include "result/result.h"
#include "result/types_check/is_result.h"
#include "result/types_check/is_result_or.h"

class TypesCheckTest : public testing::Test {
protected:
    virtual void SetUp() override {}
};

TEST_F(TypesCheckTest, is_result_test) {
    EXPECT_TRUE(result::internal::is_result_v<custom_result::CustomResult>);
}

TEST_F(TypesCheckTest, is_result_or_test) {
    EXPECT_TRUE(result::internal::is_result_or_v<custom_result::CustomResultOr<int>>);
}
