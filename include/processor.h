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
};

#endif