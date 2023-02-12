#include <string>

#define CISM_CONFIG_FILE ".cism.json"
#define CISM_CONFIG_FILE_PATH "./" CISM_CONFIG_FILE
#define CISM_CONFIG_FILE_PATH_ENV "CISM_CONFIG_FILE_PATH"
#define CISM_ERROR "\033[31m[Error]\033[0m"
#define CISM_RUNNING "\033[34m[RUNNING]\033[0m"
#define CISM_ERROR_NO_SCRIPT "No script name provided"
#define CISM_ERROR_FAILED_TO_OPEN "Failed to open"
#define CISM_ERROR_FAILED_TO_PARSE "Failed to parse"
#define CISM_ERROR_MISSING_SCRIPTS "Missing scripts"
#define CISM_ERROR_SCRIPT_NOT_FOUND "Script not found"
#define CISM_ERROR_MISSING_PATH "Missing path"
#define CISM_ERROR_FAILED_RUN "Failed to run"
#define CISM_ERROR_MISSING_PATH_ENVIRONMENT "Missing PATH environment"
#define CISM_ERROR_NO_COMMAND "No command provided"
#define CISM_ERROR_COULD_NOT_OPEN_SCRIPT "Could not open script"

#define CISM_GET_ERROR(error, generic) std::string(CISM_ERROR) + " " + std::string(error) + (std::string(generic).empty() ? "" : ": " + std::string(generic))
#define CISM_GET_RUNNING(command) std::string(CISM_RUNNING) + " " + std::string(command)