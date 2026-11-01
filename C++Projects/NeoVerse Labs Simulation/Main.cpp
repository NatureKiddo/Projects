#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <list>
#include <cstdlib>  
#include <ctime>   
#include <fstream> 
#include "Engineer.h"
#include "CityComponent.h"
#include "PowerSystem.h"
#include "TransportSystem.h"
#include "HealthSystem.h"
#include "SecuritySystem.h"
#include "SensorReading.h"
using namespace std;

//Linear search login
Engineer* loginLinear(vector<Engineer> & engineers, string username, string password) {
    for (int i = 0; i < engineers.size(); i++) {
        if (engineers[i].getUsername() == username && engineers[i].checkPassword(password)) {
            return &engineers[i]; //return pointer to the matched engineer
        }
    }
    return nullptr; //no matches found
}

//Binary search login
Engineer* loginBinary(vector<Engineer>& engineers, string username, string password) {
    vector<Engineer> sortedCopy = engineers;  //copy for sorting
    sort(sortedCopy.begin(), sortedCopy.end(), [](const Engineer& a, const Engineer& b) {
        return a.getUsername() < b.getUsername();
    });

    int low = 0, high = sortedCopy.size() - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (sortedCopy[mid].getUsername() == username) {
            if (!sortedCopy[mid].checkPassword(password)) {
                return nullptr; //username found but wrong password
            }
            //Finding valid match
            for (int i = 0; i < engineers.size(); i++) {
                if (engineers[i].getUsername() == username) {
                    return &engineers[i]; 
                }
            }
        } else if (sortedCopy[mid].getUsername() < username) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return nullptr;
}

void broadcastEvent(vector<CityComponent*>& components, string eventType) {
    for (CityComponent* comp : components) {
        comp->processEvent(eventType);
    }
}
//It checks the Clearance Level and gives opens only what they allowed to see
bool hasPermission(const Engineer* engineer, string action) {
    string clearance = engineer->getClearance();
    if (action == "processEvent") {
        return (clearance == "Medium" || clearance == "High");
    }
    if (action == "resolveEmergency") {
        return (clearance == "High");
    }
    return false;
}

//Save all engineers to a file
void saveEngineers(const vector<Engineer>& engineers, string filename) {
    ofstream outFile(filename);
    for (const Engineer& e : engineers) {
        outFile << e.getID() << "|" << e.getUsername() << "|"
                << e.getPasswordHash() << "|" << e.getClearance() << endl;
    }
    outFile.close();
    cout << "Saved " << engineers.size() << " engineers to " << filename << endl;
}

//Load engineers from a file (if it exists)
bool loadEngineers(vector<Engineer>& engineers, string filename) {
    ifstream inFile(filename);
    if (!inFile) {
        return false; // file doesn't exist yet
    }

    engineers.clear(); //remove default data before loading real saved data
    string line;
    while (getline(inFile, line)) {
        //parse the pipe-delimited line
        size_t pos1 = line.find("|");
        size_t pos2 = line.find("|", pos1 + 1);
        size_t pos3 = line.find("|", pos2 + 1);

        string id = line.substr(0, pos1);
        string uname = line.substr(pos1 + 1, pos2 - pos1 - 1);
        size_t hashVal = stoull(line.substr(pos2 + 1, pos3 - pos2 - 1));
        string clearance = line.substr(pos3 + 1);

        engineers.push_back(Engineer(id, uname, hashVal, clearance));
    }
    inFile.close();
    return true;
}
// --- Events log (events.dat) ---
void saveEventLog(const vector<pair<string,int>>& log, string filename) {
    ofstream outFile(filename);
    for (const auto& entry : log) {
        outFile << entry.first << "|" << entry.second << endl;
    }
    outFile.close();
    cout << "Saved " << log.size() << " events to " << filename << endl;
}

// --- City logs (city_logs.dat) ---
void saveCityLogs(const list<string>& logs, string filename) {
    ofstream outFile(filename);
    for (const string& entry : logs) {
        outFile << entry << endl;
    }
    outFile.close();
    cout << "Saved " << logs.size() << " city log entries to " << filename << endl;
}

// --- Config (config.txt) ---
int loadCriticalThreshold(string filename, int defaultValue) {
    ifstream inFile(filename);
    if (!inFile) {
        //No config exists yet
        ofstream outFile(filename);
        outFile << "criticalThreshold=" << defaultValue << endl;
        outFile.close();
        cout << "No config.txt found - created one with default threshold " << defaultValue << endl;
        return defaultValue;
    }

    string line;
    while (getline(inFile, line)) {
        size_t eqPos = line.find("=");
        if (eqPos != string::npos) {
            string key = line.substr(0, eqPos);
            string value = line.substr(eqPos + 1);
            if (key == "criticalThreshold") {
                return stoi(value);
            }
        }
    }
    return defaultValue; //fallback if key wasn't found
}

// --- CSV export ---
void exportEventLogToCSV(const vector<pair<string,int>>& log, string filename) {
    ofstream outFile(filename);
    outFile << "EventType,ResponseTimeMinutes" << endl; //header row
    for (const auto& entry : log) {
        outFile << entry.first << "," << entry.second << endl;
    }
    outFile.close();
    cout << "Exported event log to " << filename << " (CSV)" << endl;
}

//stores (eventType, simulatedResponseTime)
vector<pair<string, int>> eventLog;  

int main() {
    srand(time(0));
    vector<Engineer> engineers;

    //Sample data
    engineers.push_back(Engineer("ENG001", "Jane", "pass123", "High"));
    engineers.push_back(Engineer("ENG002", "John", "word456", "Medium" ));
    engineers.push_back(Engineer("ENG003", "Sam", "test789", "Low"));

    //Try loading existing saved engineers
    if (!loadEngineers(engineers, "engineers.dat")) {
        cout << "No existing engineers.dat found - creating one with sample data." << endl;
        saveEngineers(engineers, "engineers.dat");
    } else {
        cout << "Loaded engineers from engineers.dat." << endl;
    }

    string inputUser, inputPass;
    cout << "Username: ";
    cin >> inputUser;
    cout << "Password: ";
    cin >> inputPass;

    Engineer* result = loginBinary(engineers, inputUser, inputPass);

    if(result != nullptr) {
        cout << "Login successful. Welcome " << result->getID() <<
        " \nClearance: " << result->getClearance() << endl;
    } else {
        cout << "Login Failed: Invalid Credentials." << endl;
        return 0;
    }

    //test the CityComponent hierarchy and polymorphism
    vector<CityComponent*> cityComponents;
    cityComponents.push_back(new PowerSystem(1, "Main Power Grid", 85.0));
    cityComponents.push_back(new TransportSystem(2, "City Transport", 200));
    cityComponents.push_back(new HealthSystem(3, "City Health Network", 5));
    cityComponents.push_back(new SecuritySystem(4, "City Security", 3));

    cout << "\n--- Activating City Components ---" << endl;
    for (CityComponent* comp : cityComponents) {
        comp->activate();
    }

    cout << "\n--- Testing City Components ---" << endl;
    for (CityComponent* comp : cityComponents) {
        cout << comp->getStatus() << endl;
    }

    cout << "\n--- Simulating Events ---" << endl;
    cityComponents[0]->processEvent("Power failure");
    cityComponents[1]->processEvent("Traffic accident");
    cityComponents[2]->processEvent("Weather alert");
    cityComponents[3]->processEvent("Network overload");

    // --- Event Queue (FIFO) ---
    queue<string> eventQueue;
    eventQueue.push("Traffic accident");
    eventQueue.push("Power failure");
    eventQueue.push("Weather alert");

    cout << "\n--- Processing Event Queue (FIFO order) ---" << endl;
    while (!eventQueue.empty()) {
        string currentEvent = eventQueue.front();

        if (hasPermission(result, "processEvent")) {
            cout << "Processing: " << currentEvent << endl;
            broadcastEvent(cityComponents, currentEvent);
            int responseTime = rand() % 10 + 1;
            eventLog.push_back({currentEvent, responseTime});
        } else {
            cout << "Access denied: " << result->getID() << " lacks clearance to process events." << endl;
        }

        eventQueue.pop();
    }

        // --- Emergency Override Stack (LIFO) ---
    stack<string> emergencyStack;
    emergencyStack.push("Network overload");
    emergencyStack.push("Power failure");

    cout << "\n--- Resolving Emergency Stack (LIFO order) ---" << endl;
    while (!emergencyStack.empty()) {
        string currentEmergency = emergencyStack.top();

        if (hasPermission(result, "resolveEmergency")) {
            cout << "Resolving: " << currentEmergency << endl;
            broadcastEvent(cityComponents, currentEmergency);
            int responseTime = rand() % 10 + 1;
            eventLog.push_back({currentEmergency, responseTime});
        } else {
            cout << "Access denied: " << result->getID() << " lacks clearance to resolve emergencies." << endl;
        }

        emergencyStack.pop();
    }

        // --- City Data Storage ---
    vector<SensorReading> sensorData;  
    list<string> cityLogs;             

    //Insert new sensor readings
    sensorData.push_back({120, 300, 50000, "Day 1"});
    sensorData.push_back({135, 280, 50120, "Day 2"});
    sensorData.push_back({98, 310, 50200, "Day 3"});

    //Insert corresponding log entries
    cityLogs.push_back("Day 1: Energy=120MW, Traffic=300, Pop=50000");
    cityLogs.push_back("Day 2: Energy=135MW, Traffic=280, Pop=50120");
    cityLogs.push_back("Day 3: Energy=98MW, Traffic=310, Pop=50200");

    //Traverse and display sensor data (vector)
    cout << "\n--- Sensor Data (vector) ---" << endl;
    for (int i = 0; i < sensorData.size(); i++) {
        cout << sensorData[i].timestamp << ": Energy=" << sensorData[i].energyUsage
            << "MW, Traffic=" << sensorData[i].trafficDensity
            << ", Pop=" << sensorData[i].populationCount << endl;
    }

    //Traverse and display city logs (list)
    cout << "\n--- City Logs (list) ---" << endl;
    for (const string& entry : cityLogs) {
        cout << entry << endl;
    }

    //find() - plain STL find, exact string match in the list
    auto logIt = find(cityLogs.begin(), cityLogs.end(), cityLogs.front());
    if (logIt != cityLogs.end()) {
        cout << "\nPlain find() located: " << *logIt << endl;
    }

    //Remove outdated data
    sensorData.erase(sensorData.begin());
    cout << "\n--- Sensor Data after removing outdated (Day 1) ---" << endl;
    for (int i = 0; i < sensorData.size(); i++) {
        cout << sensorData[i].timestamp << ": Energy=" << sensorData[i].energyUsage << "MW" << endl;
    }
    //Remove outdated data from the list too
    cityLogs.pop_front();
    cout << "\n--- City Logs after removing outdated (Day 1) ---" << endl;
    for (const string& entry : cityLogs) {
        cout << entry << endl;
    }

    cout << "\n--- System Reports ---" << endl;

        // --- File I/O: persist this run's data ---
    saveEventLog(eventLog, "events.dat");
    saveCityLogs(cityLogs, "city_logs.dat");
    exportEventLogToCSV(eventLog, "event_report.csv");   
    
    //Total events processed
    cout << "Total events processed: " << eventLog.size() << endl;

    //Search for a specific event type in the log
    string searchTarget = "Power failure";
    auto foundEvent = find_if(eventLog.begin(), eventLog.end(), [searchTarget](const pair<string,int>& e) {
        return e.first == searchTarget;
    });
    if (foundEvent != eventLog.end()) {
        cout << "Search: found \"" << searchTarget << "\" with response time " << foundEvent->second << " minutes." << endl;
    } else {
        cout << "Search: \"" << searchTarget << "\" not found in event log." << endl;
    }

    //Filter/prioritize
    int criticalEventCount = count_if(eventLog.begin(), eventLog.end(), [](const pair<string,int>& e) {
        return (e.first == "Power failure" || e.first == "Network overload");
    });
    int nonCriticalEventCount = eventLog.size() - criticalEventCount;

    cout << "Critical events handled (Power failure/Network overload): " << criticalEventCount << endl;
    cout << "Non-critical events handled: " << nonCriticalEventCount << endl;

        //Most common emergency type
    if (eventLog.empty()) {
        cout << "Most common emergency type: N/A (no events processed)" << endl;
    } else {
        string mostCommonType;
        int highestCount = 0;
        for (const auto& entry : eventLog) {
            string typeToCheck = entry.first;
            int occurrences = count_if(eventLog.begin(), eventLog.end(), [typeToCheck](const pair<string,int>& e) {
                return e.first == typeToCheck;
            });
            if (occurrences > highestCount) {
                highestCount = occurrences;
                mostCommonType = typeToCheck;
            }
        }
        cout << "Most common emergency type: " << mostCommonType << " (" << highestCount << " occurrences)" << endl;
    }

    //Average response time
    if (eventLog.empty()) {
        cout << "Average response time: N/A (no events processed)" << endl;
    } else {
        int totalResponseTime = 0;
        for (const auto& entry : eventLog) {
            totalResponseTime += entry.second;
        }
        double avgResponseTime = (double)totalResponseTime / eventLog.size();
        cout << "Average response time: " << avgResponseTime << " minutes" << endl;
    }

    //System load summary
    cout << "\nSystem Load Summary:" << endl;
    for (CityComponent* comp : cityComponents) {
        cout << " - " << comp->getStatus() << endl;
    }

    //Cleanup
    for (CityComponent* comp : cityComponents) {
        delete comp;
    }

    cout << "\n--- STL Algorithms on Sensor Data ---" << endl;

    sort(sensorData.begin(), sensorData.end(), [](const SensorReading& a, const SensorReading& b) {
        return a.energyUsage < b.energyUsage;
    });
    cout << "Sorted by energy usage:" << endl;
    for (const SensorReading& r : sensorData) {
        cout << r.timestamp << ": " << r.energyUsage << "MW" << endl;
    }

    auto it = find_if(sensorData.begin(), sensorData.end(), [](const SensorReading& r) {
        return r.timestamp == "Day 3";
    });
    if (it != sensorData.end()) {
        cout << "\nFound Day 3 reading: Energy=" << it->energyUsage << "MW" << endl;
    } else {
        cout << "\nDay 3 reading not found." << endl;
    }

    auto minReading = min_element(sensorData.begin(), sensorData.end(), [](const SensorReading& a, const SensorReading& b) {
        return a.energyUsage < b.energyUsage;
    });
    auto maxReading = max_element(sensorData.begin(), sensorData.end(), [](const SensorReading& a, const SensorReading& b) {
        return a.energyUsage < b.energyUsage;
    });
    cout << "\nLowest energy usage: " << minReading->energyUsage << "MW (" << minReading->timestamp << ")" << endl;
    cout << "Highest energy usage: " << maxReading->energyUsage << "MW (" << maxReading->timestamp << ")" << endl;

    int criticalThreshold = loadCriticalThreshold("config.txt", 100);
    int criticalCount = count_if(sensorData.begin(), sensorData.end(), [criticalThreshold](const SensorReading& r) {
        return r.energyUsage > criticalThreshold;
    });
    cout << "\nCritical readings (energy > " << criticalThreshold << "MW): " << criticalCount << endl;


    return 0;
}