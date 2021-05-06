#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::string puser, std::string pcommand, float pcpu, std::string pram, long puptime)
        :process_id{ pid }, process_user{ puser }, process_command{ pcommand }, process_cpu_utilization{ pcpu }, process_ram{ pram }, process_up_time{ puptime }
{

}

// Set process ID
void Process::SetPid(int pid){ Process::process_id = pid; }
// Set process user
void Process::SetPuser(std::string puser){ Process::process_user = puser; }
// Set process command
void Process::SetPcommand(std::string pcommand){ Process::process_command = pcommand; }
// Set process CPU consumption
void Process::SetPcpu(float pcpu){ Process::process_cpu_utilization = pcpu; }
// Set process RAM consumption
void Process::SetPram(std::string pram){ Process::process_ram = pram; }
// Set process uptime
void Process::SetPuptime(long puptime){ Process::process_up_time = puptime; }

// Return this process's ID
int Process::Pid() { return Process::process_id; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return Process::process_cpu_utilization; }

// Return the command that generated this process
string Process::Command() { return Process::process_command; }

// Return this process's memory utilization
string Process::Ram() const { return Process::process_ram; }

// Return the user (name) that generated this process
string Process::User() { return Process::process_user; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return Process::process_up_time; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const 
{   
  return std::stol(process_ram) < std::stol(a.Ram()); 
}

// Overload the "greater than" comparison operator for Process objects
bool Process::operator>(Process const& a) const 
{   
  return std::stol(process_ram) > std::stol(a.Ram()); 
}