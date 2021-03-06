#pragma once

#include <map>
#include <vector>
#include <ostream>

#include <Nodes/INode.h>

enum ErrorType {
    UNDEFINED_IDENTIFIER,
    NON_UNIQUE_IDENTIFIER,
    UNKNOWN_TYPE,
    PRIVATE_METHOD_CALL,
    NON_ARRAY,
    REDEFINITION_CLASS,
    REDEFINITION_METHOD,
    REDEFINITION_PARAM,
    RECURSIVE_INHERITANCE,
    WRONG_TYPE,
    WRONG_ARGUMENT_NUMBER
};

std::ostream& operator<< (std::ostream& out, const ErrorType error);

struct ErrorDescription {
    Location loc;
    ErrorType type;

    //additional info: variable name, exp type, etc.
    std::string info;
};


