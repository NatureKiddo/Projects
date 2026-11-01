#include "CityComponent.h"
#include <iostream>
using namespace std;

CityComponent::CityComponent(int id, string componentName)
    : componentID(id), name(componentName) {}

CityComponent::~CityComponent() {
    cout << name << " destroyed, resource released." << endl;
}

void CityComponent::activate() {
    cout << name << " activated." << endl;
}

void CityComponent::deactivate() {
    cout << name << " deactivated." << endl;
}

string CityComponent::getStatus() const {
    return name + " is operational.";
}
