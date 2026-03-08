#ifndef REPO_H
#define REPO_H
#include <string>
#include <fstream>
#include <filesystem>


class Repo{
public:
explicit Repo(std::filesystem::path const& workdir): workdir_(workdir){}
void init();
private:
std::filesystem::path const& workdir_;
};

#endif