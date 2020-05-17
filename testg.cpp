#include <gtest/gtest.h>
#include <string>
#include "bulkmanager.h"

TEST(GOOGLE_TEST1, test1)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(2);
    man.newString("1", qq);
    man.newString("2", qq);
    ASSERT_TRUE(man.getBulk().size() == 0);
}

TEST(GOOGLE_TEST2, test2)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(7);
    man.newString("1", qq);
    man.newString("2", qq);
    man.newString("{", qq);
    ASSERT_TRUE(man.getBulk().size() == 0);
}

TEST(GOOGLE_TEST3, test3)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(3);
    man.newString("1", qq);
    man.newString("2", qq);
    ASSERT_TRUE(man.getBulk().output() == "1 2 ");
}

TEST(GOOGLE_TEST4, test4)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(6);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("{", qq);
    man.newString("2", qq);
    ASSERT_TRUE(man.getBulk().size() == 3);
    ASSERT_TRUE(man.getBulk().output() == "2 2 2 ");
}

TEST(GOOGLE_TEST5, test5)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(6);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("}", qq);
    man.newString("}", qq);
    ASSERT_TRUE(man.getBulk().size() == 3);
    ASSERT_TRUE(man.getBulk().output() == "2 2 2 ");
}

TEST(GOOGLE_TEST6, test6)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(6);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("{", qq);
    man.newString("2", qq);
    man.newString("}", qq);
    man.newString("}", qq);
    man.newString("}", qq);
    ASSERT_TRUE(man.getBulk().size() == 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}