#include <cism.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <constants.h>
#include <cism_error.h>

console::Cism::Cism(std::unique_ptr<execute::Call>&& call) : m_call(std::move(call)), m_configJson(), m_scripts()
{
}

void console::Cism::validate(std::vector<std::string>& args)
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
    m_scriptParam = std::accumulate(
        args.begin() + PARAMETER_INDEX, args.end(), std::string(),
        [](const std::string& previous, const std::string& next) {
            return previous + (previous.empty() ? "" : PARAMETER_SEPARATOR) + next;
        }
    );
}

void console::Cism::openConfigFile()
{
    std::ifstream configFile(m_configFilePath);
    if (!configFile.is_open())
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_FAILED_TO_OPEN, m_configFilePath));
    }
}

void console::Cism::parseConfigFile(std::ifstream& configFile)
{
    std::string configContents((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
    try
    {
        m_configJson = nlohmann::json::parse(configContents);
    }
    catch (const nlohmann::json::exception& e)
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
    catch (nlohmann::json::exception& e)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_MISSING_SCRIPTS, m_configFilePath));
    }
    try
    {
        m_scriptStr = m_configJson.at(SCRIPTS).at(m_scriptName).get<std::string>();
    }
    catch (nlohmann::json::exception& e)
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
    catch (const nlohmann::json::exception& e)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_MISSING_PATH, m_configFilePath));
    }
}

bool console::Cism::isExecutableInPath(const std::string& program, std::vector<std::string>& directories)
{
    return std::any_of(directories.begin(), directories.end(), [&program](const std::string& dir) {
        std::string filename = dir + PATH_DELIMITER + program;
        return access(filename.c_str(), X_OK) == 0;
    });
}

void console::Cism::buildCommand()
{
    if (m_scriptStr[0] == PATH_DELIMITER)
    {
        m_command = m_scriptStr + m_scriptParam;
        return;
    }

    char* path = getenv(PATH_ENV);
    if (!path)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_MISSING_PATH_ENVIRONMENT, m_configFilePath));
    }

    std::string pathStr(path);
    std::istringstream iss(pathStr);
    std::vector<std::string> directories;
    std::string dir;
    std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        std::back_inserter(directories));
    std::string_view sv = m_scriptStr;
    size_t spacePos = sv.find(' ');
    std::string_view program = sv.substr(0, spacePos);
    std::string_view param = (spacePos == std::string::npos) ? "" : sv.substr(spacePos + 1);

    bool isSystemCommand = isExecutableInPath(program.data(), directories);
    if (isSystemCommand)
    {
        m_scriptStr = directories.front() + PATH_DELIMITER + program.data() + PARAMETER_SEPARATOR + param.data();
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
