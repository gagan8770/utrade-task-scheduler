#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

struct Task {
    int id;
    string name;
    int priority;
    vector<int> depends_on;
    int duration_ms;
};

struct Compare {
    bool operator()(Task* a, Task* b) {
        return a->priority > b->priority;
    }
};

vector<Task> loadTasks(string filename) {

    ifstream file(filename);
    json data;
    file >> data;

    vector<Task> tasks;

    for(auto &t : data["tasks"]) {

        Task task;
        task.id = t["id"];
        task.name = t["name"];
        task.priority = t["priority"];
        task.duration_ms = t["duration_ms"];

        for(auto &d : t["depends_on"])
            task.depends_on.push_back(d);

        tasks.push_back(task);
    }

    return tasks;
}

bool detectCycle(vector<Task> &tasks) {

    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> indegree;

    for(auto &t : tasks)
        indegree[t.id] = 0;

    for(auto &t : tasks) {
        for(int dep : t.depends_on) {
            graph[dep].push_back(t.id);
            indegree[t.id]++;
        }
    }

    queue<int> q;

    for(auto &p : indegree)
        if(p.second == 0)
            q.push(p.first);

    int visited = 0;

    while(!q.empty()) {

        int node = q.front();
        q.pop();
        visited++;

        for(int next : graph[node]) {

            indegree[next]--;

            if(indegree[next] == 0)
                q.push(next);
        }
    }

    return visited != tasks.size();
}

mutex logMutex;

void executeTask(Task* task) {

    auto start = chrono::system_clock::to_time_t(
        chrono::system_clock::now());

    {
        lock_guard<mutex> lock(logMutex);
        cout << "START " << task->name << " at " << ctime(&start);
    }

    this_thread::sleep_for(
        chrono::milliseconds(task->duration_ms));

    auto end = chrono::system_clock::to_time_t(
        chrono::system_clock::now());

    {
        lock_guard<mutex> lock(logMutex);
        cout << "END " << task->name << " at " << ctime(&end);
    }
}

int main() {

    vector<Task> tasks = loadTasks("tasks.json");

    if(detectCycle(tasks)) {
        cout << "Cycle detected in dependency graph\n";
        return 0;
    }

    cout << "No cycle detected\n";

    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> indegree;

    for(auto &t : tasks)
        indegree[t.id] = 0;

    for(auto &t : tasks) {
        for(int dep : t.depends_on) {
            graph[dep].push_back(t.id);
            indegree[t.id]++;
        }
    }

    priority_queue<Task*, vector<Task*>, Compare> ready;
    unordered_map<int, Task*> taskMap;

    for(auto &t : tasks) {
        taskMap[t.id] = &t;
        if(indegree[t.id] == 0)
            ready.push(&t);
    }

    mutex mtx;

    while(!ready.empty()) {

        Task* t = ready.top();
        ready.pop();

        executeTask(t);

        for(int nxt : graph[t->id]) {

            indegree[nxt]--;

            if(indegree[nxt] == 0)
                ready.push(taskMap[nxt]);
        }
    }

    cout << "\nAll tasks completed\n";

    return 0;
}