#include <gtest/gtest.h>
#include <string>
#include "bulkmanager.h"
#include "async.h"

std::string openS = openStr();
std::string closeS = closeStr();

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
    man.newString(openS, qq);
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
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(openS, qq);
    man.newString("2", qq);
    ASSERT_TRUE(man.getBulk().size() == 3);
    ASSERT_TRUE(man.getBulk().output() == "2 2 2 ");
}

TEST(GOOGLE_TEST5, test5)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(6);
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(closeS, qq);
    man.newString(closeS, qq);
    ASSERT_TRUE(man.getBulk().size() == 3);
    ASSERT_TRUE(man.getBulk().output() == "2 2 2 ");
}

TEST(GOOGLE_TEST6, test6)
{
    std::shared_ptr<QueueString> qq;
    bulkManager man(6);
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(openS, qq);
    man.newString("2", qq);
    man.newString(closeS, qq);
    man.newString(closeS, qq);
    man.newString(closeS, qq);
    ASSERT_TRUE(man.getBulk().size() == 0);
}

TEST(GOOGLE_TEST7, test7)
{
    auto qq = std::make_shared<QueueString>();
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    async::receive(h, "1", 1, qq);
    async::receive(h2, "1\n", 2, qq);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15, qq);
    async::receive(h, "b\nc\nd\n}\n89\n", 11, qq);
    async::disconnect(h);
    async::disconnect(h2);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}