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


 //const static long Process::kHertz = sysconf(_SC_CLK_TCK); //Static constant set at runtime: NOTE: const  makes sort fail

// Constructor
Process::Process(int pid) : pid_(pid), user_(""), command_(""), cpuUtilization_(0.0){
    cpuUtilization_ = CpuUtilization();
} 

// Return this process's ID
int Process::Pid() { 
   
    return pid_; 
}

/**
 *  Return this process's CPU utilization
 */
float Process::CpuUtilization() { 
    long pidTotalTime = LinuxParser::ActiveJiffies(this->pid_)/sysconf(_SC_CLK_TCK); // unit: seconds
    long pidStartTime = LinuxParser::UpTime(this->pid_)/sysconf(_SC_CLK_TCK); // unit: seconds
    long systemUptime = LinuxParser::UpTime(); //unit: sytem uptime already in seconds
    return (float)pidTotalTime/(float)(systemUptime - pidStartTime);
}

/**
 * Return the command that generated this process
 */
string Process::Command() { 
    if(this->command_.empty()){
        this->command_ = LinuxParser::Command(this->pid_);
    }
    return this->command_;
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
    if(this->user_.empty()){
        this->user_ =  LinuxParser::User(this->Pid()); //NOTE: (?) it is better access the field directly or use the accessor method?
    }
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
    long seconds = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK)); //NOTE: UpTime() without paramters returns system uptime.
    return seconds; 
}

/**
 * Overload the "less than" comparison operator for Process objects
 *  TODO: CORRECT it does not sort properly
 */
bool Process::operator<(const Process & other) const{ 
    float ft = this->cpuUtilization_;
    float fa = other.cpuUtilization_;
    return this->cpuUtilization_< other.cpuUtilization_; //it doesn't order properly
}