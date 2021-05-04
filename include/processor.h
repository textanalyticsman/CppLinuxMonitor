#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

 private:
  //This will save the CPU average numbers for two readings
  std::vector<std::vector<std::string>> cpu_data;
  // This is the delay between readings in seconds used to calculate CPU consumption
  unsigned int delay_reading{1000};
};

#endif