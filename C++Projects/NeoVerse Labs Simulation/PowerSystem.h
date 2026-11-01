#ifndef POWERSYSTEM_H
#define POWERSYSTEM_H
#include "CityComponent.h"

class PowerSystem : public CityComponent {
private:
    double powerLevel;

public:
    PowerSystem(int id, string componentName, double level);
    void supplyPower();
    string getStatus() const override;     
    void processEvent(string eventType) override;  
};

#endif