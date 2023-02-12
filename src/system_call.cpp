#include <system_call.h>
#include <iostream>
#include <constants.h>
#include <cism_error.h>

bool execute::SystemCall::run(std::string &command)
{
    if (command.empty())
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_NO_COMMAND, ""));
    }
    lookupCommand(command);
    std::cout << CISM_GET_RUNNING(command) << std::endl;
    if (system(command.c_str()) != 0)
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_FAILED_RUN, command));
    }
    return true;
}

void execute::SystemCall::lookupCommand(std::string &command)
{
    auto filename = command.substr(0, command.find_first_of(' '));
    if (FILE *file = fopen(filename.c_str(), "r"))
    {
        fclose(file);
    }
    else
    {
        throw CismError(CISM_GET_ERROR(CISM_ERROR_COULD_NOT_OPEN_SCRIPT, command));
    }
}