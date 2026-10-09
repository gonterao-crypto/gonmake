#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <syncstream>

namespace fs = std::filesystem;

struct buildobj {
    std::string name;
    unsigned int number = 0;
    std::string outname;
};

struct workerobj {
    bool isecho;
    std::string commandmae;
    std::string commandusiro;
    fs::path tems;
    std::vector<buildobj>* objs;
    std::atomic<int>* index;
    std::atomic<int>* errork;
};

void compiletask(workerobj *wo, int id) {
    std::osyncstream bout{std::cout};
    if (wo->isecho) bout << "[" << id << "]" << "worker: kido" << std::endl;
    int mindex = 0;
    while (true) {
        mindex = wo->index->fetch_add(1);
        if (mindex >= wo->objs->size()) break;
        if (wo->errork->load() != 0) break;
        buildobj bb = (*(wo->objs))[mindex];
        std::string temsn = (wo->tems/bb.outname).string();
        std::string command = wo->commandmae + bb.name + " -o " + temsn + ".o " + wo->commandusiro;
        if (wo->isecho) {
            bout << "[" << id << "]" << command << std::endl;
        }
        int er = std::system(command.c_str());
        if (er != 0) {
            wo->errork->fetch_add(1);
            break;
        }
        bout.emit();
    }
    if (wo->isecho) bout << "[" << id << "]" << "worker: syuryo" << std::endl;
}


int getnumberfor10s(std::string &b, int min = 0) {
    unsigned int kurai = 1;
    int ret = 0;
    for (int i = b.length() - 1; i >= min; i--) {
        char n = b[i];
        if (n == '1') ret += kurai * 1;
        else if (n == '2') ret += kurai * 2;
        else if (n == '3') ret += kurai * 3;
        else if (n == '4') ret += kurai * 4;
        else if (n == '5') ret += kurai * 5;
        else if (n == '6') ret += kurai * 6;
        else if (n == '7') ret += kurai * 7;
        else if (n == '8') ret += kurai * 8;
        else if (n == '9') ret += kurai * 9;
        else if (n == '0') ret += kurai * 0;
        else {
            std::string errormsg = "suuti hennkann sippai! Kuwasikuha, ";
            errormsg += b;
            errormsg += " in ";
            errormsg += n;
            errormsg += " (";
            errormsg += std::to_string(i);
            errormsg += ")";
            std::cout << errormsg << std::endl;
        }
        kurai *= 10;
    }
    return ret;
}

int main(int argc, char* argv[]) {



    fs::path p = "";
    std::string outname = "program.exe";
    std::string commandmae = "ccache clang++ -c ";
    std::string commandusiro = "--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths -fexperimental-library";
    std::string linkcommandmae = "clang++ ";
    std::string linkcommandusiro = "--target=x86_64-w64-mingw32 -fuse-ld=lld -std=c++26 -fdiagnostics-absolute-paths ";
    int threadcount = 2;
    bool isecho = true;

    if (argc == 1) {
        std::cerr << "Hikisuu tarinai kara tyanto tuika sitekitene" << std::endl;
        return 1;
    }
    std::string in;
    for (int i = 0; i < argc; i++) {
        in = argv[i];
        if (in == "-dir") {
            i++;
            if (i >= argc) {
                std::cerr << "-dir sitei saretakedo sono sakiga naiyo!" << std::endl; return 1;
            }
            in = argv[i];
            p = in;
        } else if (in == "-o") {
            i++;
            if (i >= argc) {
                std::cerr << "-o (outname) sitei saretakedo sono sakiga naiyo!" << std::endl; return 1;
            }
            in = argv[i];
            outname = in;
        } else if (in == "-cm") {
            i++;
            if (i >= argc) {
                std::cerr << "-cm (command mae) sitei saretakedo sono sakiga naiyo!" << std::endl; return 1;
            }
            in = argv[i];
            commandmae = in;
        } else if (in == "-cu") {
            i++;
            if (i >= argc) {
                std::cerr << "-cu (command usiro) sitei saretakedo sono sakiga naiyo!" << std::endl; return 1;
            }
            in = argv[i];
            commandusiro = in;
        } else if (in == "-lcm") {
            i++;
            if (i >= argc) {
                std::cerr << "-lcm (link command mae) sitei saretakedo sono sakiga naiyo!" << std::endl; return 1;
            }
            in = argv[i];
            linkcommandmae = in;
        } else if (in == "-lcu") {
            i++;
            if (i >= argc) {
                std::cerr << "-lcu (link command usiro) sitei saretakedo sono sakiga naiyo!" << std::endl; return 1;
            }
            in = argv[i];
            linkcommandusiro = in;
        } else if (in == "-noecho") {
            isecho = false;
        } else if (in == "-echo") {
            isecho = true;
        } else if (in == "-thread") {
            i++;
            if (i >= argc) {
                std::cerr << "-thread (thread kazu) sitei saretakedo sono sakiga naiyo!" << std::endl; return 1;
            }
            std::string b = argv[i];
            threadcount = getnumberfor10s(b, 0);
        }
    }

    if (p.empty()) {
        std::cerr << "pasuga sitei saretenaiyo! -dir de sitei sitene!";
        return 1;
    }
    
    std::vector<buildobj> objs;
    unsigned int num = 0;
    for (const fs::directory_entry& x : fs::recursive_directory_iterator(p)) {
        fs::path ext = x.path().extension();
        if (ext == ".cpp" || ext == ".c") {
            std::string numbs = "";
            numbs += "a";
            numbs += std::to_string(num);
            buildobj nu = {
                x.path().string(),
                num,
                numbs,
            };
            objs.push_back(nu);
            num += 1;
        }
    }
    fs::path tems = p/".gonmake";
    fs::create_directory(tems);
    std::string awasete = "";
    for (int i = 0; i < objs.size(); i++) {
        buildobj bb = objs[i];
        std::string temsn = (tems/bb.outname).string();
        awasete += (temsn + ".o ");
    }

    /*
    
struct workerobj {
    bool isecho;
    std::string commandmae;
    std::string commandusiro;
    fs::path tems;
    std::vector<buildobj> objs;
    std::atomic<int> index(0);
}
    */
    std::atomic<int> index(0);
    std::atomic<int> errork(0);
    workerobj wo;
    wo.isecho = isecho;
    wo.commandmae = commandmae;
    wo.commandusiro = commandusiro;
    wo.tems = tems;
    wo.objs = &objs;
    wo.index = &index;
    wo.errork = &errork;
    std::vector<std::thread> threads;
    for (int i = 0; i < threadcount; i++) {
        threads.emplace_back(compiletask, &wo, i);
    }
    for (int i = 0; i < threadcount; i++) {
        threads[i].join();
    }
    if (wo.errork->load() == 0) {
        std::string command = linkcommandmae;
        command += awasete;
        command += linkcommandusiro;
        command += "-o ";
        command += (p/outname).string();
        if (isecho) {
            std::cout << command << std::endl;
        }
        int er = std::system(command.c_str());
        if (er != 0) return 1;
        return 0;
    }
}

/*
del /s /q program.exe

set "COMPITI=ccache clang++ -c"
set "COMPNI=--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths"


%COMPITI% main.cpp -o out/main.o %COMPNI%
%COMPITI% src/main.cpp -o out/src/main.o %COMPNI%
%COMPITI% src/cpu/cpumemory.cpp -o out/src/cpu/cpumemory.o %COMPNI%
%COMPITI% src/cpu/cpubump.cpp -o out/src/cpu/cpubump.o %COMPNI%
clang++ ^
out/main.o ^
out/src/main.o ^
out/src/cpu/cpumemory.o ^
out/src/cpu/cpubump.o ^
--target=x86_64-w64-mingw32 -fuse-ld=lld -o program.exe -std=c++26 -fdiagnostics-absolute-paths
if exist "program.exe" (
  program
) else (
  echo Error!
)

*/