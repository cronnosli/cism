#include <iostream>
#include <string>
#include <vector>
#include <cism.h>
#include <cism_error.h>
#include <constants.h>
#include <system_call.h>

int main(int argc, char** argv)
{
    if (argc < MINIMAL_ARGUMENTS) {
        std::cerr << "Insufficient arguments provided.\n";
        return CISM_RETURN_ERROR;
    }

    std::vector<std::string> args(argv, argv + argc);

    try
    {
        auto call = std::make_unique<execute::SystemCall>();
        console::Cism cism(std::move(call));
        cism.validate(args);
        cism.run();
    }
    catch (const CismError& e)
    {
        std::cerr << "CISM Error: " << e.what() << std::endl;
        return CISM_RETURN_ERROR;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Standard Error: " << e.what() << std::endl;
        return CISM_RETURN_ERROR;
    }
    catch (...)
    {
        std::cerr << "Unknown Error Occurred.\n";
        return CISM_RETURN_ERROR;
    }

    return CISM_RETURN_SUCCESS;
}