#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

/**
 * First Approach: implementd using a pass-thorugh strategy to gather the 
 * information from linux_parser.
 * This is not optimal, but has a more simple implementation.
 * 
 * Second Approach: 
 * Initialize once non-dynamic data like Kernel, OS vs. and store in fields
 * Update dynamic fields, like CPU, memory, processes, etc. once a timeout is 
 * due, e.g. once per second. Otherwise return stored values.
 * This will optimice the access to the system files.
 */
class System {
 public:
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif