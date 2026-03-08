#ifndef BST_H
#define BST_H

#include <string>
#include <memory>
#include <filesystem>

struct Node{
    std::string ID;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    explicit Node(const std::string& ID_) : ID(ID_), left(nullptr), right(nullptr){};
};

class BST{
std::unique_ptr<Node> root;
bool InsertImpl(std::unique_ptr<Node>& node, const std::string& id);
bool InstSearch(std::unique_ptr<Node>& node, const std::string& id);
bool InstDel(std::unique_ptr<Node>& node, const std::string& id);
std::filesystem::path workdir;
public:
    BST(){};
    BST(std::filesystem::path workdir___):workdir(std::move((workdir___))){}
    bool Insert(const std::string& ID);
    bool InsertSearch(const std::string& ID);
    bool InsDel(const std::string& ID);
    bool Search(const std::string& ID);
    bool Delete(const std::string& ID);
};



#endif