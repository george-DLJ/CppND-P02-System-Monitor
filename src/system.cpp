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

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids {LinuxParser::Pids()};
    // First approach: regenerate the processes_ vector each time.
    processes_.clear();
     // if there are still new pids, add to procesess_
    for(int pid: pids){
        Process newProcess(pid);
        newProcess.User();
        processes_.push_back(newProcess);
    }

    // Second Approach: 
    //  1. Check first which processes are still active (in Pids() vector)
    //  1.1 - update info of all processes that are in the pids list
    //      - remove found process from pids list
    //  1.2 erase Processes that are NOT on the list
    //  2. if stille there are pids (new ones)
    //  2.1 create new Process in processes_
//    for(int i; i < processes_.size() ; ++i )
//     {
//         if find(proc.Pid(), pids)
//         {   // remove pid from pids

//             // update process data

//         } else {
//             // delete process -> proces may be ended; remove from list.
//             processes_.erase(i);
//         }
//     }

    return processes_; 
}

/**
 * Return the system's kernel identifier (string)
 * DONE: (1) implement parser
 * TODO: (2) improve by creating an initializer function to parse the data only once.
 */ 
std::string System::Kernel() { return LinuxParser::Kernel(); }

/**
 * Return the system's memory utilization
 * DONE: (1) implement parser
 * TODO: (2) improve by creating a function to refresh statistics and updates class fields
 *  instead of consulting the system files each time.
 */ 
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

/**
 * Return the operating system name
 * DONE: (1) implement parser
 * TODO: (2) improve by creating an initializer function to parse the data only once.
 */ 
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

/**
 * Return the number of processes actively running on the system
 * TODO: (1) implement parser
 * TODO: (2) improve by creating a function to refresh statistics and updates class fields
 *  instead of consulting the system files each time.
 */
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

/**
 * Return the total number of processes on the system
 * TODO: (1) implement parser
 * TODO: (2) improve by creating a function to refresh statistics and updates class fields
 *  instead of consulting the system files each time.
 */ 
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

/* Return the number of seconds since the system started running
 * DONE: (1) implement parser
 * TODO: (2) improve by creating a function to refresh statistics and updates class fields
 *  instead of consulting the system files each time.
 */ 
long int System::UpTime() { return LinuxParser::UpTime(); }