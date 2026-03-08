#include "objects.h"
#include <string>
#include <vector>
#include <openssl/sha.h>
#include <cstddef>    
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>

std::vector<TreeEntry> EntryTree; 

/*Хеш*/
std::string sha256(const std::vector<std::byte>& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    SHA256_Update(
        &ctx,
        reinterpret_cast<const unsigned char*>(data.data()),
        data.size()
    );

    SHA256_Final(hash, &ctx);

   
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        oss << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(hash[i]);
    }

    return oss.str(); 
}
/*Конвертер type в строку*/
std::string StringToType(ObjectType type){
    switch (type){
        case ObjectType::blob : return "blob"; 
        case ObjectType::tree : return "tree"; 
        case ObjectType::commit: return "commit";
        default : return "Unknown";
    }
}
/*Конвертер строки в type*/
ObjectType TypeFromString(const std::string& type){
    if (type == "blob") {
        return ObjectType::blob;
    }
    if (type == "tree") {
        return ObjectType::tree;
    }
    if (type == "commit") {
        return ObjectType::commit;
    }
    throw std::runtime_error("Unknown object type: " + type); }


std::string Objects::write (ObjectType type, const std::vector<std::byte>& payload){

    /*Создается шаблон. type \n size(payload) /n*/

    std::string HeaderString = StringToType(type) + "\n" + std::to_string(payload.size()) + "\n";
    std::vector<std::byte> raw;
    const auto* hBegin = reinterpret_cast<const std::byte*>(HeaderString.data());
    const auto* hEnd   = reinterpret_cast<const std::byte*>(HeaderString.data() + HeaderString.size());
    raw.insert(raw.end(), hBegin, hEnd);
    raw.insert(raw.end(), payload.begin(), payload.end());

    /*
        В вектор байт raw записали header и payload.
        type\n
        size(payload)\n
        paload\n
    */

    const std::string hash = sha256(raw);

    /*Создет строку с hash всего raw и проверяет, чтоб размер хеша не был равен 64*/

    if (hash.size() != 64) throw std::runtime_error("invalid hash length");
    std::string hashdeader = hash.substr(0, 2);
    std::string hashheaderEnd = hash.substr(2);
    std::filesystem::path mgitobj = workdir_/".mgit"/"objects"/hashdeader/hashheaderEnd;
    std::filesystem::path mgito = workdir_/".mgit"/"objects"/hashdeader;

    /*Создаем шаблоны для записи в object, первые 2 символа директория, остальные название файла в только что созданной директории*/

    if (std::filesystem::exists(mgitobj)) {
    return hash;}
    std::filesystem::create_directories(mgito);
    std::ofstream file(mgitobj, std::ios::binary);
    if (!file) {
        throw std::runtime_error("cannot open object file for writing");}
    file.write(reinterpret_cast<const char*>(raw.data()),
           static_cast<std::streamsize>(raw.size()));
    if (!file) {
        throw std::runtime_error("failed to write object file");}

    /*Записываем raw в object по шаблону выше и возвращаем hash.*/

    return hash; }





/*На вводе hash для нахождения обьекта в object и получаем и получаем изначальный type и payload*/    
std::pair<ObjectType, std::vector<std::byte>> Objects::read (const std::string& hash){
    if (hash.size() != 64) throw std::runtime_error("invalid hash length");
    std::string hashheader = hash.substr(0, 2);
    std::string hashheaderEnd = hash.substr(2);
    std::filesystem::path mgitobj = workdir_/".mgit"/"objects"/hashheader/hashheaderEnd;
        std::ifstream file(mgitobj, std::ios::binary);
        if(!file) { throw std::runtime_error("cannot open object file for reading"); }
        std::string type, sizez;
        if (!std::getline(file, type) || !std::getline(file, sizez)){
            throw std::runtime_error("Failed to read header"); }
            if (!sizez.empty() && sizez.back() == '\r') sizez.pop_back();
            if (!type.empty()  && type.back()  == '\r') type.pop_back();
        size_t size = std::stoull(sizez);
        std::vector<std::byte> payload(size);
            if (!file.read(reinterpret_cast<char*>(payload.data()),
               static_cast<std::streamsize>(payload.size()))) {
            throw std::runtime_error("Failed to read payload");}
    ObjectType typet = TypeFromString(type);
    return { typet, std::move(payload)};
}


/*Берет путь, чаще из index, но не привязанна к нему, находит по нему файл и записывает целиком в payload, получая blob type и сам payload*/
std::string Objects::BlobHash(const std::filesystem::path& path_){
    std::ifstream file(path_, std::ios::binary);
        if (!file) {
            throw std::runtime_error("cannot open object file for reading");
        }
    std::size_t sizet = std::filesystem::file_size(path_);
    std::vector<std::byte> payload(sizet);

    file.read(reinterpret_cast<char*>(payload.data()), static_cast<std::streamsize>(payload.size()));
        if (file.bad()) {throw std::runtime_error("flag bad");}
        if (!(file.gcount() == sizet)) {throw std::runtime_error("sizet invalid");}
    
    return write(ObjectType::blob, payload); 
}


/*Записывает в tree.entries blobhash и path*/
Tree Tree::deserialize(const std::vector<std::byte>& payload){
    Tree tree;
    size_t sizez = payload.size();
    std::string payloadd (reinterpret_cast<const char*>(payload.data()), payload.size());
    std::string blobhash, path;
    std::istringstream in(payloadd);
        while (std::getline(in, blobhash)){
            if (!std::getline(in, path)){
                throw std::runtime_error("tree payload malformed: missing path line");
            }
            tree.entries.push_back(TreeEntry{std::filesystem::path(path), blobhash});
        }
    return tree;
}   

/* 
Сортирует entries через шаблонный entries и записывает payload  = blobhash + path
blobhash\n
path\n
*/
std::vector<std::byte> Tree::serialize() const{
    auto sorted = entries;
        std::sort(sorted.begin(), sorted.end(), [](const TreeEntry& a, const TreeEntry& b){return a.Path < b.Path;});
        std::vector<std::byte> payload;
        auto append = [&](const std::string& s) {
        const auto* b = reinterpret_cast<const std::byte*>(s.data());
        payload.insert(payload.end(), b, b + s.size()); };

            for (const auto& g: sorted){
                append(g.Blobhash);
                payload.push_back(static_cast<std::byte>('\n'));
                
                const std::string pathstr = g.Path.generic_string();
                append(pathstr);
                payload.push_back(static_cast<std::byte>('\n'));
            }
            return payload;
}

std::vector<std::byte> Commit::serialize() const{
    std::vector<std::byte> payload;
    auto gh = [&payload](const std::string& g){
        const auto *first = reinterpret_cast<const std::byte*>(g.data());
        payload.insert(payload.end(), first, first + g.size());
        payload.push_back(static_cast<std::byte>('\n'));
    };
    auto gl = [&payload](std::uint64_t g) {
    for (int i = 0; i < 8; ++i) {
        payload.push_back(static_cast<std::byte>((g >> (8 * i)) & 0xFF));}
        payload.push_back(static_cast<std::byte>('\n'));
    };
    gh(parent);
    gh(tree);
    gl(time);
    gh(message);
    return payload;
}

Commit Commit::deserialize(const std::vector<std::byte>& payload) {
    std::string payloadd(reinterpret_cast<const char*>(payload.data()), payload.size());
    std::istringstream in(payloadd);

    auto trim_cr = [](std::string& s) {
        if (!s.empty() && s.back() == '\r') s.pop_back();
    };

    std::string parentt, treee, timee, messagee;

    if (!std::getline(in, parentt)) {
        throw std::runtime_error("Некорректный payload: отсутствует строка parent");
    }
    trim_cr(parentt);

    if (!std::getline(in, treee)) {
        throw std::runtime_error("Некорректный payload: отсутствует строка tree");
    }
    trim_cr(treee);

    if (!std::getline(in, timee)) {
        throw std::runtime_error("Некорректный payload: отсутствует строка time");
    }
    trim_cr(timee);

    if (!std::getline(in, messagee)) {
        throw std::runtime_error("Некорректный payload: отсутствует строка message");
    }
    trim_cr(messagee);

    uint64_t timeee = 0;
    try {
        timeee = std::stoull(timee);
    } catch (...) {
        throw std::runtime_error("Некорректный payload: время не является числом");
    }

    Commit commit;
    commit.parent = parentt;
    commit.tree = treee;
    commit.time = timeee;
    commit.message = messagee;
    return commit;
}

