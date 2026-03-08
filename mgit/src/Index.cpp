#include "Repo.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "Index.h"
#include "mgiterror.h"
#include <iterator>

/*load читает с mgit/index абсолютные пути и загружает их в контейнер entries.*/
void Index::load(){
    entries.clear();
    std::ifstream file(workdir_/".mgit"/"index");
    auto mgitindex = workdir_/".mgit"/"index";
    if (!file){throw IOError("failed to open file: " + mgitindex.string());}
    std::string en;
    while (std::getline(file, en)){
        if (en.empty()) {continue;}
        entries.push_back(en);
    }
}
/*save читает с entries абслютные пути и записывает их в mgit/index*/
void Index::save(){
    auto mgitindex = workdir_/".mgit"/"index";
    std::ofstream file(mgitindex);
    if (!file){throw IOError("failed to open file: " + mgitindex.string());}
    for (const auto& g:entries){
        if (g.empty()) {continue;}
        file << g <<"\n";
    }
}
/*Принимает путь в виде строки, проверяет есть ли такой в staded или же есть ли он в index. Если нет, то записывает и добавляет в entries*/
void Index::add(const std::string& relPath){
    auto mgitpath = workdir_/relPath;
    std::ifstream file(mgitpath);
    if (!file){throw IOError("failed to open file: " + mgitpath.string());}
    auto mgitindex = workdir_/".mgit"/"index";
    std::ofstream in(mgitindex, std::ios::app);
    bool notStagedYet = true;
    for (const auto& g: entries){
        if (g == relPath){ notStagedYet = false; break;}}
    if (notStagedYet){
        in << relPath << "\n";
        entries.push_back(relPath);
    } else {std::cout <<relPath<< " is already staged\n";}
}  
    
    
    
