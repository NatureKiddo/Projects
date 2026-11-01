#ifndef HEALTHSYSTEM_H
#define HEALTHSYSTEM_H
#include "CityComponent.h"

class HealthSystem : public CityComponent {
private:
    int hospitalCnt;

public:
    HealthSystem(int id, string componentName, int count);
    void provideCare();
    string getStatus() const override;
    void processEvent(string eventType) override;
};

#endif