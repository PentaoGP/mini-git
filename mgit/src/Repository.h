#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <cstdint>
#include <filesystem>
#include <set>
#include "objects.h"

class Commit;

class Repository{
public:
std::string parent;
std::string tree;
uint64_t time;

explicit Repository(const std::filesystem::path& workdir);
explicit Repository(std::string mas);

std::string ComPa();
uint64_t GetCurrentTime();
void IndexLoader(const std::filesystem::path& in);
std::string CommitHS();
void Com(Commit commit);
void checkout(const std::string& hash);
void status();
std::set<std::string> indexSet();
void log();
std::set<std::string> workSet();
private:
const std::filesystem::path workdir_;
std::string massage;
Objects objects_;
};



#endif