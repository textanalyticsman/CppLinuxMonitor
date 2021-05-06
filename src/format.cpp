#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{
    long int tdays{ seconds/60/60/24 };
    long int thours{ seconds/60/60%24 };
    long int tminutes{ seconds/60%60 };
    long int tseconds{ seconds%60 };
    
    string result { std::to_string(tdays) + ":" + std::to_string(thours) + ":" + std::to_string(tminutes) + ":" + std::to_string(tseconds)};

    return result;
}