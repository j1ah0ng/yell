#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// exec 
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

const char cmd[]        = "figlet -f big";
constexpr int cmdlen    = strlen(cmd);

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string output;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) 
        throw std::runtime_error("popen() failed to create pipe");

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        output += buffer.data();
    }

    return output;
}

int main(int argc, char *argv[]) {

    // no argument exit case
    if (argc == 1) exit(0);
    
    // create the full command
    int chars = 0;
    for (int i = 1; i < argc; ++i) {
        std::string temp = argv[i];
        chars += temp.length();
        if (i + 1 != argc) ++chars;
    }
    char *fullCmd = (char *)calloc(sizeof(char), cmdlen + 1 + chars);
    strcpy(fullCmd, cmd);
    strcat(fullCmd, (" "));
    for (int i = 1; i < argc; ++i) {
        strcat(fullCmd, argv[i]);
        if (i + 1 != argc) strcat(fullCmd, " ");
    }

    // run the command, piping to a string
    std::string output = exec(fullCmd);

    // figure out the width of the output
    int width = 0;
    for (char &c : output) {
        ++width;
        if (c == '\n') break;
    }
    // figure out the height of the output
    int height = 0;
    for (char &c : output) {
        if (c == '\n') ++height;
    }

    // get terminal info
    std::string ttyInfo = exec("stty size");
    std::string ttyHeightStr = ttyInfo.substr(0, ttyInfo.find(" "));
    std::string ttyWidthStr = ttyInfo.substr(ttyInfo.find(" "), ttyInfo.length());
    int ttyHeight, ttyWidth;
    try {
        ttyHeight = std::stoi(ttyHeightStr);
        ttyWidth = std::stoi(ttyWidthStr);
    } catch (std::out_of_range &e) { 
        e.what();
        std::cout << "Terminal size beyond integer size\n";
        return(-1);
    }

    // calculate offsets
    int padLeft = (ttyWidth / 2) - (width / 2);
    int padTop = (ttyHeight / 2) - (height / 2);

    // print
    for (int i = 0; i < padTop - 1; ++i) std::cout << "\n";
    for (int i = 0; i < padLeft; ++i) std::cout << " ";
    for (char &c : output) {
        std::cout << c;
        if (c == '\n') for (int i = 0; i < padLeft; ++i) std::cout << " ";
    }
    for (int i = 0; i < padTop + 2; ++i) std::cout << "\n";

    // we dont like memory leaks
    free(fullCmd);
}
