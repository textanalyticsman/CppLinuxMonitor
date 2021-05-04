#include "processor.h"
#include "linux_parser.h"
#include <thread>
#include <chrono>
#include <iostream>

// Return the aggregate CPU utilization
float Processor::Utilization() 
{
  // I have used the algorihm proposed at https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  
  // First reading to get CPU numbers
  Processor::cpu_data.push_back(LinuxParser::CpuUtilization());
  // Sleep during some time before executing second reading. Idea taken from https://stackoverflow.com/questions/52268378/c-usleep-returns-immediately-on-linux
  std::this_thread::sleep_for(std::chrono::milliseconds(Processor::delay_reading));
  // Second reading
  Processor::cpu_data.push_back(LinuxParser::CpuUtilization());
  
  float first_idle{ 0.0 };
  float first_non_idle{ 0.0 };
  float first_total{ 0.0 };

  float second_idle{ 0.0 };
  float second_non_idle{ 0.0 };
  float second_total{ 0.0 };  
  // These are the indexes for idel and wait quantities
  unsigned const int index_idle{3};
  unsigned const int index_wait{4};

  for(long unsigned int i=0; i<cpu_data[0].size(); ++i)
  {
    if(i==index_idle || i==index_wait)
    {
        first_idle += std::stof(cpu_data[0][i]);
        second_idle += std::stof(cpu_data[1][i]);
    }
    else
    {
        first_non_idle += std::stof(cpu_data[0][i]);
        second_non_idle += std::stof(cpu_data[1][i]);
    }
  }        

  cpu_data.clear();

  first_total = first_idle + first_non_idle;
  second_total = second_idle + second_non_idle;

  float total_difference {second_total - first_total};
  float idle_difference {second_idle - first_idle};

  float consumption {(total_difference - idle_difference) / total_difference};

  return consumption; 
}

/*float Processor::Utilization2() 
{
  // I have used the algorihm proposed at https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  
  // First reading to get CPU numbers
  Processor::cpu_data.push_back(LinuxParser::CpuUtilization());
  
  float first_idle{ 0 };
  float first_non_idle{ 0 };
  float first_total{ 0 };

  float second_idle{ 0 };
  float second_non_idle{ 0 };
  float second_total{ 0 };  
  // These are the indexes for idel and wait quantities
  unsigned const int index_idle{3};
  unsigned const int index_wait{4};

  if(cpu_data.size()==2)
  {
    for(long unsigned int i=0; i<cpu_data.size(); ++i)
    {
        if(i==index_idle || i==index_wait)
        {
            first_idle += std::stof(cpu_data[0][i]);
            second_idle += std::stof(cpu_data[1][i]);
        }
        else
        {
            first_non_idle += std::stof(cpu_data[0][i]);
            second_non_idle += std::stof(cpu_data[1][i]);
        }
    }

    cpu_data.clear();

    first_total = first_idle + first_non_idle;
    second_total = second_idle + second_non_idle;

    float total_difference {second_total - first_total};
    float idle_difference {second_idle - first_idle};

    float consumption {(total_difference - idle_difference) / total_difference};      
  }


  return consumption; 
}*/