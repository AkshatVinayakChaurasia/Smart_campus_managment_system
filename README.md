# Integrated Smart Campus Management System

A C++ project built for the **Data Structures and Algorithms II (CCSE0301)** PBL at NIET Greater Noida.
It shows how data structures and graph algorithms can power an integrated campus platform,
instead of running isolated systems for academics, administration, facilities and student services.

- **Problem statement:** Design an integrated smart campus platform that manages institutional data efficiently, optimises operations and improves the student experience through intelligent decision-making.
- **SDG:** 4 – Quality Education
- **Team:** Solo (Akshat Chaurasia)
- **Status:** 🚧 work in progress – about **55 %** complete (Review 2)

## Architecture

All modules read from one shared data layer, so the same campus data is never duplicated across systems.

```
                    +---------------------------+
                    |   main.cpp (menu driven)  |
                    +-------------+-------------+
                                  |
   +----------+----------+--------+--------+-----------+-----------+
   |          |          |                 |           |           |
+--v---+  +---v---+  +---v----+      +-----v----+  +---v----+  +---v-----+
|Naviga|  |Network|  | Course |      | Student  |  |Service |  |  Exam   |
| tion |  |Plannin|  | Planner|      | Records  |  |Requests|  |Timetable|
+--+---+  +---+---+  +---+----+      +-----+----+  +---+----+  +----+----+
   |          |          |                 |           |            |
   +----------+----------+--------+--------+-----------+------------+
                                  |
                    +-------------v-------------+
                    |  CampusData (shared layer)|
                    |  data/*.txt input files   |
                    +---------------------------+
```

## Modules and DSA concepts

| # | Module | Data structure / algorithm | Time complexity | Status |
|---|--------|----------------------------|-----------------|--------|
| 1 | Campus Navigation | Graph (adjacency list), Dijkstra + min-heap, BFS, DFS | O((V+E) log V) / O(V+E) | ✅ Done |
| 2 | Infrastructure Network Planning | Kruskal's MST + Disjoint Set Union | O(E log E) | ✅ Done |
| 3 | Course Prerequisite Planner | DAG, Topological Sort (Kahn), cycle detection | O(V+E) | ✅ Done |
| 4 | Student Records | Hash table with separate chaining | O(1) average | ✅ Done |
| 5 | Service Requests | Binary min-heap / priority queue | O(log n) | 🚧 In progress |
| 6 | Exam Timetable | Greedy graph colouring (Welsh–Powell order) | O(V+E) | 🚧 In progress |

## Project structure

```
Smart_campus_managment_system/
├── include/          header files for every module
├── src/              implementations + main.cpp
├── data/             sample input data (map, courses, students, exam conflicts)
├── tests/            self-checking tests (make test)
├── docs/screenshots/ output screenshots
├── Makefile
└── README.md
```

## Build and run

Requires `g++` with C++17 support.

```bash
make            # builds ./smartcampus
./smartcampus   # run the menu driven app (reads the data/ folder)
make test       # runs the test suite
make clean
```

On Windows without `make`:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o smartcampus
smartcampus.exe
```

## Input file formats

| File | Format |
|------|--------|
| `data/campus_map.txt` | `LocationA,LocationB,distance_in_metres` |
| `data/courses.txt` | `CODE\|Course Name\|prereq1,prereq2` |
| `data/students.txt` | `roll\|name\|branch\|year\|cgpa` (sample data only) |
| `data/exam_conflicts.txt` | `SUBJECT1 SUBJECT2` (they share students) |

## What is left to do

- [ ] Service Requests: change priority of a pending request, auto-escalation, assign nearest staff using the Navigation module, save/load requests
- [ ] Exam Timetable: build conflicts from enrolment data, room capacity, spread exams over dates, compare with optimal colouring
- [ ] Trie-based prefix search for library books / student names
- [ ] Integration of all modules with one shared persistent data layer
- [ ] Performance comparison of the chosen data structures against simpler alternatives on larger data
- [ ] Final report and demonstration

## References

- T. H. Cormen et al., *Introduction to Algorithms* (CLRS)
- E. Horowitz and S. Sahni, *Fundamentals of Data Structures*
- Smart Campus Framework: Definition, Model, Measurement – ITB Journal
- Construction of Smart Campus System Based on Cloud Computing – Atlantis Press
- Smart campus management system based on the Internet of Things – IOS Press
