#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
#include <chrono>
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

vector<Task> loadTasks(string filename) {

    ifstream file(filename);
    json data;
    file >> data;

    vector<Task> tasks;

    for (auto &t : data["tasks"]) {

        Task task;
        task.id = t["id"];
        task.name = t["name"];
        task.priority = t["priority"];
        task.duration_ms = t["duration_ms"];

        for (auto &d : t["depends_on"])
            task.depends_on.push_back(d);

        tasks.push_back(task);
    }

    return tasks;
}

int main() {

    vector<Task> tasks = loadTasks("tasks.json");

    for (auto &t : tasks) {
        cout << "Task: " << t.name << endl;
    }

    return 0;
}