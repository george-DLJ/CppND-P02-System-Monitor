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
vector<Process>& System::Processes() { return processes_; }

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