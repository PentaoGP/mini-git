#ifndef INDEX_H
#define INDEX_H
#include <vector>
#include <fstream>
#include <string>

class Index{
public:
    explicit Index (const std::filesystem::path& workdir): workdir_(workdir){} 
    std::vector<std::string> entries;
    void load();
    void add(const std::string& relPath);
    void save();
private:
std::filesystem::path const& workdir_;



};
#endif