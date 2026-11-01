#ifndef TRANSPORTSYSTEM_H
#define TRANSPORTSYSTEM_H
#include "CityComponent.h"

class TransportSystem : public CityComponent {
private:
    int trafficFlow;

public:
    TransportSystem(int id, string componentName, int flow);
    void manageTraffic();
    string getStatus() const override;
    void processEvent(string eventType) override;
};

#endif