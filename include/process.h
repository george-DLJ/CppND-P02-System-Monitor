#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int const pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  float cpuUtilization_;

 private:
    int pid_;
    std::string user_;
    std::string command_;
    //float cpuUtilization_;
    //static constexpr long kHertz = sysconf(_SC_CLK_TCK); //kHertz (Clock tics per second) this static constant at cannot be set at compile time due to sysconf()! 
    //const static  long kHertz; //therefore it is declared as static constant at runtime. NOTE: const crashes the sort function.
};

#endif