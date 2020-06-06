#include "MSHtoCGNS/Utilities/Error.hpp"

std::string error(const char* prettyFunction, std::string message, const char* cgError) {
    return std::string(prettyFunction) + " - " +  message + " / " +  "cg_get_error()" + " - " + cgError;
}

std::string error(const char* prettyFunction, std::string message) {
    return std::string(prettyFunction) + " - " +  message;
}
