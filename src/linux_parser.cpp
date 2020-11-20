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

// TODO(?): Read and return the number of jiffies for the system
//          I'm confused because the linux man says there are not 
//          jiffies any more since Kernel 2.6 but clock tics
// NOTE: I asume (system cpu) jiffies value is the sum of all values but guest.
//         jiffies: user + nice + system + idle + io 
//       from cpu line in /proc/stat file.        
// see: https://man7.org/linux/man-pages/man5/proc.5.html
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
}

// (?): Read and return the number of active jiffies for a PID
// It is not clear if I should use just utime+ stime or als
// add the children nodes (cutime + cstime) 
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string key;
  string token;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    long utime{0};
    long stime{0};
    long cutime{0};
    long cstime{0};
    long activeJiffies {0};
    // data is in a single line:
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 1;
    while (linestream >> token){ //return the nth token. 
      switch(i) {
        case LinuxParser::PidStat::kutime:
          utime = stol(token);
          break;
        case LinuxParser::PidStat::kstime:
          stime = stol(token);
          break;
        case LinuxParser::PidStat::kcutime:
          cutime = stol(token);
          break;
          //activeJiffies +=  stol(token);
        break;
        case LinuxParser::PidStat::kcstime: //this is the token with higher index: add to sum and return;
          //activeJiffies +=  stol(token);
          cstime = stol(token);
          activeJiffies = utime + stime + cutime + cstime; 
          return activeJiffies;
      }
      i++;
    }
  }
  return 0;  
}

/** 
 * TODO: Read and return the number of active jiffies for the system
 * 
 * NOTE: It is not clear on the project description und neither on linux man
 *      but I asume it is the cpu non-idle time from file /proc/stat:
 *        Jiffies = user + nice + system + irq + softirq + steal
 */ 
long LinuxParser::ActiveJiffies() { 
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  if (cpuUtilization.size() > CPUStates::kSteal_){ //NOTE: kSteal_: last used cpu state index.
    return stol(cpuUtilization[kUser_]) + 
            stol(cpuUtilization[kNice_]) + 
            stol(cpuUtilization[kSystem_]) + 
            stol(cpuUtilization[kIRQ_]) + 
            stol(cpuUtilization[kSoftIRQ_]) +
            stol(cpuUtilization[kSteal_]);
  }
  return 0; 
}

/**
 * Read and return the number of idle jiffies for the system
 * NOTE: It is not clear on the project description und neither on linux man
 *       how is this value calculated. I asume it is the cpu idle time from 
 *       file /proc/stat:
 *         IdleJiffies = idle + iowait
 */ 
long LinuxParser::IdleJiffies() { 
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  if (cpuUtilization.size() > CPUStates::kSteal_){ //NOTE: kSteal_: last used cpu state index.
    return stol(cpuUtilization[kIdle_]) +
            stol(cpuUtilization[kIOwait_]);
  }
  return 0; 
}

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

/**
 *  Read and return the number of running processes.
 * 
 *  This information is stored on /proc/stat
 *  
 */
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
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

/**
 * Read and return the command associated with a process
 * 
 * file contains a single line; return all of it.
 */
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line; 
  if (stream.is_open()) {
     std::getline(stream, line);
    return line;
  }
  return string();
}

/**
 * Read and return the memory used by a process 
 * TODO: convert to MB?
 * 
 */
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
   if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        //TODO: convert to MB(?)
        return value;
      }
    }
  } 
  return string();  
}

/**
 *  Read and return the user ID associated with a process
 */
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
   if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  } 
  return string(); 
}

/**
 *  Read and return the user associated with a process
 */
string LinuxParser::User(int pid) { 
  return UserName(Uid(pid)); 
}

/** 
 * Read and return the pid 'starttime' of a process in clocktics.
 * from file /proc/[PID]/stat
 * 
 * TODO: consider rename the functiontion to starttime
 * @return [pid] uptime in raw units (e.g. clock_tics)
 */ 
long LinuxParser::UpTime(int pid) {
  string line;
  string key;
  string token;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    // data is in a single line:
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 1;
    while (linestream >> token){ //return the nth token. 
      if (i == LinuxParser::PidStat::kstartime){
        return stol(token);
      }
      i++;
    }
  }
  return 0; 
}

/**
 * Helper Function. Reads the username that belongs to a given Uid
 * /etc/passwd contains following format (where ':' is separator):
 *   username:pwd:uid:gid:UserIdInfo:home/dir:command/shell
 *  NOTE: some elements may contain spaces.
 */ 
string LinuxParser::UserName(std::string userId){
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    string username, pwd, uid;
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_'); // Replace empty spaces with underscores
      std::replace(line.begin(), line.end(), ':', ' '); // (!) separator is ':' replace it with empty space
      std::istringstream linestream(line);
      linestream >> username >> pwd >> uid; 
      if (uid == userId){
        return username;
      }
    }
  }
  return userId; 
}
