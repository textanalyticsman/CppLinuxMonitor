#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, std::string puser = "", std::string pcommand = "", float pcpu = 0, std::string pram = "", long puptime = 0);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  // To set values
  void SetPid(int pid);
  void SetPuser(std::string puser);
  void SetPcommand(std::string pcommand);
  void SetPcpu(float pcpu);
  void SetPram(std::string pram);
  void SetPuptime(long puptime);
  // TODO: Declare any necessary private members
 private:
  int process_id;
  std::string process_user;
  std::string process_command;
  float process_cpu_utilization;
  std::string process_ram;
  long process_up_time;
};

#endif