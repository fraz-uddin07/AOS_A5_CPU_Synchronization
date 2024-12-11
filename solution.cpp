#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <unistd.h>
#include <algorithm>

using namespace std;

#define WHITE "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"

struct StudentInfo {
    int index;
    int arrivalTime;
    int washingTime;
    int patience;
    bool hasWashed;
    bool hasLeft;
    
    StudentInfo(int idx, int arrival, int washing, int wait) 
        : index(idx), arrivalTime(arrival), washingTime(washing), 
          patience(wait), hasWashed(false), hasLeft(false) {}

    bool operator<(const StudentInfo& other) const {
        if (arrivalTime != other.arrivalTime) {
            return arrivalTime < other.arrivalTime;
        }
        return index < other.index;
    }
};

class WashingMachineMonitor {
private:
    int totalStudents;
    int totalMachines;
    vector<StudentInfo> students;
    vector<int> machineEndTime;
    mutex globalMutex;
    mutex printMutex;
    
    int studentsLeftWithoutWashing;
    int simulationTime;

    void printMessage(const string& color, const string& message) {
        lock_guard<mutex> lock(printMutex);
        cout << color << message << WHITE << endl;
    }

    bool tryGetMachine(int studentIndex, int currentTime, int washTime) {
        lock_guard<mutex> lock(globalMutex);
        
        // Find machine with earliest end time
        int earliestMachine = 0;
        for(int i = 1; i < totalMachines; i++) {
            if(machineEndTime[i] < machineEndTime[earliestMachine]) {
                earliestMachine = i;
            }
        }
        
        // If machine is available now
        if(machineEndTime[earliestMachine] <= currentTime) {
            machineEndTime[earliestMachine] = currentTime + washTime;
            return true;
        }
        
        return false;
    }

    void sortStudents() {
        sort(students.begin(), students.end());
    }

public:
    WashingMachineMonitor(int students, int machines) 
        : totalStudents(students), totalMachines(machines), 
          studentsLeftWithoutWashing(0), simulationTime(0) {
        machineEndTime.resize(machines, 0);
    }

    void addStudent(int index, int arrival, int washing, int patience) {
        students.emplace_back(index + 1, arrival, washing, patience);
    }

    void startSimulation() {
        sortStudents();
        vector<thread> threads;

        for (int i = 0; i < totalStudents; i++) {
            threads.emplace_back(&WashingMachineMonitor::studentThread, 
                               this, ref(students[i]));
        }

        for (auto& thread : threads) {
            thread.join();
        }

        printResults();
    }

private:
    void studentThread(StudentInfo& student) {
        // Wait until arrival time
        this_thread::sleep_for(
            chrono::seconds(student.arrivalTime));

        // Student arrives
        printMessage(WHITE, "Student " + 
                    to_string(student.index) + " arrives");

        bool gotMachine = false;
        int waitTime = 0;
        int currentTime = student.arrivalTime;

        // Try to get machine until patience runs out
        while(waitTime <= student.patience) {
            if(tryGetMachine(student.index, currentTime, student.washingTime)) {
                gotMachine = true;
                break;
            }
            this_thread::sleep_for(chrono::seconds(1));
            waitTime++;
            currentTime++;
        }

        if(gotMachine) {
            // Start washing
            printMessage(GREEN, "Student " + 
                       to_string(student.index) + 
                       " starts washing");

            // Simulate washing time
            this_thread::sleep_for(
                chrono::seconds(student.washingTime));

            // Finish washing
            student.hasWashed = true;
            printMessage(YELLOW, "Student " + 
                       to_string(student.index) + 
                       " leaves after washing");
        } else {
            // Leave without washing
            lock_guard<mutex> lock(globalMutex);
            studentsLeftWithoutWashing++;
            printMessage(RED, "Student " + 
                       to_string(student.index) + 
                       " leaves without washing");
        }

        student.hasLeft = true;
    }

    void printResults() {
        cout << studentsLeftWithoutWashing << endl;
        float leftPercentage = (float)studentsLeftWithoutWashing / 
                             totalStudents * 100;
        cout << WHITE << (leftPercentage >= 25 ? "Yes" : "No") 
             << endl;
    }
};

int main() {
    int N, M;
    cin >> N >> M;

    WashingMachineMonitor monitor(N, M);

    for (int i = 0; i < N; i++) {
        int arrival, washing, patience;
        cin >> arrival >> washing >> patience;
        monitor.addStudent(i, arrival, washing, patience);
    }

    monitor.startSimulation();

    return 0;
}