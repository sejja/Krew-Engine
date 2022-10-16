//
//  Lexer.cpp
//  Krew Header Tool
//
//  Created by Diego Revilla on 1/4/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#if 1 //Change this to 0 when you are running the Editor/Game and 1 when you want to compile the KHT
#include <Windows.h>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>
#include "CodeGen.h"

#define NAMESPACE_LETTER_LENGTH 9
#define CLASS_LETTER_LENGTH     5
#define VOID_LETTER_LENGTH      4
#define LEFTOVERS               3
#define LEFTOVERS2              3
#define LEFTOVERS3              4

std::vector<Obj> serialized_objs;

// ------------------------------------------------------------------------
/*! Read file
*
*   Reads a file in search for object data
*/ // --------------------------------------------------------------------
void Readfile(std::string path) {

    Obj newobj;
    bool _cfound = false;

    //if the file is a header or a source file
    if(path.back() == 'h' || path.back() == 'p') {

        std::ifstream _file;
        std::string output;
        std::string::size_type i;

         _file.open(path);

        //if the file is open
        if (_file.is_open()) {

            //while we still have lines to read                
            while(!_file.eof()) {

                output.clear();
                std::getline(_file, output);

                //if we have found a namespace
                if (output.find("namespace") != std::string::npos) {

                    i = output.find("using namespace");

                    if(i != std::string::npos)
                        continue;

                    std::remove(output.begin(), output.end(), ' ');
                    std::remove(output.begin(), output.end(), '\t');
                    std::remove(output.begin(), output.end(), '{');
                    std::remove(output.begin(), output.end(), '}');
                    std::remove(output.begin(), output.end(), '\n');
                    output.resize(output.size() - 2);

                    i = output.find("namespace");

                    if (i != std::string::npos) {

                        output.erase(i, NAMESPACE_LETTER_LENGTH);
                    
                    }

                    std::cout << "Searching on namespace: " << output + '\n';
                    newobj.Namespaces.push_back(output);                    

                //if we found a new class to serialize
                } else if (output.find("CLASS()") != std::string::npos) {
                    
                    _cfound = true;
                    std::getline(_file, output);
                    serialized_objs.push_back(newobj);
                    output = output.substr(0, output.find(": public"));
                    std::remove(output.begin(), output.end(), '\t');
                    output.resize(output.size() - 1);
                    
                    if(output.back() == ' ' || output.back() == '\n')
                        output.pop_back();

                    i = output.find("class");


                    //fetch the name of the class
                    if (i != std::string::npos) {

                        output.erase(i, 6);
                    
                    }

                    std::cout << "Found Class: " << output + '\n';
                    serialized_objs.back().Name = output;
                    serialized_objs.back().header = path;

                //if we found a property
                } else if(output.find("PROPERTY()") != std::string::npos && _cfound) {

                    std::getline(_file, output);
                    
                    while(output.front() == '\t' || output.front() == ' ') {

                        output = output.substr(1, output.size() - 1);

                    }

                    while(output.back() == '\t' || output.back() == ' ' || output.back() == ';') {

                        output = output.erase(output.size() - 1);

                    }
                    
                    std::string type = output.substr(0, output.find(' '));
                    output=output.substr(output.find_first_of(" \t")+1);

                    std::cout << "\tWith Property: " << output + '\n';
                    serialized_objs.back().Properties.push_back({output, type});

                //if we found a function
                }
            }
        }   

        _file.close();

    } else {

        std::cout << "invalid file" << '\n';

    }
}

// ------------------------------------------------------------------------
/*! Get All Files Names Within folder
*
*   Gets all the filenames in a certain folder for scan
*/ // --------------------------------------------------------------------
std::vector<std::string> get_all_files_names_within_folder(std::string folder) {

    std::vector<std::string> names;
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile((folder + "/*.*").c_str(), &fd);
    
    //If we reached said folder and there are items inside
    if (hFind != INVALID_HANDLE_VALUE) {
        
        /*Discover files within folder, we still need one iteration, since we are
        doing FindNextFile,and doing an ordinary loop would lead us to skip the
        first file*/
        do {

            // read all files in current folder if we are not acessing
            // somethng unapropiate
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                names.push_back(fd.cFileName);
                Readfile(folder + '\\' + fd.cFileName);
            
            }
        
        //do this while we still have files to read
        } while (::FindNextFile(hFind, &fd));

        ::FindClose(hFind);
        
    }

    return names;

}

// ------------------------------------------------------------------------
/*! Get All Directories Within folder
*
*   Gets all the directories which contains files within a folder
*/ // --------------------------------------------------------------------
void get_all_directories_within_folder(std::string folder) {

    DIR *dir = opendir(folder.c_str());
    dirent *entry = readdir(dir);
    struct stat s;
    char foldername[MAX_PATH];

    //while we are suspected to have more directories within a floder
    while (entry != NULL) {

        strncpy(foldername, entry->d_name, MAX_PATH);
        stat(foldername, &s);

        /*if the found item is a folder which does not start by "."(is not a return
            alias or a hidden item)*/
        if ((s.st_mode & S_IFDIR) && foldername[0] != '.') {

            std::string newpath(folder + '\\' + entry->d_name);
            std::cout << newpath + '\n';
            auto files = get_all_files_names_within_folder(newpath);
            get_all_directories_within_folder(newpath);

            /*for debugging and loggin pourposes, print all the files within the 
                directory*/
            for(unsigned i = 0; i < files.size(); i++) {

                std::cout << files[i] + '\n';

            }
        }

        entry = readdir(dir);
    }

    closedir(dir);
    std::cout << "-------------------------------\n";

}

// ------------------------------------------------------------------------
/*! Main
*
*   Krew Header Tool's Entrypoint
*/ // --------------------------------------------------------------------
int main() {

    std::cout << "====================================================================================================\n";
    std::cout << "-----------------------------------------: Krew Header Tool :---------------------------------------\n";
    std::cout << "-----------------------------------------:                  :---------------------------------------\n";
    std::cout << "-----------------------------------------:     SKG 2021     :---------------------------------------\n";
    std::cout << "====================================================================================================\n";
    std::cout << '\n';
    std::cout << "====================================================================================================\n";
    std::cout << "-----------------------------------------:     Compiling      :-------------------------------------\n";
    std::cout << "====================================================================================================\n";
    wchar_t this_process_path[MAX_PATH];
    GetModuleFileNameW(NULL, this_process_path, sizeof(this_process_path));
    char _tempcharplac[MAX_PATH];
    const char DefChar = ' ';
    WideCharToMultiByte(CP_ACP, 0, this_process_path, -1, _tempcharplac, 260, &DefChar, NULL);
    std::string pathstring(_tempcharplac);

    pathstring.resize(pathstring.size() - 35);
    std::cout << pathstring + "\\Source"<< std::endl;
    auto files = get_all_files_names_within_folder(pathstring + "\\Source");
    get_all_directories_within_folder(pathstring + "\\Source");

    /*after we have obtained all the files within the subfolders, find the ones in 
        main folder*/
    for(unsigned i = 0; i < files.size(); i++)
        std::cout << files[i] + '\n';

    std::cout << "====================================================================================================\n";
    std::cout << "-----------------------------------------: Generating Code... :-------------------------------------\n";
    std::cout << "====================================================================================================\n";
    GenerateCode(serialized_objs);
    std::cout << "====================================================================================================\n";
    std::cout << "---------------------------------------: Compilation Succesful :------------------------------------\n";
    std::cout << "-----------------------------------------:                  :---------------------------------------\n";
    std::cout << "------------------------------------------:     code 0     :----------------------------------------\n";
    std::cout << "====================================================================================================\n";

	return 0;

}

#endif
