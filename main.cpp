#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

namespace fs = std::filesystem;

struct buildobj {
    std::string name;
    unsigned int number = 0;
    std::string outname;
};

int main(int argc, char* argv[]) {



    fs::path p = "";
    std::string outname = "program.exe";
    std::string commandmae = "ccache clang++ -c ";
    std::string commandusiro = "--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths";
    std::string linkcommandmae = "clang++ ";
    std::string linkcommandusiro = "--target=x86_64-w64-mingw32 -fuse-ld=lld -std=c++26 -fdiagnostics-absolute-paths ";
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
    int er = 0;
    for (int i = 0; i < objs.size(); i++) {
        buildobj bb = objs[i];
        std::string temsn = (tems/bb.outname).string();
        std::string command = commandmae + bb.name + " -o " + temsn + ".o " + commandusiro;
        if (isecho) {
            std::cout << command << std::endl;
        }
        er = std::system(command.c_str());
        if (er != 0) return 1;
        awasete += (temsn + ".o ");
    }
    std::string command = linkcommandmae;
    command += awasete;
    command += linkcommandusiro;
    command += "-o ";
    command += (p/outname).string();
    if (isecho) {
        std::cout << command << std::endl;
    }
    er = std::system(command.c_str());
    if (er != 0) return 1;
    return 0;
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