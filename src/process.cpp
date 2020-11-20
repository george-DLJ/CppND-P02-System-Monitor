#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip> //to set stream float decimal precission.

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


const long Process::kHertz = sysconf(_SC_CLK_TCK); //Static constant set at runtime to avoid call sysconf on each tics calculation.

/**
 * Constructor overwritten we need to initialize pid and 
 * at least cpuUtilization, in order to be able to sort the
 * Processes.
 */
Process::Process(int pid) : pid_(pid){
    cpuUtilization_ = CpuUtilization();
} 

// Return this process's ID
int Process::Pid() { return pid_; }

/**
 *  Return this process's CPU utilization
 */
float Process::CpuUtilization() { 
    long pidTotalTime = LinuxParser::ActiveJiffies(this->pid_)/kHertz; // unit: seconds (after divide by kHertz)
    long pidStartTime = LinuxParser::UpTime(this->pid_)/kHertz; // unit: seconds (after divide by kHertz)
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
 * Return this process's memory utilization in MB!
 * 
 * NOTE: the display expects the data in MB. Therefore 
 *       the parsed data is here converted to MB.
 * 
 * @return memory used by a process in MB.
 */ 
string Process::Ram() { 
    // convert data to MB with one decimal:
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << stof(LinuxParser::Ram(this->pid_)) / 1024.0;
    //std::string s = stream.str(); 
    return stream.str();
}

/**
 * DONE: Return the user (name) that generated this process
 */ 
string Process::User() { 
    if(this->user_.empty()){
        this->user_ =  LinuxParser::User(this->Pid()); 
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
    long seconds = LinuxParser::UpTime() - (starttime / kHertz); //NOTE: UpTime() without paramters returns system uptime.
    return seconds; 
}

/**
 * Overload the "less than" comparison operator for Process objects
 */
bool Process::operator<(const Process & other) const{ 
    return this->cpuUtilization_< other.cpuUtilization_; 
}