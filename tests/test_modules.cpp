// Simple self-checking tests:  make test
#include <iostream>

#include "campus_data.h"
#include "course_planner.h"
#include "exam_timetable.h"
#include "navigation.h"
#include "network_planning.h"
#include "service_requests.h"
#include "student_records.h"

using namespace campus;

static int failed = 0, total = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        ++total;                                                           \
        if (!(cond)) { ++failed; std::cout << "FAIL line " << __LINE__ << ": " #cond "\n"; } \
    } while (0)

int main() {
    CampusData d;
    CHECK(d.loadMap("data/campus_map.txt"));
    CHECK(d.loadCourses("data/courses.txt"));

    // Navigation
    int gate = d.locationId("Main_Gate"), lab = d.locationId("Computer_Lab");
    PathResult p = shortestPath(d, gate, lab);
    CHECK(p.reachable);
    CHECK(p.distance == 150 + 120 + 90 + 70);              // Gate-Admin-BlockA-BlockB-Lab
    CHECK(p.path.front() == gate && p.path.back() == lab);
    CHECK(fewestStops(d, gate, lab).distance >= 1);
    CHECK(bfsOrder(d, gate).size() == d.locations.size());
    CHECK(dfsOrder(d, gate).size() == d.locations.size());

    // Network planning
    MSTResult m = planNetwork(d);
    CHECK(m.connected);
    CHECK(m.chosen.size() == d.locations.size() - 1);
    CHECK(m.totalLength > 0);

    // Course planner
    PlanResult cp = planCourses(d);
    CHECK(cp.valid);
    int total_courses = 0;
    for (auto& s : cp.semesters) total_courses += static_cast<int>(s.size());
    CHECK(total_courses == static_cast<int>(d.courseCodes.size()));
    // a cycle must be detected
    CampusData cyc;
    cyc.courseCodes = {"A", "B"};
    cyc.courseNames = {"A", "B"};
    cyc.unlocks = {{1}, {0}};
    cyc.prereqCount = {1, 1};
    CHECK(!planCourses(cyc).valid);

    // Student records
    StudentTable t(7);   // tiny table on purpose to force collisions
    Student a{"R1", "A", "CSE", 2, 8.0}, b{"R2", "B", "CSE", 2, 8.5};
    CHECK(t.insert(a));
    CHECK(t.insert(b));
    CHECK(!t.insert(a));
    CHECK(t.search("R2") != nullptr);
    CHECK(t.remove("R1"));
    CHECK(t.search("R1") == nullptr);
    CHECK(t.size() == 1);
    StudentTable big;
    CHECK(big.loadFromFile("data/students.txt"));
    CHECK(big.search("2501330100004") != nullptr);

    // Service requests (in progress module)
    RequestHeap h;
    h.submit(3, "Library", "AC not working");
    h.submit(1, "Hostel", "Water leakage");
    h.submit(3, "Lab", "Projector issue");
    Request r;
    CHECK(h.processNext(r) && r.priority == 1);
    CHECK(h.processNext(r) && r.location == "Library");    // FIFO among equal priority
    CHECK(h.processNext(r) && r.location == "Lab");
    CHECK(!h.processNext(r));

    // Exam timetable (in progress module)
    ExamTimetable et;
    CHECK(et.loadConflicts("data/exam_conflicts.txt"));
    Timetable tt = et.schedule();
    CHECK(et.valid(tt));
    CHECK(tt.slots >= 2);

    std::cout << (total - failed) << "/" << total << " checks passed\n";
    return failed ? 1 : 0;
}
