#include "BST.H"
#include <string>
#include <iostream>
#include <memory>
#include "CardIndex.h"
#include <fstream>

Node ID_;
CardCont card;

bool BST::InsertImpl(std::unique_ptr<Node>& node, const std::string& id){
    if(!node){node = std::make_unique<Node>(id); return true;}
    if(id < node->ID) {return InsertImpl(node->left, id);}
    if(id > node->ID) {return InsertImpl(node->right, id);}
    return false;
}



bool BST::Insert(const std::string& id){
    return InsertImpl(root, id);
}

bool BST::InstSearch(std::unique_ptr<Node>& node, const std::string& id){
    if(id == node ->ID){return true;}
    if(id < node->ID) {return InsertImpl(node->left, id);}
    if(id > node->ID) {return InsertImpl(node->right, id);}
    return false;
}

bool BST::InsertSearch(const std::string& ID){
    return InstSearch(root, ID);
}

bool BST::InstDel(std::unique_ptr<Node>& node, const std::string& id){
    if (!node) return false;
    if(id == node ->ID){
        if(node->left == nullptr && node ->right == nullptr){
            node.reset();return true;}
        if(node->left == nullptr && node ->right != nullptr){
            node = std::move(node ->right); return true;
        }
        if(node->left != nullptr && node ->right == nullptr){
            node = std::move(node ->left); return true;
        }
        if (node->left != nullptr && node->right != nullptr) {
        Node* succ = node->right.get();
        while (succ->left) {
            succ = succ->left.get();}
        node->ID = succ->ID;

        return InstDel(node->right, succ->ID);}
    }
    if(id < node->ID) {return InstDel(node->left, id);}
    if(id > node->ID) {return InstDel(node->right, id);}
    return false;
}
bool BST::InsDel(const std::string& ID){
    return InstDel(root, ID);
}


bool BST::Search(const std::string& id){
    if(InsertSearch(id)){std::cout << "Данный пользователь существует, желаете вывести данные? yes/no\n";} else {std::cout << "Данный пользователь не существует.\n";}
    std::string cmd;
    std::getline(std::cin, cmd);
    if(cmd == "yes"){card.status(id);} else {return false;}
}

bool BST::Delete(const std::string& ID){
    if(!InsertSearch(ID)){std::cout<<"Такой карточки нет.\n";return false;}
    if(InsDel(ID)){std::cout<<"Удаление прошло успешно.\n";} else{std::cout <<"При удаление произошла ошибка.\n"; return false;}
}
