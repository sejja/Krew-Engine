//
//  CodeGen.cpp
//  Krew Header Tool
//
//  Created by Diego Revilla on 1/4/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#include "CodeGen.h"
#include <iostream>
#include <fstream>
#include <set>

// ------------------------------------------------------------------------
/*! Generate Code
*
*   Given the data gathered by the Lexer, generate code for the Engine
*/ // --------------------------------------------------------------------
void GenerateCode(std::vector<Obj>& Objects) {

    unsigned i;
    std::ofstream _source("Intermediate/SourceDB.gen.cpp");

    if(!_source)
		_source.open("SourceDB.gen.cpp", std::fstream::in | std::fstream::out | std::fstream::trunc);

    std::vector<Obj>::iterator ito;
    std::vector<std::string>::iterator its;

    std::cout << "\tIncluding Dependencies...\n";
    std::set<std::string> includes;

    //include all the dependency files
    for(ito = Objects.begin(); ito != Objects.end(); ito++) {
        includes.insert("#include \"" + (*ito).header + "\"\n");
    }

    for(auto& x : includes) 
        _source << x;

    std::cout << "\tCreating Spaces...\n";
    std::cout << "\tCreating Name Table...\n";
    std::cout << "Generating Source...\n";
    _source << "/*This file has been automatically generated, \n";
    _source << "DO NOT MODIFY UNDER ANY CIRCUMSTANCE, \n";
    _source << "//\n";
    _source << "Good Neighbours Header Tool - 2021*/\n";
    std::cout << "\tIncluding Generated Data...\n";
    _source << "\n#include<Shared.h>\n";
    _source << "template<typename T>  class TCreator : public Engine::Factory::ICreator {Engine::IBase* Create() override {return new T();}};\nint DllMain() {\n";
    _source << "\tstatic bool loaded = false;\n";
    _source << "\tif(!loaded) {\n";

    //Go through every object within the list
    for(i = 0; i < Objects.size(); i++) {

        _source << "\n";

        std::string nametype;

        //write the correct namespaces
        for(its = Objects[i].Namespaces.begin(); its != Objects[i].Namespaces.end(); its++)
            nametype += ((*its) + "::");
        nametype += Objects[i].Name;
        _source << "\tEngine::Factory::Instance().RegisterCustomCreator<" + nametype + ">(\"" + nametype + "\");";

    }
    
    _source << "\tloaded = true;}\n";
    _source << "\n\n\treturn 1;\n\n}\n";

    for(i = 0; i < Objects.size(); i++) {

        _source << "\n";

        std::string nametype;

        //write the correct namespaces
        for(its = Objects[i].Namespaces.begin(); its != Objects[i].Namespaces.end(); its++)
            nametype += ((*its) + "::");
        nametype += Objects[i].Name;

        //TO JSON
        _source << "void " << nametype << "::ToJson(" << nametype << "::json& j) const {\n";

        for(auto& x : Objects[i].Properties) {
            _source << "\tj[\"" << x.first << "\"] = " << x.first << ";\n";
        }

        _source << "}\n";

        //FROM JSON
        _source << "void " << nametype << "::FromJson(const " << nametype << "::json& j) {\n";
        for(auto& x : Objects[i].Properties) {
            _source << "\tauto it_" << x.first << " = j.find(\"" << x.first << "\");\n";
            _source << "\tif(it_" << x.first << " != j.end()) " << x.first << " = (*it_" << x.first << ").get<" << x.second << ">();\n";
        }
        _source << "}\n";

        //OnEditor
        _source << "#ifdef _EDITOR_\n";
        _source << "void " << nametype << "::OnEditor() {\n";
        _source << "\tEditor::UIPropertyDraw _UIDraw;\n";
        for(auto& x : Objects[i].Properties) {
            _source << "\t_UIDraw << std::pair{\"" << x.first << "\", &" << x.first << "};\n";
        }
        _source << "}\n";
        _source << "#endif\n";
    }

    _source.close();

}