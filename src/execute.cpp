#include <iostream>
#include <string>
#include <vector>
#include <cism.h>
#include <cism_error.h>
#include <constants.h>
#include <system_call.h>

#define CISM_RETURN_SUCCESS 0
#define CISM_RETURN_ERROR 1

int main(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);
    try
    {
        execute::Call *call = new execute::SystemCall();
        console::Cism cism(call);
        cism.validate(args);
        cism.run();
    }
    catch (const CismError &e)
    {
        std::cerr << e.what() << std::endl;
        return CISM_RETURN_ERROR;
    }
    return CISM_RETURN_SUCCESS; 
}