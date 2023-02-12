#include <gtest/gtest.h>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <nlohmann/json.hpp>
#include <cism.h>
#include <call.h>
#include <cism_error.h>
#include <constants.h>

class MockCall : public execute::Call
{
public:
    bool run(std::string &)
    {
        return true;
    }
};

class CismTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        createFile();
    }

    void createFile()
    {
        cism = std::make_unique<console::Cism>(new MockCall());
        nlohmann::json configJson;
        nlohmann::json scripts;
        configJson["path"] = "scripts";
        scripts["build"] = "build.sh";
        configJson["scripts"] = scripts;
        std::ofstream configFile(".cism.json");
        configFile << configJson.dump();
        configFile.close();
    }

    void TearDown() override
    {
        std::remove(".cism.json");
        createFile();
    }

    void setMalformedConfig()
    {
        std::ofstream configFile(".cism.json");
        configFile << "invalid json";
        configFile.close();
    }

    void setScriptMissingConfig()
    {
        nlohmann::json configJson;
        nlohmann::json scripts;
        configJson["path"] = "scripts";
        std::ofstream configFile(".cism.json");
        configFile << configJson.dump();
        configFile.close();
    }

    void setPathMissingConfig()
    {
        nlohmann::json configJson;
        nlohmann::json scripts;
        scripts["build"] = "build.sh";
        configJson["scripts"] = scripts;
        std::ofstream configFile(".cism.json");
        configFile << configJson.dump();
        configFile.close();
    }

    std::unique_ptr<console::Cism> cism;
};

TEST_F(CismTest, Success)
{
    std::vector<std::string> arguments = {"cism", "build"};
    EXPECT_NO_THROW(cism->validate(arguments));
    EXPECT_NO_THROW(cism->run());
}

TEST_F(CismTest, SuccessWithParams)
{
    std::vector<std::string> arguments = {"cism", "build", "param1", "param2"};
    EXPECT_NO_THROW(cism->validate(arguments));
    EXPECT_NO_THROW(cism->run());
}

TEST_F(CismTest, ScriptNotFoundInConfig)
{
    std::vector<std::string> arguments = {"cism", "not_found"};
    EXPECT_THROW({
        try
        {
            cism->validate(arguments);
            cism->run();
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_SCRIPT_NOT_FOUND, "not_found");
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(CismTest, NoScriptSpecified)
{
    std::vector<std::string> arguments;
    EXPECT_THROW({
        try
        {
            cism->validate(arguments);
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_NO_SCRIPT, "");
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(CismTest, FailedToParseConfig)
{
    setMalformedConfig();
    std::vector<std::string> arguments = {"cism", "build"};
    EXPECT_THROW({
        try
        {
            cism->validate(arguments);
            cism->run();
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_FAILED_TO_PARSE, "./.cism.json");
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(CismTest, parseScriptMissingInConfig)
{
    setScriptMissingConfig();
    std::vector<std::string> arguments = {"cism", "build"};
    EXPECT_THROW({
        try
        {
            cism->validate(arguments);
            cism->run();
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_MISSING_SCRIPTS, "./.cism.json");
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(CismTest, parsePathMissingInConfig)
{
    setPathMissingConfig();
    std::vector<std::string> arguments = {"cism", "build"};
    EXPECT_THROW({
        try
        {
            cism->validate(arguments);
            cism->run();
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_MISSING_PATH, "./.cism.json");
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}

TEST_F(CismTest, openConfigFileFail)
{
    std::remove(".cism.json");
    std::vector<std::string> arguments = {"cism", "build"};
    EXPECT_THROW({
        try
        {
            cism->validate(arguments);
            cism->run();
        }
        catch (const CismError &e)
        {
            auto expected = CISM_GET_ERROR(CISM_ERROR_FAILED_TO_OPEN, "./.cism.json");
            EXPECT_STREQ(e.what(), expected.c_str());
            throw;
        }
    },
                 CismError);
}