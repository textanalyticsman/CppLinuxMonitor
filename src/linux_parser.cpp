#include <dirent.h>
#include <unistd.h>
#include <sstream>
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
  string os, kernel, version;
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

// I am using regular expressions to look for text where is possible
// The first parameter is the pattern I am using to match
// The second parameter is the text (line from a Linux file) I will analyse
// The third parameter is a group because I want to use groups to get the proper data
// For example, R"((\w*:)(\s)*(\d+)(\s*)(kB)?)" in this case the third group will allow
// us to get data from this text "MemTotal:       33263352 kB"
std::string LinuxParser::RegularExpression(std::string patttern, std::string text, int group)
{
  std::string result;
  std::regex pat{patttern};
  std::smatch matches;

  if (regex_search(text, matches, pat))
    result = matches.str(group);

  return result;
}

// TODO: Read and return the system memory utilization
// I need to get the first value from MemTotal:       33263352 kB
// Then I need the second value from MemFree:        23736848 kB
// Last I have to execute (MemTotal-MemFree)/100 to get the ratio
// When it comes to memory the file always starts MemTotal and then MemFree
float LinuxParser::MemoryUtilization() 
{ 
  std::string line;
  float value{0};
  // Vector to save total and free memory values
  std::vector<float> memory_values;

  // Pattern to look for memory information inside /proc/meminfo
  std::string pattern{ R"((Mem\w*:)(\s)*(\d+)(\s*)(kB)?)" };

  std::ifstream filestream("/proc/meminfo");
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::string regular_expression_result = RegularExpression(pattern, line, 3);
      if(regular_expression_result != "")
        memory_values.push_back(std::stof(regular_expression_result));     
    }
  }
  
  // If everything goes well we should have a vector with total and free memory
  if(memory_values.size() == 2)
  {
    float total = memory_values[0]; 
    float free = memory_values[1];

    value = (total - free)/total;
  }

  return value;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

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
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

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
