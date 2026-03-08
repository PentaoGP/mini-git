#include <string>
#include <filesystem>
#include <iostream>
#include "CardFil.h"

void Init::init(){
auto workdir_ = workdir/"data";
    if(std::filesystem::exists(workdir_) && std::filesystem::is_directory(workdir_)){std::cout<<"Директория уже существует\n"; return;}
    if(std::filesystem::create_directories(workdir_)){std::cout <<"Директория была успешно создана\n";} else{
        std::cout<<"He удалось создать директорию\n";
    }
}