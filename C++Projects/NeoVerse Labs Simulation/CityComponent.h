#ifndef CITYCOMPONENT_H
#define CITYCOMPONENT_H
#include <string>
using namespace std;

class CityComponent {
protected:
    int componentID;
    string name;

public:
    //Constructor
    CityComponent(int id, string componentName);

    //Virtual destructor
    virtual ~CityComponent();

    //Shared interface
    virtual void activate();
    virtual void deactivate();
    virtual string getStatus() const;

    //Pure virtual
    virtual void processEvent(string eventType) = 0;
};

#endif