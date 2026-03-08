#include "CardIndex.h"
#include <string>
#include <iostream>
#include <vector>
#include <cctype>
#include <fstream>
#include <filesystem>
#include "BST.h"

bool IsValidID(const std::string& ID){
    for (char g: ID){
        if (!std::isdigit(static_cast<unsigned char>(g)))
        return false;
    }
    return true;
}

bool IsValidN(const std::string& fullname){
    for(char g:fullname){
        if(!std::isalpha(g)){return false;}
    }
return true;
}

bool isValidEmail(const std::string&email){
    for(char g:email){
        if(g == '@'){return true;}
    }
    return false;
}


BST bst;

std::string CardCont::IDC(){
    std::string T_ID;
    while (true){
        std::getline(std::cin, T_ID);
        if(T_ID.empty()){std::cout <<"ID пустой\n";continue;}
        if(T_ID.size() != 8){std::cout<<"Был введен некоректный ID\n"; continue;}
        if(!IsValidID(T_ID)){std::cout <<"Ввод коректен только при введение цифр\n";continue;}
        return T_ID;
    }
}


void CardCont::CardAdd(){
std::cout << "Введите ID пользователя.\n";
std::string T_ID;
    while (true){
        std::getline(std::cin, T_ID);
        if(T_ID.empty()){std::cout <<"ID пустой\n";continue;}
        if(T_ID.size() != 8){std::cout<<"Был введен некоректный ID\n"; continue;}
        if(!IsValidID(T_ID)){std::cout <<"Ввод коректен только при введение цифр\n";continue;}
        break;
    }

bst.Insert(T_ID);

std::cout << "Введите ФИО пользователя.\n";
std::string T_fullname;
    while(true){
        std::getline(std::cin, T_fullname);
        if(T_fullname.empty()){std::cout <<"ФИО пустое\n";continue;}
        if(!IsValidN(T_fullname)){std::cout<<"ФИО содержит недопустимые знаки\n";continue;}
        break;
    }

std::cout <<"Введите номер телефона пользователя.\n";
std::string T_numberphone;
    while(true){
        std::getline(std::cin, T_numberphone);
        if(T_numberphone.empty()){std::cout <<"Номер телефона пустой\n";continue;}
        if(T_numberphone.size() != 12){std::cout<<"Был введен некоректный номер телефона\n"; continue;}
        if(!IsValidID(T_numberphone)){std::cout <<"Ввод коректен только при введение цифр\n";continue;}
        break;
    }

std::cout<<"Введите email пользователя.\n";
std::string T_email;
    while(true){
        std::getline(std::cin, T_email);
        if(T_email.empty()){std::cout<<"email пустой\n";continue;}
        if(!isValidEmail(T_email)){std::cout<<"Некоректный email\n";continue;}
        break;
    }

    CardConteiner.emplace_back(T_ID, T_fullname, T_numberphone,T_email);

}

void CardCont::DirAdd(){
std::string d_ID;
    if(CardConteiner.empty()){std::cout<< "Карточек нет"; return;}
    d_ID = CardConteiner.back().ID;
    auto path = d_ID + ".txt";

    std::ofstream file(workdir/"data"/path);
    if(!file){std::cout<<"Ошибка открытия "; return;}
    file << CardConteiner.back().fullname << "\n" << CardConteiner.back().numberphone << "\n" << CardConteiner.back().email <<"\n";
}

bool CardCont::status(const std::string& id){
    auto path = id + ".txt";
    std::ifstream file(workdir/"data"/path);
    if(!file){std::cout << "Такой карточки нет\n"; return false;}

    std::string T_ID, T_fullname, T_numberphoe, T_email;
    std::getline(file, T_fullname);
    std::getline(file, T_numberphoe);
    std::getline(file, T_email);
    T_ID = id;
    std::cout << "Данные по вашему id = " << T_ID << " || " << T_fullname << " || " << T_numberphoe << " || " << T_email << std::endl;
}

