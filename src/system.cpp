#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <functional>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() 
{ 
    processes_.clear();
    vector<int> list_pids = LinuxParser::Pids();
    
    for(int pid : list_pids)
    {
        Process process(pid, LinuxParser::User(pid), LinuxParser::Command(pid), 
          LinuxParser::CpuUtilization(pid), LinuxParser::Ram(pid), LinuxParser::UpTime(pid));
        processes_.push_back(process);
    }
    
    // I have taken ideas from below link to sort the list of Process objects
    // https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects
    std::sort(processes_.begin(), processes_.end(), std::greater<Process>());

    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }