#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unistd.h> //
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int const pid);
  int Pid();                               
  std::string User();                     
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
    int pid_;
    std::string user_;
    std::string command_;
    float cpuUtilization_;
    
    //static constexpr long kHertz = sysconf(_SC_CLK_TCK); //kHertz (Clock tics per second) this static constant at cannot be set at compile time due to sysconf()! 
    const static long kHertz; //therefore it is declared as static constant at runtime. NOTE: const crashes the sort function.
};

#endif