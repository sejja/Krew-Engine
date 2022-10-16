//
//  CodeGen.cpp
//  Krew Header Tool
//
//  Created by Diego Revilla on 1/4/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#include <vector>
#include <string>

struct Obj {
    std::string Name;
    std::vector<std::pair<std::string, std::string>> Properties;
    std::string header;
    std::vector<std::string> Namespaces;
};

void GenerateCode(std::vector<Obj>& Objects);