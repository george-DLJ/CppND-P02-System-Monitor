#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <iostream> //TEST ONLY TODO: remove

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

System::System(){
    kernel_ = LinuxParser::Kernel(); 
    os_ = LinuxParser::OperatingSystem(); 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids {LinuxParser::Pids()};
    // First approach: regenerate the processes_ vector each time.
    processes_.clear();
     // if there are still new pids, add to procesess_
    for(int pid: pids){
        Process newProcess(pid);
        float pidCpu = newProcess.cpuUtilization_;
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

    // Order processes by CPU use:
    // NOTE: the vector should be ordered in descending order because we want the higher CPU usage first 
    //       but we have implemented only operator< which orders the vector ascending and 
    //       ncurses_display reads the vector from index 0. Therefore we need reorder the other way around.
    // NOTE: use rbegin() and rend() to order the vector using reverse iterators. 
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
 * DONE: (1) implement parser
 * TODO: (2) improve by creating a function to refresh statistics and updates class fields
 *  instead of consulting the system files each time.
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

//TODO: REMOVE
void System::TestSortFunction()
{
    std::vector<Process> processesList = {};
    Process proc1(1);
    proc1.cpuUtilization_ = 0.1;
    processesList.push_back(proc1);

    Process proc2(2);
    proc2.cpuUtilization_ = 0.2;
    processesList.push_back(proc2);

    Process proc3(3);
    proc3.cpuUtilization_ = 0.3;
    processesList.push_back(proc3);

    std::cout << "Input vector:\n";
    PrintProcessesData(processesList);
    std::sort(processesList.rbegin(), processesList.rend());

    std::cout << "After Sort():\n";
    PrintProcessesData(processesList);

}

//TODO: REMOVE
void System::PrintProcessesData(std::vector<Process> &processes)
{
    for(Process p: processes)
    {
        std::cout << "  " << p.Pid() << "; " << p.cpuUtilization_ << "\n";
    }
}