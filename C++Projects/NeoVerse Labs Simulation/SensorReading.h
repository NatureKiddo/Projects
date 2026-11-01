#ifndef SENSORREADING_H
#define SENSORREADING_H
#include <string>
using namespace std;

struct SensorReading {
    int energyUsage;      
    int trafficDensity;   
    int populationCount;
    string timestamp;     
};

#endif