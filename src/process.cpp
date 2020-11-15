#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


const long Process::kHertz = sysconf(_SC_CLK_TCK); //Static constant set at runtime.

// Constructor
Process::Process(int pid) : pid_(pid), user_(""), command_(""){}

// Return this process's ID
int Process::Pid() { 
    return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long pidTotalTime = LinuxParser::ActiveJiffies(this->pid_)/kHertz; // unit: seconds
    long pidStartTime = LinuxParser::UpTime(this->pid_)/kHertz; // unit: seconds
    long systemUptime = LinuxParser::UpTime(); //unit: seconds
    return (float)pidTotalTime/(float)(systemUptime - pidStartTime);
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(this->pid_);
    return string(); 
}

/**
 * Return this process's memory utilization
 */ 
string Process::Ram() { 
    return LinuxParser::Ram(this->pid_);
}

// DONE: Return the user (name) that generated this process
// TODO: improve: return field data if initialized.
string Process::User() { 
    // if not set retrieve from parser.
    if(this->user_ == string()){
        return LinuxParser::User(this->Pid()); //NOTE: (?) it is better access the field directly or use the accessor method?
    }
    // otherwise return stored value.
    return this->user_;
}

/**
 * Return the age of this process (in seconds)
 * (!) parser returns the value in its raw value: in clock_tics. 
 * This function converts it to a readable format: seconds
 * to do so the constant kHertz (Clock tics per second) is used.
 * @return process uptime in seconds
 */
long int Process::UpTime() {
    long starttime = LinuxParser::UpTime(this->pid_);
    long seconds = LinuxParser::UpTime() - (starttime / kHertz); //NOTE: UpTime() without paramters returns system uptime.
    return seconds; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }