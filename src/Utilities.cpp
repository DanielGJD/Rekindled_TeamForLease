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
#include "Exception.h"

#ifndef FLE_WINDOWS // Platform-specific crap used in SpawnProcess
    #include <stdio.h>    /* printf         */
    #include <stdlib.h>   /* exit           */
    #include <unistd.h>   /* fork, getpid   */
    #include <sys/wait.h> /* wait           */
    #include <cstring>
#endif

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

namespace {
    /*!
        Helper function for spawning a new process.  This is designed to be
        cross-platform; if anyone has issues on any system with it, please let me
        know on Slack.

        It's ugly.  It's gross.  But according to my testing, it works across platforms.

        I really hate platform-dependent code.

        Usage follows:

            process:      A relative position of the thing we want to launch.  This is
                          relative to execLocation (discussed below).  Since this requires
                          nasty string parsing to work, don't include any normal relative
                          links (like ./, ..\, etc.).

            arguments:    A space-delimited string of the arguments to pass to the thing
                          being launched.

            execLocation: The directory to execute in.  This is necessary if the thing
                          being launched requires dlls, resources, etc.
    */
    void SpawnProcess(const std::string& process, const std::string& arguments, const std::string& execLocation) {
#ifdef FLE_WINDOWS

        std::stringstream ss;

        ss << execLocation << "/" << process;

        STARTUPINFO startInfo;
        PROCESS_INFORMATION processInfo;

        DWORD pid = GetCurrentProcessId();

        // Zero out relevant memory
        ZeroMemory(&startInfo, sizeof(STARTUPINFO));
        ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

        // Copy the string to a vector of characters
        std::vector<char> command_line(arguments.c_str(), arguments.c_str() + arguments.size() + 1);

        BOOL err = CreateProcess(ss.str().c_str(), &command_line[0], 0, 0, FALSE,
            CREATE_UNICODE_ENVIRONMENT, 0, execLocation.c_str(),
            &startInfo, &processInfo);

        if (!err) throw FLE::Exception("Could not create process.");

        WaitForSingleObject(processInfo.hProcess, INFINITE);

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);


#else

        int pid;

        pid = fork();

        if (pid == 0) /* child */
        {
            std::vector<char*> command_line;

            char *procCString = new char[process.size() + 1];
            strcpy(procCString, process.c_str());
            command_line.push_back(procCString);

            const char* argCharacter = arguments.c_str();
            int words = 0;

            while (*argCharacter != 0) {
                if (*argCharacter == ' ')
                    ++words;
                ++argCharacter;
            }
            ++words;

            argCharacter = arguments.c_str();

            for (int i = 1; i <= words; ++i) {
                const char* first = argCharacter;
                while (*argCharacter != ' ' && *argCharacter != 0) ++argCharacter;

                command_line.push_back(new char[(argCharacter - first) + 1]);
                char* copy = command_line.back();
                while (first < argCharacter) {
                    *copy = *first;
                    ++first;
                    ++copy;
                }

                *copy = 0;
                ++argCharacter;
            }

            // Enter correct directory
            chdir(execLocation.c_str());
            // Finally get to make the process
            execv(process.c_str(), &command_line[0]);

            for (char* toDelete : command_line)
                delete[] toDelete;

            delete[] procCString;

            exit(10);
        }
        else { /* parent */
            int code, status;
            wait(&status);
            code = WEXITSTATUS(status);
        }

#endif
    }
}


/*!
    This is highly dependent on knowing exactly where Blisstopia.exe is in relation
    to the level editor.  I would highly recommend placing both the editor and
    the game binary in the same directory in the repo.
*/
void SpawnNewLevelProcess(const std::string& level) {
    std::stringstream args;
    args << "-level " << level;
    SpawnProcess("Blisstopia.exe", args.str(), "");
}