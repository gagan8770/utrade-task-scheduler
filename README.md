# Concurrent Task Scheduler (uTrade Mini Project)

## Overview

This project implements a simplified **task execution engine** similar to a build system (like `make`) or a job orchestrator used in data pipelines.

Tasks are defined in a JSON file and contain:

* an ID
* task name
* priority
* dependency list
* simulated execution duration

The scheduler reads the task definitions, constructs a **dependency graph**, checks for **circular dependencies**, and executes tasks in the correct order while respecting **priorities**.

Execution is simulated using `sleep(duration_ms)`.

---

## Features

* JSON-based task configuration
* Dependency graph construction
* Cycle detection using **Kahn's Topological Sort**
* Priority-based task scheduling
* Task execution simulation
* Real-time logging of task start and completion
* Timestamped execution logs

---

## Project Structure

```
utrade-task-scheduler
│
├── main.cpp        # Scheduler implementation
├── tasks.json      # Example input tasks
├── json.hpp        # JSON parsing library
├── README.md       # Project documentation
└── .gitignore      # Ignore build artifacts
```

---

## How to Build

Compile using **C++17**:

```
g++ -std=c++17 main.cpp -o scheduler
```

---

## How to Run

```
./scheduler
```

The program will read tasks from:

```
tasks.json
```

---

## Input Format

Example `tasks.json`:

```
{
  "tasks": [
    {
      "id": 1,
      "name": "Compile A",
      "priority": 2,
      "depends_on": [],
      "duration_ms": 1000
    },
    {
      "id": 2,
      "name": "Compile B",
      "priority": 1,
      "depends_on": [],
      "duration_ms": 1500
    },
    {
      "id": 3,
      "name": "Link",
      "priority": 1,
      "depends_on": [1,2],
      "duration_ms": 2000
    },
    {
      "id": 4,
      "name": "Test",
      "priority": 2,
      "depends_on": [3],
      "duration_ms": 1000
    }
  ]
}
```

---

## Execution Example

```
No cycle detected

START Compile B at Wed Mar 11 16:31:22 2026
END Compile B at Wed Mar 11 16:31:23 2026
START Compile A at Wed Mar 11 16:31:23 2026
END Compile A at Wed Mar 11 16:31:24 2026
START Link at Wed Mar 11 16:31:24 2026
END Link at Wed Mar 11 16:31:26 2026
START Test at Wed Mar 11 16:31:26 2026
END Test at Wed Mar 11 16:31:27 2026

All tasks completed
```

---

## Design Decisions

### Directed Acyclic Graph (DAG)

Tasks are modeled as nodes in a **Directed Acyclic Graph**.
Dependencies represent edges between tasks.

Example:

```
Compile A  \
             →  Link  →  Test
Compile B  /
```

---

### Cycle Detection

We use **Kahn's Algorithm (BFS Topological Sort)** to detect cycles in the dependency graph.

If a cycle exists, execution stops because the dependency graph is invalid.

---

### Priority Scheduling

A **priority queue** is used to determine which task should run next when multiple tasks are ready.

Lower numeric value means **higher priority**:

```
priority = 1 → highest
priority = 2 → lower
```

---

### Task Execution

Tasks are executed by simulating work using:

```
sleep(duration_ms)
```

This allows us to test scheduling logic without real workloads.

---

## Algorithms Used

| Component             | Algorithm               |
| --------------------- | ----------------------- |
| Cycle detection       | Kahn's Topological Sort |
| Scheduling            | Priority Queue          |
| Dependency resolution | Graph traversal         |

---

## Limitations

* Worker thread pool size is currently fixed
* Task retries on failure are not implemented
* Dependency graph visualization is not included
* Task cancellation is not implemented

---

## Future Improvements

Possible enhancements:

* Configurable worker pool (`--workers N`)
* Task retry with failure simulation
* Critical path computation
* Graph visualization using Graphviz
* Task cancellation for downstream failures

---

## Author

Gagan Pahwa
Chitkara University
uTrade Solutions Campus Hiring 2026 – Round 2
