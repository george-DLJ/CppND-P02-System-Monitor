#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

/**
 * Constructor ovewritten to avoid read continuously data 
 * that does not change like kernel and OS
 */
System::System(){
    kernel_ = LinuxParser::Kernel(); 
    os_ = LinuxParser::OperatingSystem(); 
}

/**
 *  Return a container composed of the system's processes
 * 
 *  Order processes by CPU use descending:
 *  NOTE: the vector should be ordered in descending order because we want the higher CPU usage first 
 *         but we have implemented only operator< which orders the vector ascending and 
 *         ncurses_display reads the vector from index 0. Therefore we need reorder the other way around.
 *  NOTE: use rbegin() and rend() to order the vector using reverse iterators.
 */ 
vector<Process>& System::Processes() { 
    vector<int> pids {LinuxParser::Pids()};
    processes_.clear();
    for(int pid: pids){
        Process newProcess(pid);
        processes_.push_back(newProcess);
    } 
    std::sort(processes_.rbegin(), processes_.rend());
    return processes_; 
}

/**
 * Return the system's kernel identifier (string)
 *  
 * Improved version: assumed Kernel doesn't changes, 
 *           therefore, LinuxParser is used only on constructor.
 */ 
std::string System::Kernel() { 
    return kernel_; 
}

/**
 * Return the system's memory utilization
 * 
 * Implemented using a pass-through strategy.
 */ 
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

/**
 * Return the operating system name
 * Improved version: assumed Kernel doesn't changes, 
 *           therefore, LinuxParser is used only on constructor.
 */ 
std::string System::OperatingSystem() { 
    return os_;
}

/**
 * Return the number of processes actively running on the system
 * 
 * Implemented using a pass-through strategy.
 */
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

/**
 * Return the total number of processes on the system
 * 
 * Implemented using a pass-through strategy.
 */ 
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

/**
 * Return the number of seconds since the system started running
 * 
 * Implemented using a pass-through strategy.
 */ 
long int System::UpTime() { return LinuxParser::UpTime(); }
