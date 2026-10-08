// exam_timetable.h  --  Module 6: Exam Timetable Scheduling   [IN PROGRESS]
// Concepts: Graph colouring (greedy, Welsh-Powell ordering)
//
// Done   : conflict graph from file, greedy colouring with highest-degree-first order
// TODO   : build conflicts automatically from student enrolment data
// TODO   : room / seat capacity per slot
// TODO   : spread exams over dates (no more than one exam per student per day)
// TODO   : compare greedy result with a backtracking optimum on small inputs
#pragma once
#include <string>
#include <vector>

namespace campus {

struct Timetable {
    std::vector<std::string> subjects;
    std::vector<int> slotOf;      // slotOf[i] = slot number (0-based) of subjects[i]
    int slots = 0;
};

class ExamTimetable {
public:
    bool loadConflicts(const std::string& path);
    Timetable schedule() const;                       // O(V + E) after sorting by degree
    bool valid(const Timetable& t) const;             // no two conflicting subjects share a slot
    size_t subjectCount() const { return names_.size(); }

private:
    std::vector<std::string> names_;
    std::vector<std::vector<int>> adj_;
    int idOf(const std::string& name);
};

}  // namespace campus
