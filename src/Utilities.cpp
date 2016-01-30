/*!
    \file   Utilities.cpp
    \author Sean McGeer
    \date   11/29/15

    \brief
        Implements various small utilities that are used by the game or engine.

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Utilities.h"
#include "Serialize.h"
#include "State.h"
#include "Level.h"
#include "Platforms.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

namespace FLE = ForLeaseEngine;

std::vector<ForLeaseEngine::State *> LoadLevels(std::string levelList, std::vector<ForLeaseEngine::State *> appendTo) {
    FLE::Serializer serial;
    serial.ReadFile(levelList);
    FLE::Serializer levelArray = serial.GetChild("Levels");
    FLE::ArraySerializer levels(levelArray);

    for (unsigned i = 0; i < levels.Size(); ++i) {
        FLE::Serializer level = levels[i];
        std::string file;
        level.ReadString("File", file);

        FLE::Level* levelInstance = new FLE::Level(file);

        appendTo.push_back(levelInstance);
        std::cout << "Just got " << file << std::endl;
        std::cout << "Name: " << levelInstance->GetName() << std::endl;
    }

    return appendTo;
}

std::vector<ForLeaseEngine::State *> LoadSingleLevel(std::string levelFileName, std::vector<ForLeaseEngine::State *> appendTo) {
    FLE::Level* level = new FLE::Level(levelFileName);
    appendTo.push_back(level);
    return appendTo;
}

namespace CommandLine {
    bool ArgumentExists(char** start, char** end, const std::string argument) {
        char** result = std::find(start, end, argument);

        if (result == end)
            return false;
        else
            return true;
    }

    StringArgument GetStringArgument(char** start, char** end, const std::string argument) {
        char ** result = std::find(start, end, argument);

        if (result != end && ++result != end)
            return std::pair<bool, std::string>(true, *result);

        return StringArgument(false, "Argument not found.");
    }

    namespace {

        template<typename T>
        T convert(const std::string& input) {
            std::istringstream inputStream(input);
            T converted;

            inputStream >> converted;

            if (!inputStream.eof())
                throw std::invalid_argument("Not integral.");

            return converted;
        }

    }

    IntArgument GetIntArgument(char** start, char** end, const std::string argument) {
        StringArgument result = GetStringArgument(start, end, argument);

        if (result.first) {
            try {
                return IntArgument(true, convert<int>(result.second.c_str()));
            }
            catch (std::invalid_argument) {
                return IntArgument(false, -1);
            }
        }

        return IntArgument(false, -1);
    }
}

void SpawnProcess(const std::string process, std::string arguments) {
    #ifdef FLE_WINDOWS

        // Shamelessly stolen from Mead's CS180 notes--I'm going to modify it later.
        STARTUPINFO startInfo;
        PROCESS_INFORMATION processInfo;

        DWORD pid = GetCurrentProcessId();
        std::cout << "parent pid = " << pid << std::endl;

        // allocate memory and set to 0
        ZeroMemory(&startInfo, sizeof(STARTUPINFO));
        ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

        std::cout << "creating child process" << std::endl;
        const char *program = "..\\blisstopia\\Blisstopia.exe";
        BOOL err = CreateProcess(program,     // program to run
            0,           // command line
            0,           // security attributes
            0,           // thread attributes
            FALSE,       // don't inherit handles
            CREATE_UNICODE_ENVIRONMENT,           // creation flags (none)
            0,           // use parent's environment
            "..\\blisstopia",           // use parent's directory
            &startInfo, // start up info
            &processInfo   // process info
            );

        if (!err)
        {
            std::cout << "Error creating process" << std::endl;
            return;
        }

        std::cout << "waiting for child to terminate" << std::endl;
        WaitForSingleObject(processInfo.hProcess, INFINITE);
        std::cout << "parent terminating" << std::endl;

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

    #endif
}