#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include "Repo.h"
#include "Index.h"
#include "mgiterror.h"
#include "objects.h"
#include "Repository.h"

using namespace std;
int main(int argc, char* argv[]){
filesystem::path workdir = filesystem::current_path();
Objects objects(workdir);
Index index(workdir);
Repo repo(workdir);
Repository repository(workdir);
while (true){
try {
string command, cmd, topic, toplnext, toplast;
cout << ">";

if(!getline(cin, command)) { break;}
if(command.empty()){continue;;}
stringstream ci(command);
ci >> cmd >> topic >> toplnext;
getline(ci, toplast);
if (!toplast.empty() && toplast[0] == ' ') toplast.erase(0, toplast.find_first_not_of(' '));

if (cmd =="mgit" && topic == "init"){
    repo.init();
    continue;
}
if (cmd == "exit"){
    break;
}
if (cmd == "mgit" && topic == "add"){
    repository.IndexLoader(toplnext);
    continue;
}
if(cmd == "mgit" && topic =="commit" && toplnext == "-m"){
    if(toplast.empty()){cout << "message is empty"; continue;}
    Commit commit(toplast);
    commit.parent = repository.ComPa();
    commit.time = repository.GetCurrentTime();
    commit.tree = repository.CommitHS();
        if(commit.tree.empty()){cout << "nothing to commit"; continue;}
    
    repository.Com(commit);

    std::ofstream file(workdir/".mgit"/"index", std::ios::trunc);
    file.close();
    continue;
}

if(cmd == "mgit" && topic == "log"){
    repository.log();
    continue;
}

if(cmd == "mgit" && topic == "checkout"){
    if (toplnext.empty()){cout << "hash is empty"; continue;}
    repository.checkout(toplnext);
    continue;
}

if (cmd == "mgit" && topic == "status") {
    repository.status();
    continue;
}

std::cout << "command is not found" << std::endl;
continue;


}
catch(const RepoAlreadyExists& g) {cerr <<g.what() <<" - Oops, looks like there was an error\n";}
catch(const IOError& i){ cerr <<i.what()<<" - Oops, looks like there was an error\n";}
catch(const exception& e){ cerr << e.what() <<" - Oops, looks like there was an error\n";}
}
}
