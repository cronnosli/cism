#ifndef CISM_H
#define CISM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <call.h>
#include <constants.h>
#include <nlohmann/json.hpp>

namespace console
{
    class Cism
    {
    public:
        Cism(std::unique_ptr<execute::Call> &&);
        ~Cism() = default;
        void validate(std::vector<std::string> &);
        void run();
    private:
        void openConfigFile();
        void parseConfigFile(std::ifstream &);
        void findScript();
        void findPath();
        void buildCommand();
        bool isExecutableInPath(const std::string&, std::vector<std::string>&); 
        std::string m_scriptName;
        std::string m_scriptParam;
        std::string m_configFilePath;
        std::string m_pathStr;
        std::string m_scriptStr;
        std::string m_command;

        nlohmann::json m_configJson;
        nlohmann::json m_scripts;

        std::unique_ptr<execute::Call> m_call;
    };
}

#endif