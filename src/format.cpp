#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// I have taken the idea from here: https://www.geeksforgeeks.org/converting-seconds-into-days-hours-minutes-and-seconds/
string Format::ElapsedTime(long seconds) 
{
    long int tdays{ seconds/(24 * 3600) };
    seconds %= (24 * 3600);

    long int thours{ seconds/3600 };
    seconds %= 3600;

    long int tminutes{ seconds/60 };
    seconds %= 60;

    long int tseconds{ seconds };
    
    string result { std::to_string(tdays) + ":" + std::to_string(thours) + ":" + std::to_string(tminutes) + ":" + std::to_string(tseconds)};

    return result;
}