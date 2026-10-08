#include "exam_timetable.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "campus_data.h"

namespace campus {

int ExamTimetable::idOf(const std::string& name) {
    for (size_t i = 0; i < names_.size(); ++i)
        if (names_[i] == name) return static_cast<int>(i);
    names_.push_back(name);
    adj_.emplace_back();
    return static_cast<int>(names_.size()) - 1;
}

bool ExamTimetable::loadConflicts(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string a, b;
        if (!(ss >> a >> b)) continue;
        int u = idOf(a), v = idOf(b);
        adj_[u].push_back(v);
        adj_[v].push_back(u);
    }
    return !names_.empty();
}

Timetable ExamTimetable::schedule() const {
    int n = static_cast<int>(names_.size());
    Timetable t;
    t.subjects = names_;
    t.slotOf.assign(n, -1);

    // Welsh-Powell: colour vertices in decreasing order of degree
    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    std::sort(order.begin(), order.end(),
              [&](int a, int b) { return adj_[a].size() > adj_[b].size(); });

    for (int u : order) {
        std::vector<bool> used(n + 1, false);
        for (int v : adj_[u])
            if (t.slotOf[v] != -1) used[t.slotOf[v]] = true;
        int c = 0;
        while (used[c]) ++c;
        t.slotOf[u] = c;
        t.slots = std::max(t.slots, c + 1);
    }
    return t;
}

bool ExamTimetable::valid(const Timetable& t) const {
    for (size_t u = 0; u < adj_.size(); ++u)
        for (int v : adj_[u])
            if (t.slotOf[u] == t.slotOf[v]) return false;
    return true;
}

}  // namespace campus
