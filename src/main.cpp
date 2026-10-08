// Integrated Smart Campus Management System
// DSA-II PBL (CCSE0301) - menu driven console application
//
// Usage:  ./smartcampus [data_directory]      (default: data/)

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "campus_data.h"
#include "course_planner.h"
#include "exam_timetable.h"
#include "navigation.h"
#include "network_planning.h"
#include "service_requests.h"
#include "student_records.h"

using namespace campus;

static int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return lo;
        try {
            int v = std::stoi(line);
            if (v >= lo && v <= hi) return v;
        } catch (...) {}
        std::cout << "  Please enter a number between " << lo << " and " << hi << ".\n";
    }
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return trim(s);
}

static void printLocations(const CampusData& d) {
    for (size_t i = 0; i < d.locations.size(); ++i)
        std::cout << "  " << std::setw(2) << i + 1 << ". " << d.locations[i] << "\n";
}

static void printPath(const CampusData& d, const PathResult& r, const char* unit) {
    if (!r.reachable) {
        std::cout << "  No route found.\n";
        return;
    }
    std::cout << "  Route: ";
    for (size_t i = 0; i < r.path.size(); ++i)
        std::cout << d.locations[r.path[i]] << (i + 1 < r.path.size() ? " -> " : "\n");
    std::cout << "  Total: " << r.distance << " " << unit << "\n";
}

// ---------------- Module 1 ----------------
static void navigationMenu(const CampusData& d) {
    std::cout << "\n--- Campus Navigation ---\n";
    printLocations(d);
    int n = static_cast<int>(d.locations.size());
    int a = readInt("From (number): ", 1, n) - 1;
    int b = readInt("To   (number): ", 1, n) - 1;
    int mode = readInt("1) Shortest distance (Dijkstra)  2) Fewest stops (BFS)  3) Explore from source (BFS/DFS): ", 1, 3);
    if (mode == 1) {
        printPath(d, shortestPath(d, a, b), "metres");
    } else if (mode == 2) {
        printPath(d, fewestStops(d, a, b), "stops");
    } else {
        std::cout << "  BFS order: ";
        for (int v : bfsOrder(d, a)) std::cout << d.locations[v] << "  ";
        std::cout << "\n  DFS order: ";
        for (int v : dfsOrder(d, a)) std::cout << d.locations[v] << "  ";
        std::cout << "\n";
    }
}

// ---------------- Module 2 ----------------
static void networkMenu(const CampusData& d) {
    std::cout << "\n--- Infrastructure Network Planning (Kruskal MST) ---\n";
    MSTResult r = planNetwork(d);
    for (const auto& e : r.chosen)
        std::cout << "  " << std::left << std::setw(18) << d.locations[e.u] << " - "
                  << std::setw(18) << d.locations[e.v] << std::right << std::setw(5) << e.weight << " m\n";
    std::cout << "  Minimum total cable length: " << r.totalLength << " m\n";
    if (!r.connected) std::cout << "  Warning: some buildings are not connected to the rest.\n";
}

// ---------------- Module 3 ----------------
static void courseMenu(const CampusData& d) {
    std::cout << "\n--- Course Prerequisite Planner (Topological Sort) ---\n";
    PlanResult r = planCourses(d);
    for (size_t s = 0; s < r.semesters.size(); ++s) {
        std::cout << "  Semester " << s + 1 << ": ";
        for (int c : r.semesters[s]) std::cout << d.courseCodes[c] << "  ";
        std::cout << "\n";
    }
    if (!r.valid) {
        std::cout << "  Circular prerequisites detected! Courses stuck: ";
        for (int c : r.stuck) std::cout << d.courseCodes[c] << " ";
        std::cout << "\n";
    }
}

// ---------------- Module 4 ----------------
static void studentMenu(StudentTable& t) {
    while (true) {
        std::cout << "\n--- Student Records (Hash Table) ---\n"
                  << "  1. Search by roll no.\n  2. Add student\n  3. Delete student\n"
                  << "  4. List all\n  5. Table statistics\n  0. Back\n";
        int ch = readInt("Choice: ", 0, 5);
        if (ch == 0) return;
        if (ch == 1) {
            const Student* s = t.search(readLine("Roll no.: "));
            if (s) std::cout << "  " << s->roll << " | " << s->name << " | " << s->branch << " | Year " << s->year << " | CGPA " << s->cgpa << "\n";
            else std::cout << "  Not found.\n";
        } else if (ch == 2) {
            Student s;
            s.roll = readLine("Roll no.: ");
            s.name = readLine("Name: ");
            s.branch = readLine("Branch: ");
            s.year = readInt("Year (1-4): ", 1, 4);
            s.cgpa = readInt("CGPA x10 (e.g. 85 for 8.5): ", 0, 100) / 10.0;
            std::cout << (t.insert(s) ? "  Added.\n" : "  Roll no. already exists.\n");
        } else if (ch == 3) {
            std::cout << (t.remove(readLine("Roll no.: ")) ? "  Deleted.\n" : "  Not found.\n");
        } else if (ch == 4) {
            t.forEach([](const Student& s) { std::cout << "  " << s.roll << " | " << s.name << " | " << s.branch << " | Year " << s.year << "\n"; });
        } else {
            std::cout << "  Records: " << t.size() << "  Buckets: " << t.bucketCount()
                      << "  Load factor: " << std::fixed << std::setprecision(2) << t.loadFactor()
                      << "  Longest chain: " << t.longestChain() << "\n";
        }
    }
}

// ---------------- Module 5 (in progress) ----------------
static void requestMenu(RequestHeap& h) {
    while (true) {
        std::cout << "\n--- Service Requests (Min-Heap)  [in progress] ---\n"
                  << "  1. Submit request\n  2. View most urgent\n  3. Process most urgent\n  0. Back\n";
        int ch = readInt("Choice: ", 0, 3);
        if (ch == 0) return;
        if (ch == 1) {
            int p = readInt("Priority (1 = emergency ... 5 = low): ", 1, 5);
            std::string loc = readLine("Location: ");
            std::string desc = readLine("Description: ");
            std::cout << "  Request #" << h.submit(p, loc, desc) << " submitted. Pending: " << h.size() << "\n";
        } else {
            Request r;
            bool ok = (ch == 2) ? h.peek(r) : h.processNext(r);
            if (!ok) { std::cout << "  No pending requests.\n"; continue; }
            std::cout << (ch == 2 ? "  Next: " : "  Processed: ") << "#" << r.id << " [P" << r.priority << "] "
                      << r.location << " - " << r.description << "\n";
        }
    }
}

// ---------------- Module 6 (in progress) ----------------
static void timetableMenu(const ExamTimetable& et) {
    std::cout << "\n--- Exam Timetable (Graph Colouring)  [in progress] ---\n";
    Timetable t = et.schedule();
    for (int s = 0; s < t.slots; ++s) {
        std::cout << "  Slot " << s + 1 << ": ";
        for (size_t i = 0; i < t.subjects.size(); ++i)
            if (t.slotOf[i] == s) std::cout << t.subjects[i] << "  ";
        std::cout << "\n";
    }
    std::cout << "  Slots used: " << t.slots << "   Clash free: " << (et.valid(t) ? "yes" : "NO") << "\n";
}

int main(int argc, char** argv) {
    std::string dir = (argc > 1) ? argv[1] : "data";
    if (!dir.empty() && dir.back() != '/') dir += '/';

    CampusData data;
    StudentTable students;
    ExamTimetable exams;
    RequestHeap requests;

    if (!data.loadMap(dir + "campus_map.txt")) std::cerr << "Could not load campus_map.txt\n";
    if (!data.loadCourses(dir + "courses.txt")) std::cerr << "Could not load courses.txt\n";
    if (!students.loadFromFile(dir + "students.txt")) std::cerr << "Could not load students.txt\n";
    if (!exams.loadConflicts(dir + "exam_conflicts.txt")) std::cerr << "Could not load exam_conflicts.txt\n";

    std::cout << "=====================================================\n"
              << "   Integrated Smart Campus Management System\n"
              << "   DSA-II PBL  |  Loaded: " << data.locations.size() << " locations, "
              << data.courseCodes.size() << " courses, " << students.size() << " students\n"
              << "=====================================================\n";

    while (true) {
        std::cout << "\nMAIN MENU\n"
                  << "  1. Campus Navigation\n"
                  << "  2. Infrastructure Network Planning\n"
                  << "  3. Course Prerequisite Planner\n"
                  << "  4. Student Records\n"
                  << "  5. Service Requests          [in progress]\n"
                  << "  6. Exam Timetable            [in progress]\n"
                  << "  0. Exit\n";
        int ch = readInt("Choice: ", 0, 6);
        switch (ch) {
            case 1: navigationMenu(data); break;
            case 2: networkMenu(data); break;
            case 3: courseMenu(data); break;
            case 4: studentMenu(students); break;
            case 5: requestMenu(requests); break;
            case 6: timetableMenu(exams); break;
            default: std::cout << "Goodbye!\n"; return 0;
        }
    }
}
