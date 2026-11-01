#include "SecuritySystem.h"
#include <iostream>
using namespace std;

SecuritySystem::SecuritySystem(int id, string componentName, int threat)
    : CityComponent(id, componentName), threatLevel(threat) {}

void SecuritySystem::monitorCity() {
    cout << name << " monitoring city, threat level: " << threatLevel << endl;
}

string SecuritySystem::getStatus() const {
    return name + " - Threat Level: " + to_string(threatLevel);
}

void SecuritySystem::processEvent(string eventType) {
    if (eventType == "Network overload") {
        threatLevel += 1;
        cout << name << " handling NETWORK OVERLOAD - isolating affected systems." << endl;
    } else {
        cout << name << " received unrelated event: " << eventType << " (no action taken)." << endl;
    }
}