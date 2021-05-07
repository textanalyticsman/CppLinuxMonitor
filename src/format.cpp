#include <string>
#include <iomanip>

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

    // I am using this answer https://knowledge.udacity.com/questions/155686
    std::ostringstream stream;
      stream << std::setw(2) << std::setfill('0') << tdays << ":"
      <<std::setw(2) << std::setfill('0') << thours << ":" 
      << std::setw(2) << std::setfill('0') << tminutes << ":"
      << std::setw(2) << std::setfill('0') << tseconds;

    return stream.str();
}