#include "TransportSystem.h"
#include <iostream>
using namespace std;

TransportSystem::TransportSystem(int id, string componentName, int flow)
    : CityComponent(id, componentName), trafficFlow(flow) {}

void TransportSystem::manageTraffic() {
    cout << name << " managing traffic flow: " << trafficFlow << " vehicles/min." << endl;
}

string TransportSystem::getStatus() const {
    return name + " - Traffic Flow: " + to_string(trafficFlow) + " vehicles/min";
}

void TransportSystem::processEvent(string eventType) {
    if (eventType == "Traffic accident") {
        trafficFlow = trafficFlow / 2;
        cout << name << " handling TRAFFIC ACCIDENT - rerouting vehicles, flow reduced." << endl;
    } else {
        cout << name << " received unrelated event: " << eventType << " (no action taken)." << endl;
    }
}