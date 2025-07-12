#include <gtest/gtest.h>
#include <memory>

#include "chatClient.hpp"

using namespace testing;

class ChatClientTest : public ::testing::Test
{
public:
    void SetUp() override
    {
        sut_ = std::make_unique<ChatClient>();
    }

    void TearDown() override
    {
        sut_.reset();
    }

    void setPortExpect(const int portValue) {
        ;
    }

    std::unique_ptr<ChatClient> sut_;
};

TEST_F(ChatClientTest, setPortOk)
{
    setPort(8080);
    sut_->setPort();
}
