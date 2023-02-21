#include <cism.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <constants.h>
#include <cism_error.h>

#define MINIMAL_ARGUMENTS 2
#define SCRIPT_NAME_INDEX 1
#define PARAMETER_INDEX 2
#define SCRIPTS "scripts"
#define SCRIPT "script"
#define PATH "path"
#define PATH_DELIMITER '/'
#define PARAMETER_SEPARATOR " "
#define PATH_ENV_DELIMITER ':'
#define PATH_ENV "PATH"

console::Cism::Cism(std::unique_ptr<execute::Call> &call) : m_call(std::move(call)), m_configJson(), m_scripts()
{
}

void console::Cism::validate(std::vector<std::string> &args)
{
    if (args.size() < MINIMAL_ARGUMENTS)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_NO_SCRIPT, ""));
    }
    m_scriptName = args[SCRIPT_NAME_INDEX];
    if (getenv(CISM_CONFIG_FILE_PATH_ENV) != NULL)
    {
        m_configFilePath = getenv(CISM_CONFIG_FILE_PATH_ENV);
    }
    else
    {
        m_configFilePath = CISM_CONFIG_FILE_PATH;
    }

    for (int i = PARAMETER_INDEX; i < args.size(); i++)
    {
        m_scriptParam += PARAMETER_SEPARATOR + args[i];
    }
}

void console::Cism::openConfigFile()
{
    std::ifstream configFile(m_configFilePath);
    if (!configFile.is_open())
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_FAILED_TO_OPEN, m_configFilePath));
    }
}

void console::Cism::parseConfigFile(std::ifstream &configFile)
{
    std::string configContents((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
    try
    {
        m_configJson = nlohmann::json::parse(configContents);
    }
    catch (const nlohmann::json::exception &e)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_FAILED_TO_PARSE, m_configFilePath));
    }
}

void console::Cism::findScript()
{
    try
    {
        m_scripts = m_configJson[SCRIPTS];
        if (m_scripts == nullptr)
        {
            throw CismError(CISM_GET_ERROR(CISM_ERROR_MISSING_SCRIPTS, m_configFilePath));
        }
    }
    catch (nlohmann::json::exception &e)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_MISSING_SCRIPTS, m_configFilePath));
    }
    try
    {
        m_scriptStr = m_configJson.at(SCRIPTS).at(m_scriptName).get<std::string>();
    }
    catch (nlohmann::json::exception &e)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_SCRIPT_NOT_FOUND, m_scriptName));
    }
}

void console::Cism::findPath()
{
    try
    {
        m_pathStr = m_configJson.at(PATH).get<std::string>();        
    }
    catch (const nlohmann::json::exception &e)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_MISSING_PATH, m_configFilePath));
    }
}

void console::Cism::buildCommand()
{
    if (m_scriptStr[0] == PATH_DELIMITER)
    {
        m_command = m_scriptStr + m_scriptParam;
        return;
    }

    char *path = getenv(PATH_ENV);
    if (!path)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_MISSING_PATH_ENVIRONMENT, m_configFilePath));
    }

    bool isSystemCommand = false;
    std::vector<std::string> directories;
    char *p = path;
    char *next = strchr(p, PATH_ENV_DELIMITER);
    while (next)
    {
        directories.push_back(std::string(p, next - p));
        p = next + 1;
        next = strchr(p, PATH_ENV_DELIMITER);
    }
    directories.push_back(p);

    std::string program = m_scriptStr.substr(0, m_scriptStr.find(' '));
    std::string param = m_scriptStr.substr(m_scriptStr.find(' ') + 1);

    for (const std::string &dir : directories)
    {
        std::string filename = dir + PATH_DELIMITER + program;
        if (access(filename.c_str(), X_OK) == 0)
        {
            m_scriptStr = filename + PARAMETER_SEPARATOR + param;
            isSystemCommand = true;
        }
    }

    m_command = (isSystemCommand ? m_scriptStr : m_pathStr + PATH_DELIMITER + m_scriptStr) + m_scriptParam;
}

void console::Cism::run()
{
    openConfigFile();
    std::ifstream configFile(m_configFilePath);
    parseConfigFile(configFile);
    findScript();
    findPath();
    buildCommand();
    m_call->run(m_command);
}
