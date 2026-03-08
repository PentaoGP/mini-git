#include "Repo.h"
#include <string>
#include <fstream>
#include <filesystem>
#include "mgiterror.h"

void Repo::init(){
    auto mgitdir = workdir_ / ".mgit";
        if (std::filesystem::exists(mgitdir)) {
            throw RepoAlreadyExists("Repository already exists: " + mgitdir.string());
        }
    std::filesystem::create_directory(workdir_ / ".mgit");
    std::filesystem::create_directory(workdir_ / ".mgit" / "refs");
    std::filesystem::create_directory(workdir_ / ".mgit" / "objects");
    auto mgitrefsmain = workdir_ /".mgit"/"refs"/"main";
    std::ofstream file(workdir_/".mgit"/"refs"/"main");
        if (!file){
            throw IOError("failed to create/open file: " + mgitrefsmain.string());
        }
    auto mgithead = workdir_ /".mgit"/"HEAD";
    std::ofstream file2(workdir_/".mgit"/ "HEAD");
        if (!file2){
            throw IOError("failed to create/open file: " + mgithead.string());
        }
    auto mgitindex = workdir_ /".mgit"/"index";
    std::ofstream file3(workdir_/".mgit"/"index");
        if (!file3){
            throw IOError("failed to create/open file: " + mgitindex.string());
        }
}