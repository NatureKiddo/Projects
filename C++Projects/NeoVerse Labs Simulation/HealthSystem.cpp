#include "HealthSystem.h"
#include <iostream>
using namespace std;

HealthSystem::HealthSystem(int id, string componentName, int count)
    : CityComponent(id, componentName), hospitalCnt(count) {}

void HealthSystem::provideCare() {
    cout << name << " providing care across " << hospitalCnt << " hospitals." << endl;
}

string HealthSystem::getStatus() const {
    return name + " - Active Hospitals: " + to_string(hospitalCnt);
}

void HealthSystem::processEvent(string eventType) {
    if (eventType == "Weather alert") {
        cout << name << " handling WEATHER ALERT - preparing hospitals for incoming casualties." << endl;
    } else {
        cout << name << " received unrelated event: " << eventType << " (no action taken)." << endl;
    }
}