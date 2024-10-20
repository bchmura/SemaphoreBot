#ifndef SEMAPHOREFLAGS_HPP
#define SEMAPHOREFLAGS_HPP

#include <ArduinoLog.h>
#include <map>
#include <string>
#include <cstring>
#include <tuple>
#include <vector>

enum class FlagType {letter, number, special };

class SemaphoreFlags {
    public:
        SemaphoreFlags();
        std::tuple<std::string, FlagType, int, int> GetSemaphoreSymbol(std::string string);
        std::vector<std::tuple<std::string, FlagType,int,int>> ConvertToFlagSequence(const std::string &);

    private:
        std::map<std::string, std::tuple<FlagType, int, int>> library;
        void LoadSemaphoreLibrary();

};



#endif //SEMAPHOREFLAGS_HPP
