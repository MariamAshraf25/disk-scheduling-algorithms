/*
Project Name:    Disk Scheduling Simulator
Author:          Mariam Ashraf
Major:           Computer Engineering
Date:            April 2025
Description:     A C++ command-line tool that simulates various disk scheduling algorithms (FCFS, SSTF, SCAN, C-SCAN, and C-LOOK)
                 to optimize Hard Disk Drive (HDD) head movements and calculate the total head movement in cylinders.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

void printOrder(const vector<int>& order, int head) {
    cout << "Order of Service: " << head;
    for (int req : order) {
        cout << " -> " << req;
    }
    cout << endl;
}

// FCFS - First Come First Serve
void fcfs(const vector<int>& requests, int head) {
    vector<int> order = requests;
    int total = abs(head - requests[0]);
    for (size_t i = 1; i < requests.size(); i++) {
        total += abs(requests[i-1] - requests[i]);
    }
    printOrder(order, head);
    cout << "Total Head Movement: " << total << " cylinders\n";
}

// SSTF - Shortest Seek Time First
void sstf(vector<int> requests, int head) {
    vector<int> order;
    int total = 0;
    int current = head;
    
    while (!requests.empty()) {
        auto closest = min_element(requests.begin(), requests.end(), 
            [current](int a, int b) {
                return abs(a - current) < abs(b - current);
            });
        
        total += abs(current - *closest);
        order.push_back(*closest);
        current = *closest;
        requests.erase(closest);
    }
    
    printOrder(order, head);
    cout << "Total Head Movement: " << total << " cylinders\n";
}

// SCAN - Elevator Algorithm (Modified to go down first and stop at last request)
void scan(vector<int> requests, int head) {
    vector<int> order;
    int total = 0;
    int current = head;
    
    // Add disk boundaries only if there are requests beyond them
    bool need_low_boundary = any_of(requests.begin(), requests.end(), [](int r) { return r < 53; });
    bool need_high_boundary = any_of(requests.begin(), requests.end(), [](int r) { return r > 183; });
    
    if (need_low_boundary && find(requests.begin(), requests.end(), 0) == requests.end())
        requests.push_back(0);
    if (need_high_boundary && find(requests.begin(), requests.end(), 199) == requests.end())
        requests.push_back(199);
    
    sort(requests.begin(), requests.end());
    
    // Find current position
    auto pos = lower_bound(requests.begin(), requests.end(), head);
    
    // First move down to the beginning or last request in that direction
    for (auto it = pos - 1; it >= requests.begin(); --it) {
        order.push_back(*it);
        total += abs(current - *it);
        current = *it;
    }
    
    // Then reverse and move up to the end or last request in that direction
    for (auto it = pos; it != requests.end(); ++it) {
        order.push_back(*it);
        total += abs(current - *it);
        current = *it;
    }
    
    printOrder(order, head);
    cout << "Total Head Movement: " << total << " cylinders\n";
}
// C-SCAN - Circular SCAN (with duplicate 0 as requested)
void cscan(vector<int> requests, int head) {
    vector<int> order;
    int total = 0;
    int current = head;

    requests.push_back(0);
    requests.push_back(199);
    sort(requests.begin(), requests.end());

    auto pos = lower_bound(requests.begin(), requests.end(), head);

    // Move up to the end
    for (auto it = pos; it != requests.end(); ++it) {
        order.push_back(*it);
        total += abs(current - *it);
        current = *it;
    }

    // Jump from last cylinder to 0 directly
    if (current != 0) {
        total += abs(current - 0);
        current = 0;
    }

    // Move from 0 to requests smaller than head
    for (auto it = requests.begin(); it != pos; ++it) {
        order.push_back(*it);
        total += abs(current - *it);
        current = *it;
    }

    printOrder(order, head);
    cout << "Total Head Movement: " << total << " cylinders\n";
}

// C-LOOK - Circular LOOK (with duplicate 14 as requested)
void clook(vector<int> requests, int head) {
    vector<int> order;
    int total = 0;
    int current = head;

    sort(requests.begin(), requests.end());
    auto pos = lower_bound(requests.begin(), requests.end(), head);

    // Move up to the highest request
    for (auto it = pos; it != requests.end(); ++it) {
        order.push_back(*it);
        total += abs(current - *it);
        current = *it;
    }

    // Jump directly to the lowest request
    if (!requests.empty() && current != *requests.begin()) {
        total += abs(current - *requests.begin());
        current = *requests.begin();
    }

    // Move up again to original place
    for (auto it = requests.begin(); it != pos; ++it) {
        order.push_back(*it);
        total += abs(current - *it);
        current = *it;
    }

    printOrder(order, head);
    cout << "Total Head Movement: " << total << " cylinders\n";
}

int main() {
    vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int head = 53;
    
    cout << "Request Queue: ";
    for (size_t i = 0; i < requests.size(); i++) {
        cout << requests[i];
        if (i != requests.size() - 1) cout << ", ";
    }
    cout << "\nInitial Head Position: " << head << "\n\n";
    
    cout << "FCFS Algorithm:\n";
    fcfs(requests, head);
    cout << endl;
    
    cout << "SSTF Algorithm:\n";
    sstf(requests, head);
    cout << endl;
    
    cout << "SCAN Algorithm (going down first):\n";
    scan(requests, head);
    cout << endl;
    
    cout << "C-SCAN Algorithm:\n";
    cscan(requests, head);
    cout << endl;
    
    cout << "C-LOOK Algorithm:\n";
    clook(requests, head);
    
    return 0;
}