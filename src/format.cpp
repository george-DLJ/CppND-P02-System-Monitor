#include <string>
#include <sstream> //NOTE: required for ostringstream
#include <iomanip> //NOTE: required for setw and setfill

#include "format.h"

using std::string;

/** helper function to show seconds in  HH:MM:SS format
 * @param seconds measuring seconds
 * @return string with format HH:MM:SS
 */
string Format::ElapsedTime(long seconds) {
    int ss = (int)seconds % 60; 
    int mm = (int)(((seconds)/60) % 60);
    long hh = seconds / 3600;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << std::to_string(hh) << ":" 
        << std::setw(2) << std::setfill('0') << std::to_string(mm) << ":" 
        << std::setw(2) << std::setfill('0') << std::to_string(ss);
    return oss.str(); 
}