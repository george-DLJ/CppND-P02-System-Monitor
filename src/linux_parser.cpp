#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
//  (!) requires update gcc to c++17
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


/**
 *  Helper function to read the tokens on files.
 */
inline string readToken(const string tokenName, const string &line )
{
  string key;
  string value;
  std::istringstream linestream(line);
      linestream >> key >> value; // >> units; (kB)
      if (key == tokenName) {
        return value; 
      }
  return "";
}


/**
 * Retrieves the system memory utilization.
 * 
 * This function uses /proc/meminfo to calculate the memory utilization. The 
 * calculation is done using just MemTotal and Memfree parameters. 
 */
float LinuxParser::MemoryUtilization() { 
  float memtotal = 0.0, memfree = 0.0 ; //Unused: memavailable = -1, buffers = -1;
  string line;
  string key;
  string value;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    {
      // 1st line expected MemTotal value.
      std::getline(stream, line);
      
      std::istringstream linestream(line);
      linestream >> key >> value; // >> units; (kB)
      if (key == "MemTotal:") {
      memtotal = stof(value); 
       }
      // Alternative: use a helper function. 
      // It works but crashes when token is not found on line (or line is empty).
      // string tokenValue = readToken("MemTotal:", line );
      // memtotal = stof(tokenValue);
    }
    {
      // 2nd line expected MemFree value.
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value; // >> units; (kB)
      if (key == "MemFree:") {
        memfree = stof(value);
      }
    }
  }
  return (memtotal - memfree) / memtotal; 
}

/**
 * Read and return the system uptime
 * 
 * The information is gathered from /proc/uptime file which contains two values:
 *  - the uptime of the system (including time spent in suspend) and 
 *  - the amount of time spent in the idle process
 * This file contains a single line.
 */
long int LinuxParser::UpTime() { 
    long int uptimeValue = 0;
    string line;
    string uptimeStr;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptimeStr;
      uptimeValue = stol(uptimeStr);
      return uptimeValue;
    }
    return uptimeValue;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string cpuKey;
  string cpuStat;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> cpuKey;
      if (cpuKey == "cpu") {
        vector<string> cpuUtilization {};
        while (linestream >> cpuStat){
          cpuUtilization.push_back(cpuStat);
        }
        return cpuUtilization;
      }
    }
  }
  return {}; 
}

/** 
 * Read and return the total number of processes
 * 
 * This  information can be found on /proc/stat file
 * under the token 'processes'
 * 
 * This file contains multiple lines of information.
 */ 
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  string uptime;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  string uptime;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return stoi(value);
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }