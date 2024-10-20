//
// Created by bill on 10/18/2024.
//

#include "SemaphoreFlags.hpp"
#include "ArduinoLog.h"

SemaphoreFlags::SemaphoreFlags() {
    Log.verboseln("Starting semaphoreFlags");
    LoadSemaphoreLibrary();
}

std::vector<std::tuple<std::string, FlagType,int,int>> SemaphoreFlags::ConvertToFlagSequence(const std::string &source) {
    Log.verboseln("SemaphoreFlags to convert %S", source.c_str());
    std::vector<std::tuple<std::string, FlagType,int,int>> flagSequence;
    bool isAlphaMode = true;

    for(int pos = 0; pos < source.length(); pos++) {
        Log.verboseln("... Mapping char %S from pos %d", std::string(1,source[pos]).c_str(), pos );
        if (isAlphaMode && std::isdigit(source[pos])) {
            Log.verboseln("... Switching to numbers");
            flagSequence.push_back(GetSemaphoreSymbol("NUMBERSTOFOLLOW"));
            isAlphaMode = false;
        } else if (! isAlphaMode && std::isalpha(source[pos]))  {
            Log.verboseln("... Switching to letters");
            flagSequence.push_back(GetSemaphoreSymbol("LETTERSTOFOLLOW"));
            isAlphaMode = true;
        }

        flagSequence.push_back(GetSemaphoreSymbol(std::string(1,source[pos])));

        if (std::get<0>(flagSequence.back()) == "ERROR_1") {
            Log.verboseln("... Adding error sequence");
            flagSequence.push_back(GetSemaphoreSymbol("ERROR_2"));
            flagSequence.push_back(GetSemaphoreSymbol("ERROR_1"));
            flagSequence.push_back(GetSemaphoreSymbol("ERROR_2"));
        }
    }

    flagSequence.push_back(GetSemaphoreSymbol("REST"));

    return flagSequence;
}

std::tuple<std::string, FlagType, int, int> SemaphoreFlags::GetSemaphoreSymbol(std::string inputString) {

    std::string requestedChar = inputString;

    if (library.find(inputString) == library.end()) {
        inputString = "ERROR_1";
    }

    std::tuple<FlagType, int, int> libTuple = library[inputString];
    std::tuple<std::string,FlagType, int, int> returnTuple = {inputString,
                                                                std::get<0>(libTuple),
                                                                std::get<1>(libTuple),
                                                                std::get<2>(libTuple)  };

    return returnTuple;
}

void SemaphoreFlags::LoadSemaphoreLibrary() {
  // type, left, right

  library["A"] = std::make_tuple(FlagType::letter, 180, 225);
  library["B"] = std::make_tuple(FlagType::letter, 135, 225);
  library["C"] = std::make_tuple(FlagType::letter, 90, 225);
  library["D"] = std::make_tuple(FlagType::letter, 45, 225);
  library["E"] = std::make_tuple(FlagType::letter, 225, 90);
  library["F"] = std::make_tuple(FlagType::letter, 225, 135);
  library["G"] = std::make_tuple(FlagType::letter, 225, 180);
  library["H"] = std::make_tuple(FlagType::letter, 135, 270);
  library["I"] = std::make_tuple(FlagType::letter, 90, 270);
  library["J"] = std::make_tuple(FlagType::letter, 45, 135);
  library["K"] = std::make_tuple(FlagType::letter, 180, 45);
  library["L"] = std::make_tuple(FlagType::letter, 180, 90);
  library["M"] = std::make_tuple(FlagType::letter, 180, 135);
  library["N"] = std::make_tuple(FlagType::letter, 180, 180);
  library["O"] = std::make_tuple(FlagType::letter, 135, 0);
  library["P"] = std::make_tuple(FlagType::letter, 135, 45);
  library["Q"] = std::make_tuple(FlagType::letter, 134, 90);
  library["R"] = std::make_tuple(FlagType::letter, 135, 135);
  library["S"] = std::make_tuple(FlagType::letter, 135, 180);
  library["T"] = std::make_tuple(FlagType::letter, 90, 45);
  library["U"] = std::make_tuple(FlagType::letter, 90, 90);
  library["V"] = std::make_tuple(FlagType::letter, 45, 180);
  library["W"] = std::make_tuple(FlagType::letter, 0, 135);
  library["X"] = std::make_tuple(FlagType::letter, 0, 180);
  library["Y"] = std::make_tuple(FlagType::letter, 90, 135);
  library["Z"] = std::make_tuple(FlagType::letter, 270, 135);

  library["1"] = std::make_tuple(FlagType::number, 180, 225);
  library["2"] = std::make_tuple(FlagType::number, 135, 225);
  library["3"] = std::make_tuple(FlagType::number, 90, 225);
  library["4"] = std::make_tuple(FlagType::number, 45, 225);
  library["5"] = std::make_tuple(FlagType::number, 225, 90);
  library["6"] = std::make_tuple(FlagType::number, 225, 135);
  library["7"] = std::make_tuple(FlagType::number, 225, 180);
  library["8"] = std::make_tuple(FlagType::number, 135, 270);
  library["9"] = std::make_tuple(FlagType::number, 90, 270);
  library["0"] = std::make_tuple(FlagType::number, 180, 45);

  library[" "] = std::make_tuple(FlagType::number, 225, 225);

  library["NUMBERSTOFOLLOW"] = std::make_tuple(FlagType::special, 90, 225);
  library["LETTERSTOFOLLOW"] = std::make_tuple(FlagType::special, 45, 135);
  library["REST"] = std::make_tuple(FlagType::special, 225, 225);
  library["CANCEL"] = std::make_tuple(FlagType::special, 90, 180);
  library["ACKNOWLEDGE"] = std::make_tuple(FlagType::special, 90, 225);
  library["ERROR_1"] = std::make_tuple(FlagType::special, 90, 90);
  library["ERROR_2"] = std::make_tuple(FlagType::special, 180, 180);
  library["ATTENTION_1"] = std::make_tuple(FlagType::special,90,90);
  library["ATTENTION_2"] = std::make_tuple(FlagType::special,180,180);

}




