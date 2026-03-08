#ifndef MGITERROR_H
#define MGITERROR_H

#include <stdexcept>
#include <string>
#include "Repo.h"

class MgitError : public std::runtime_error{
    public:
        explicit MgitError(const std::string& msg) : std::runtime_error(msg){}
};

class RepoAlreadyExists : public MgitError{
    public:
        explicit RepoAlreadyExists(const std::string& msg) : MgitError(msg){}
};
class IOError : public MgitError{
    public:
        explicit IOError(const std::string& msg) : MgitError(msg){}
};


#endif