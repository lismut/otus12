#include <gtest/gtest.h>
#include <string>
#include "bulkmanager.h"
std::mutex globalCoutMutex;

TEST(GOOGLE_TEST1, test1)
{
    bulkManager man(2);
    man.newString("1");
    man.newString("2");
    ASSERT_TRUE(man.getBulk().size() == 0);
}

TEST(GOOGLE_TEST2, test2)
{
    bulkManager man(7);
    man.newString("1");
    man.newString("2");
    man.newString("{");
    ASSERT_TRUE(man.getBulk().size() == 0);
}

TEST(GOOGLE_TEST3, test3)
{
    bulkManager man(3);
    man.newString("1");
    man.newString("2");
    ASSERT_TRUE(man.getBulk().output() == "1 2 ");
}

TEST(GOOGLE_TEST4, test4)
{
    bulkManager man(6);
    man.newString("{");
    man.newString("2");
    man.newString("{");
    man.newString("2");
    man.newString("{");
    man.newString("2");
    ASSERT_TRUE(man.getBulk().size() == 3);
    ASSERT_TRUE(man.getBulk().output() == "2 2 2 ");
}

TEST(GOOGLE_TEST5, test5)
{
    bulkManager man(6);
    man.newString("{");
    man.newString("2");
    man.newString("{");
    man.newString("2");
    man.newString("{");
    man.newString("2");
    man.newString("}");
    man.newString("}");
    ASSERT_TRUE(man.getBulk().size() == 3);
    ASSERT_TRUE(man.getBulk().output() == "2 2 2 ");
}

TEST(GOOGLE_TEST6, test6)
{
    bulkManager man(6);
    man.newString("{");
    man.newString("2");
    man.newString("{");
    man.newString("2");
    man.newString("{");
    man.newString("2");
    man.newString("}");
    man.newString("}");
    man.newString("}");
    ASSERT_TRUE(man.getBulk().size() == 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}