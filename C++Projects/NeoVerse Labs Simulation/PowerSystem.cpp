#include "PowerSystem.h"
#include <iostream>
using namespace std;

PowerSystem::PowerSystem(int id, string componentName, double level)
    : CityComponent(id, componentName), powerLevel(level) {}

void PowerSystem::supplyPower() {
    cout << name << " is supplying power at level " << powerLevel << endl;
}

string PowerSystem::getStatus() const {
    return name + " - Power Level: " + to_string(powerLevel) + "%";
}

void PowerSystem::processEvent(string eventType) {
    if (eventType == "Power failure") {
        powerLevel = 0;
        cout << name << " handling POWER FAILURE - shutting down grid, switching to backup." << endl;
    } else {
        cout << name << " received unrelated event: " << eventType << " (no action taken)." << endl;
    }
}
