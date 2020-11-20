#include "processor.h"
#include <vector>
#include <string>
#include <iostream> //DEBUG try-catch block

#include "linux_parser.h"

using std::vector;
using std::string;
using LinuxParser::CPUStates;

/** 
 * Returns the aggregate CPU utilization
 * 
 * Calculates overall CPU utilization based on following values 
 * from "cpu" line on '/proc/stats' file.
 * 
 * To calcupate the values it has been used the formula explained on following 
 * article: Accurate calculation of CPU [...] in Linux (https://stackoverflow.com/a/23376195/1806436)
 * Using following jeffies:
 *  - Idle Time:    idle + iowait
 *  - NonIdle Time: user + nice + system + irq + softirq + steal
 *  - Total = Idle + NonIdle  
 *  CPU_Percentage = (Total - Idle)/Total 
 * 
 * @return CPU utilization in percentage.
 */
float Processor::Utilization() { 
    
    std::vector<std::string> cpuUtilization = LinuxParser::CpuUtilization();
    try {
         float idle = stof(cpuUtilization[CPUStates::kIdle_]) + stoi(cpuUtilization[CPUStates::kIOwait_]);   
        float nonIdle = stof(cpuUtilization[CPUStates::kUser_]) + 
                    stof(cpuUtilization[CPUStates::kNice_]) +
                    stof(cpuUtilization[CPUStates::kSystem_]) +
                    stof(cpuUtilization[CPUStates::kIRQ_]) +
                    stof(cpuUtilization[CPUStates::kSoftIRQ_]) +
                    stof(cpuUtilization[CPUStates::kSteal_]);
        float total = idle + nonIdle;
        float totalDiff = total - prevTotal_;
        float idleDiff = idle - prevIdle_;
        float CPU_Percentage = (totalDiff - idleDiff) / totalDiff; // More dynamic Calculation (changes between cpu utilisation calls)
        prevTotal_ = total;
        prevIdle_ = idle; 
        return CPU_Percentage;
    }
    catch(const std::exception& e) {
        std::cout << "ERROR calculatng CPU Utilization: "
                    << e.what() << "'\n";
    }   
    return 0.0; 
}