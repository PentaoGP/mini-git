#include "Repository.h"
#include <string>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include "objects.h"
#include <set>
#include <map>

Repository::Repository(const std::filesystem::path& workdir)
    : workdir_(workdir),
      objects_(workdir_)
{}

/*Проверяет main.Если файл нет или пустой, то parent none, первый комит*/
std::string Repository::ComPa(){
    std::fstream file(workdir_/".mgit"/"refs"/"main");
    std::string parentt;
    if(!file.is_open()) {parentt = "none";return parentt;}
    std::getline(file, parentt);
            if(parent.empty()){
            parentt = "none"; }
        return parentt;
}     

uint64_t Repository::GetCurrentTime() {
    return static_cast<uint64_t>(std::time(nullptr));
}



void Repository::IndexLoader(const std::filesystem::path& in) {
    std::cout << "ADD called with: " << in << "\n";

    if (!std::filesystem::exists(in)) {
        std::cout << "Path not found\n";
        return;
    }
    if (!std::filesystem::is_regular_file(in)) {
        std::cout << "Not a file\n";
        return;
    }

    auto indexPath = workdir_ / ".mgit" / "index";
    std::cout << "Index path: " << indexPath << "\n";

    std::filesystem::create_directories(indexPath.parent_path());

    // будем хранить относительные пути
    std::string rel = std::filesystem::relative(in, workdir_).generic_string();

    // проверка на дубликат
    {
        std::ifstream file(indexPath);
        std::string line;
        while (std::getline(file, line)) {
            if (line == rel) {
                std::cout << "Already in index\n";
                return;
            }
        }
    }

    std::ofstream out(indexPath, std::ios::app);
    out << rel << "\n";
    std::cout << "Added to index: " << rel << "\n";
}

std::string Repository::CommitHS(){
    auto co = workdir_/".mgit"/"index";
    std::ifstream file(co);
        if(!file.is_open()){ std::cout << "index is not found"; return "";}
    EntryTree.clear();
        
    Tree tree;

    std::string en;
    while(std::getline(file, en)){
        if(en.empty()){continue;}
        auto hg = workdir_/en;
        auto gh = objects_.BlobHash(hg);

        EntryTree.push_back({std::filesystem::path(en), gh});
    }
    
    tree.entries = EntryTree;
    std::vector<std::byte> payload;
    payload = tree.serialize();
    std::string treehash = objects_.write(ObjectType::tree, payload);
    
    return treehash;
}

void Repository::Com(Commit commit){
    std::vector<std::byte> payload = commit.serialize();
    auto CommitHash = objects_.write(ObjectType::commit, payload);

    std::ofstream file(workdir_/".mgit"/"refs"/"main");
    if(!file.is_open()){std::cout << "File is not open";throw std::runtime_error("");}

    
    
    file << CommitHash <<"\n";
}

void Repository::log(){
std::filesystem::path pa = workdir_/".mgit"/"refs"/"main";
std::fstream file(pa);
if(!file.is_open()){std::cout << "main is none"; return;}
std::string parentt;
std::getline(file, parentt);

std::string curHash = parentt;
if(curHash == ""){std::cout <<"path error"; return;}

    while(curHash != "none"){
    


if(curHash.size() <= 64){std::cout<<"Битая строка"; return;}
std::string pat = curHash.substr(0, 2);
std::string par = curHash.substr(2);
std::fstream joi(workdir_/".mgit"/"objects"/pat/par, std::ios::binary);
if(!joi.is_open()){std::cout << "joi is not open"; return;}


std::string type, sizee;
std::getline(joi, type);
    if(!(type == "commit")){std::cout<<"type is not commit"; return;}
std::getline(joi, sizee);
size_t hj = std::stoi(sizee);
std::vector<std::byte> payload(hj);
joi.read(reinterpret_cast<char*>(payload.data()), hj);
    if(joi.gcount() != static_cast<std::streamsize>(hj)) {
    std::cout << "Ошибка чтения: прочитано байт меньше, чем ожидалось\n";
    return;
}

Commit commit;
commit.deserialize(payload);
std::string thisHash;
thisHash = curHash;
auto cr = commit.parent;
auto ct = commit.tree;
auto cy = commit.time;
auto cu = commit.message;
std::cout <<"ThisHash = " <<thisHash <<"\n" <<"parent = "<< cr << "\n" << "tree = "<<ct << "\n" <<"time = "<< cy << "\n" <<"message = "<< cu <<std::endl;
curHash = commit.parent;
    }
}

void Repository::checkout(const std::string& hash){
    Commit commit;

    auto pr = hash.substr(0, 2);
    auto pt = hash.substr(2);
    auto hashpath = workdir_/".mgit"/"objects"/pr/pt;

    std::fstream file(hashpath, std::ios::binary);
    if(!file.is_open()){ std::cout <<"commit is not found\n"; return; }

    std::string type, size;
    std::getline(file, type);
    if(type != "commit"){ std::cout<< "type not commit\n"; return; }

    std::getline(file, size);
    size_t sizet = std::stoull(size);

    std::vector<std::byte> payload(sizet);
    file.read(reinterpret_cast<char*>(payload.data()), static_cast<std::streamsize>(sizet));
    if(file.gcount() != static_cast<std::streamsize>(sizet)) {
        std::cout << "error written, bytes <\n";
        return;
    }

    commit.deserialize(payload);

    // --- Загружаем tree-объект (тоже из objects)
    auto treehash = commit.tree;
    auto gp = treehash.substr(0, 2);
    auto go = treehash.substr(2);

    auto path = workdir_/".mgit"/"objects"/gp/go;
    std::fstream ser(path, std::ios::binary);
    if(!ser.is_open()){ std::cout <<"tree is not found\n"; return; }

    std::string treetype, sizeh;
    std::getline(ser, treetype);
    if(treetype != "tree"){ std::cout << "type not tree\n"; return; }

    std::getline(ser, sizeh);
    size_t sizec = std::stoull(sizeh);

    std::vector<std::byte> payloadd(sizec);
    ser.read(reinterpret_cast<char*>(payloadd.data()), static_cast<std::streamsize>(sizec));
    if(ser.gcount() != static_cast<std::streamsize>(sizec)) {
        std::cout << "error written, bytes <\n";
        return;
    }

    Tree tree;
    tree.deserialize(payloadd);   

    // --- Для каждого entry читаем blob и восстанавливаем файл
    for (const auto& g : tree.entries) {
        const std::string& blobhash = g.Blobhash;

        auto a = blobhash.substr(0, 2);
        auto b = blobhash.substr(2);
        auto c = workdir_/".mgit"/"objects"/a/b;

        std::fstream blo(c, std::ios::binary);
        if(!blo.is_open()){ std::cout <<"blob is not found\n"; return; }

        std::string blobtype, blobsize;
        std::getline(blo, blobtype);
        if(blobtype != "blob"){ std::cout<<"error - type is not blob\n"; return; }

        std::getline(blo, blobsize);
        size_t blsz = std::stoull(blobsize);

        std::vector<std::byte> pay(blsz);
        blo.read(reinterpret_cast<char*>(pay.data()), static_cast<std::streamsize>(blsz));
        if(blo.gcount() != static_cast<std::streamsize>(blsz)) {
            std::cout << "error written, bytes <\n";
            return;
        }

        auto gpath = workdir_/g.Path;
        std::filesystem::create_directories(gpath.parent_path());

        std::ofstream za(gpath, std::ios::binary | std::ios::out | std::ios::trunc);
        if(!za.is_open()){ std::cout <<"cannot open file for writing\n"; return; }

        za.write(reinterpret_cast<const char*>(pay.data()),
                 static_cast<std::streamsize>(pay.size()));
        if (!za) { std::cout << "write error\n"; return; }
    }
}







std::set<std::string> Repository::indexSet(){
        std::ifstream file(workdir_/".mgit"/"index");
        std::string h;
        std::set<std::string> indexMap;
        
            while(std::getline(file, h)){
                if(h.empty()){continue;}
                if (h.back() == '\r') {h.pop_back();}
            indexMap.insert(h);}
        return indexMap;
}


std::set<std::string> Repository::workSet(){
    std::set<std::string> workSet;
    for(std::filesystem::recursive_directory_iterator g(workdir_); g != std::filesystem::recursive_directory_iterator(); ++g){
        if(g->is_directory() && g->path().filename() == ".mgit"){g.disable_recursion_pending();continue;}
        if(g->is_directory() && g->path().filename() == ".vscode"){g.disable_recursion_pending();continue;}
        if(g->is_directory() && g->path().filename() == "src"){g.disable_recursion_pending();continue;}

        if (!g->is_regular_file()) continue;
        std::filesystem::path rel = std::filesystem::relative(g->path(), workdir_); 
        workSet.insert(rel.generic_string());
    }
    return workSet;
}






void Repository::status() {

    
    auto pathCommit = workdir_ / ".mgit" / "refs" / "main";
    std::ifstream refFile(pathCommit);
    if (!refFile.is_open()) { std::cout << "main is not open"; return; }

    std::string pa;
    std::getline(refFile, pa);
    if (!pa.empty() && pa.back() == '\r') pa.pop_back();

    if (pa.empty() || pa == "none") { std::cout << "commit is not found"; return; }

    
    Commit commit;

    auto pr = pa.substr(0, 2);
    auto pt = pa.substr(2);
    auto hashpath = workdir_ / ".mgit" / "objects" / pr / pt;

    std::fstream cfile(hashpath, std::ios::binary);
    if (!cfile.is_open()) { std::cout << "commit is not found\n"; return; }

    std::string type, size;
    std::getline(cfile, type);
    if (!type.empty() && type.back() == '\r') type.pop_back();
    if (type != "commit") { std::cout << "type not commit\n"; return; }

    std::getline(cfile, size);
    if (!size.empty() && size.back() == '\r') size.pop_back();
    size_t sizet = std::stoull(size);

    std::vector<std::byte> payload(sizet);
    cfile.read(reinterpret_cast<char*>(payload.data()), static_cast<std::streamsize>(sizet));
    if (cfile.gcount() != static_cast<std::streamsize>(sizet)) {
        std::cout << "error written, bytes <\n";
        return;
    }

    commit = Commit::deserialize(payload);

    
    auto treehash = commit.tree;
    auto gp = treehash.substr(0, 2);
    auto go = treehash.substr(2);

    auto tpath = workdir_ / ".mgit" / "objects" / gp / go;
    std::fstream tfile(tpath, std::ios::binary);
    if (!tfile.is_open()) { std::cout << "tree is not found\n"; return; }

    std::string treetype, sizeh;
    std::getline(tfile, treetype);
    if (!treetype.empty() && treetype.back() == '\r') treetype.pop_back();
    if (treetype != "tree") { std::cout << "type not tree\n"; return; }

    std::getline(tfile, sizeh);
    if (!sizeh.empty() && sizeh.back() == '\r') sizeh.pop_back();
    size_t sizec = std::stoull(sizeh);

    std::vector<std::byte> payloadd(sizec);
    tfile.read(reinterpret_cast<char*>(payloadd.data()), static_cast<std::streamsize>(sizec));
    if (tfile.gcount() != static_cast<std::streamsize>(sizec)) {
        std::cout << "error written, bytes <\n";
        return;
    }

    Tree tree = Tree::deserialize(payloadd);

    
    auto ids  = indexSet();  
    auto work = workSet();   

    std::map<std::string, std::string> commitMap;
    for (const auto& e : tree.entries) {
        commitMap[e.Path.generic_string()] = e.Blobhash;
    }

    
    std::vector<std::string> untracked;
    for (const auto& g : work) {
        if (ids.find(g) == ids.end() && commitMap.find(g) == commitMap.end()) {
            untracked.push_back(g);
        }
    }

    
    std::vector<std::string> modified;
    for (const auto& p : ids) {
        auto it = commitMap.find(p);
        if (it == commitMap.end()) continue;      
        if (work.find(p) == work.end()) continue; 

        std::string curHash = objects_.BlobHash(workdir_ / p);
        if (curHash != it->second) {
            modified.push_back(p);
        }
    }

    
    std::cout << "staged:\n";
    for (const auto& p : ids) std::cout << "  " << p << "\n";

    std::cout << "modified:\n";
    for (const auto& p : modified) std::cout << "  " << p << "\n";

    std::cout << "untracked:\n";
    for (const auto& p : untracked) std::cout << "  " << p << "\n";
}


