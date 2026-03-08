#ifndef CARDINDEX_H
#define CARDINDEX_H

#include <string>
#include <vector>
#include <filesystem>

class CardIndex{
public:
CardIndex(){};
CardIndex(const std::string& T_ID , const std::string& T_fullname, const std::string& T_numberphone, const std::string& T_email) : ID(T_ID), fullname(T_fullname), numberphone(T_numberphone), email(T_email){};
std::string ID;
std::string fullname;
std::string numberphone;
std::string email;

private:

};


struct CardCont{
    CardCont(std::filesystem::path workdir__):workdir(std::move((workdir__))){};
    CardCont(){};
    std::vector<CardIndex> CardConteiner;
    void CardAdd();
    void DirAdd();
    bool status(const std::string& ID);
    std::string IDC();

private:
std::filesystem::path workdir;
};

#endif

