#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

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

/* 
 I need to get the first value from MemTotal:       33263352 kB
 Then I need the second value from MemFree:        23736848 kB
 Last I have to execute (MemTotal-MemFree)/100 to get the ratio
 When it comes to memory the file always starts MemTotal and then MemFree
 On WSL /proc/meminfo returns
 MemTotal:       33263352 kB
 MemFree:        22359900 kB
 However, on Ubuntu without WSL it returns
 MemTotal:        3825964 kB
 MemFree:         1262180 kB
 MemAvailable:    2518760 kB
 Thus the regular expression has been modified to get just MemTotal and MemFree
*/

float LinuxParser::MemoryUtilization() 
{ 
  std::string line;
  float value{0};
  // Vector to save total and free memory values
  std::vector<float> memory_values;

  // Pattern to look for memory information inside /proc/meminfo
  std::string pattern{ R"((Mem[^A]\w*:)(\s)*(\d+)(\s*)(kB)?)" };

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
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

// Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  long int uptime{0};
  std::string line;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) 
    std::getline(filestream, line);

  std::istringstream linestream{ line };
  
  std::string tmp;
  linestream >> tmp;
  uptime=std::stod(tmp);  

  return uptime;
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

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{
  std::string line;
  std::string pattern{ R"(cpu\s+((?:\d+\s*)+))" };

  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string regular_expression_result;

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      regular_expression_result = RegularExpression(pattern, line, 1);

      if(regular_expression_result != "")
        break;
    }
  }

  std::istringstream linestream{ regular_expression_result };
  std::vector<std::string> cpu_tokens;
  std::string tmp;
  while (linestream >> tmp)
    cpu_tokens.push_back(tmp);

  return cpu_tokens;

}

// To return the CPU utilization per process
float LinuxParser::CpuUtilization(int pid)
{
  float cpu_consumption{ 0.0 };
  std::string line;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +  kStatFilename);

  if (filestream.is_open()) 
    std::getline(filestream, line);
  
  // Sometimes there are process that dies and then the pseudo file disappears, which causes errors.
  // Thus, if no data is detected, the analysis is by passed
  if(line != "")
  {
      std::istringstream linestream{ line };
      vector<float> cpu_tokens;
      std::string tmp;
      
      int index{0};

      while (linestream >> tmp)
      {
        ++index;
        // Positions 14, 15, 16, 17 and 22 are used to calculate CPU consumption per process
        // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
        if(index == 14 || index == 15 || index == 16 || index == 17)
          cpu_tokens.push_back(stof(tmp));
        else if(index == 22)
        {
          cpu_tokens.push_back(stof(tmp));
          break;
        }
      }

      float total_time{0.0};
      long int hertz{sysconf(_SC_CLK_TCK)};
      float seconds{0.0};
      long uptime{UpTime()};

      for(int i=0; i<4; ++i)
        total_time += cpu_tokens[i];
      
      seconds = uptime - (cpu_tokens[4]/hertz);

      cpu_consumption = total_time / hertz / seconds;
  }

  return cpu_consumption;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses()
{ 
  std::string line;
  // Pattern to look for total number of processes
  std::string pattern{ R"(processes\s{1}(\d+))" };

  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string regular_expression_result;
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      regular_expression_result = RegularExpression(pattern, line, 1);
      if(regular_expression_result != "")
        break;
    }
  }  
  return std::stoi(regular_expression_result);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses()
{ 
  std::string line;
  // Pattern to look for total number of processes
  std::string pattern{ R"(procs_running\s{1}(\d+))" };

  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string regular_expression_result;
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      regular_expression_result = RegularExpression(pattern, line, 1);
      if(regular_expression_result != "")
        break;
    }
  }  
  return std::stoi(regular_expression_result);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string command_line;

  if (filestream.is_open()) 
  {
    std::getline(filestream, command_line);
  }

  return command_line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{
  std::string line;
  std::string pattern{ R"(VmSize:\s{1}(\d+)\s{1}kB)" };

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string ram;

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      ram = RegularExpression(pattern, line, 1);

      if(ram != "")
        break;
    }
  }
  
  long int result{0};
  if(ram!="") // to control cases when the process died in the middle of the process
  {
    result = std::stol(ram);
    result = result / 1024;
  }

  return std::to_string(result);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) 
{
  std::string line;
  std::string pattern{ R"(Uid:\s+(\d+))" };

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string user_id;

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      user_id = RegularExpression(pattern, line, 1);

      if(user_id != "")
        break;
    }
  }

  return user_id;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) 
{
  std::string line;

  std::string uid { Uid(pid) };
  std::string pattern{ "(\\w+[-]*\\w+):x:" + uid };

  std::ifstream filestream(kPasswordPath);
  std::string user_name;

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      user_name = RegularExpression(pattern, line, 1);

      if(user_name != "")
        break;
    }
  }

  return user_name;
}

// Read and return the uptime of a process
// This implementation only works with kernel > 2.6
// Otherwise it will retur zero.
long LinuxParser::UpTime(int pid) 
{
  std::string kernel{ LinuxParser::Kernel() };
  float kernel_version{ std::stof(kernel.substr(0,3)) };
  long int uptime{ 0 };

  if (kernel_version > 2.6)
  {
    std::string line;

    std::ifstream filestream(kProcDirectory + std::to_string(pid) +  kStatFilename);

    if (filestream.is_open()) 
      std::getline(filestream, line);
    
    // Sometimes there are process that dies and then the pseudo file disappears, which causes errors.
    // Thus, if no data is detected, the analysis is by passed
    if(line != "")
    {
        std::istringstream linestream{ line };
        std::string tmp;      
        int index{0};

        while (linestream >> tmp)
        {
          ++index;
          if(index == 22)
            break;        
        }

        long int hertz{ sysconf(_SC_CLK_TCK) };          

        uptime = UpTime() - std::stol(tmp) / hertz;
    }
  }

  return uptime;
}
