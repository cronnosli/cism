#include <gtest/gtest.h>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cism.h>
#include <system_call.h>
#include <cism_error.h>
#include <constants.h>

class SystemCallTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        systemCall = std::make_unique<execute::SystemCall>();
    }

    void TearDown() override
    {
    }

    std::unique_ptr<execute::SystemCall> systemCall;
};

TEST_F(SystemCallTest, SuccessfulExecution)
{
    std::string command = "/bin/printf \"\"";    
    EXPECT_NO_THROW(systemCall->run(command));
}

TEST_F(SystemCallTest, UnsuccessfulExecution)
{
    std::string command = "/bin/nonexistent";
    EXPECT_THROW({
        try
        {
            systemCall->run(command);
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_COULD_NOT_OPEN_SCRIPT, command);
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(SystemCallTest, CommandErrorReturn)
{
    std::string command = "/bin/sh -c \"exit 1\"";
    EXPECT_THROW({
        try
        {
            systemCall->run(command);
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_FAILED_RUN, command);
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(SystemCallTest, CommandNotFound)
{
    std::string command = "invalid_command";
    EXPECT_THROW({
        try
        {
            systemCall->run(command);
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_COULD_NOT_OPEN_SCRIPT, command);
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(SystemCallTest, EmptyCommand)
{
    std::string command = "";
    EXPECT_THROW({
        try
        {
            systemCall->run(command);
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_NO_COMMAND, "");
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}