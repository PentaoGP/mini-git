#include<vector>
#include <iostream>
#include <windows.h>
#include "CardFil.h"
#include "CardIndex.h"
#include <filesystem>
#include <sstream>
#include "BST.h"

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Init workdir_(std::filesystem::current_path());
    CardCont workdir__(std::filesystem::current_path());
    BST workdir___(std::filesystem::current_path());
    std::cout <<"Введите комманду\n" << ">";

    std::string command, first;
    std::cout<<"Введите комманду"<<std::endl;
    while(true){

        std::cout << ">";
        if(!std::getline(std::cin, command)){std::cout<<"Ошибка ввода.\n";continue;}
        std::istringstream in(command);
        in >> first;
        if (first.empty()) continue;


        if(first == "init"){
            workdir_.init();
            continue;
        }
        if(first == "exit"){
            break;
        }
        if(first == "add"){
            workdir__.CardAdd();
            workdir__.DirAdd();

            continue;
        }
        if(first == "search"){
            auto id = workdir__.IDC();
            workdir___.Search(id);
            continue;
        }
        if(first == "status"){
            auto id = workdir__.IDC();
            workdir__.status(id);
            continue;
        }
        if(first == "delete"){
            auto id = workdir__.IDC();
            workdir___.Delete(id);
            continue;
        }
    
    
    
    }


}