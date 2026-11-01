#ifndef SECURITYSYSTEM_H
#define SECURITYSYSTEM_H
#include "CityComponent.h"

class SecuritySystem : public CityComponent {
private:
    int threatLevel;

public:
    SecuritySystem(int id, string componentName, int threat);
    void monitorCity();
    string getStatus() const override;
    void processEvent(string eventType) override;
};

#endif