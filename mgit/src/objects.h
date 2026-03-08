#ifndef OBJECTS_H
#define OBJECTS_H
#include <string>
#include <vector>
#include <filesystem>
#include <cstddef>
#include <cstdint>

enum class ObjectType {blob, tree, commit};

struct Commit {
    Commit() = default;
    explicit Commit(std::string& toplast) : message(toplast){}
std::string parent;
std::string tree;
uint64_t time;
std::string message;
std::vector<std::byte> serialize() const;
static Commit deserialize(const std::vector<std::byte>& payload);
};

struct TreeEntry {
std::filesystem::path Path;
std::string Blobhash;
TreeEntry(std::filesystem::path p, std::string h): Path(std::move(p)), Blobhash(std::move(h)) {}
};
extern std::vector<TreeEntry> EntryTree;

class Tree{
    public:
std::vector<TreeEntry> entries;
std::vector<std::byte> serialize() const;
static Tree deserialize(const std::vector<std::byte>& payload);
};

class Objects{
public:
    std::string write (ObjectType type, const std::vector<std::byte>& payload);
    explicit Objects(std::filesystem::path const& workdir): workdir_(workdir){}
    std::pair<ObjectType, std::vector<std::byte>> read (const std::string& hash);
    std::string BlobHash (const std::filesystem::path& path_);
    
private:
std::filesystem::path const& workdir_;



};
#endif