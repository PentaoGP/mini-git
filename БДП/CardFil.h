#ifndef CARDFIL_H
#define CARDDIL_H

#include <filesystem>
#include <utility>


struct Init{
Init(std::filesystem::path workdir_):workdir(std::move((workdir_))){}
void init();



private:
const std::filesystem::path workdir;
};


#endif