#include "student_records.h"

#include <fstream>

#include "campus_data.h"

namespace campus {

StudentTable::StudentTable(size_t buckets) : table_(buckets) {}

size_t StudentTable::hash(const std::string& key) const {
    unsigned long long h = 0;
    for (unsigned char c : key) h = (h * 31 + c) % table_.size();
    return static_cast<size_t>(h);
}

bool StudentTable::insert(const Student& s) {
    auto& chain = table_[hash(s.roll)];
    for (const Student& x : chain)
        if (x.roll == s.roll) return false;
    chain.push_back(s);
    ++count_;
    return true;
}

const Student* StudentTable::search(const std::string& roll) const {
    const auto& chain = table_[hash(roll)];
    for (const Student& x : chain)
        if (x.roll == roll) return &x;
    return nullptr;
}

bool StudentTable::remove(const std::string& roll) {
    auto& chain = table_[hash(roll)];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->roll == roll) {
            chain.erase(it);
            --count_;
            return true;
        }
    }
    return false;
}

size_t StudentTable::longestChain() const {
    size_t best = 0;
    for (const auto& c : table_) best = std::max(best, c.size());
    return best;
}

void StudentTable::forEach(const std::function<void(const Student&)>& fn) const {
    for (const auto& chain : table_)
        for (const Student& s : chain) fn(s);
}

bool StudentTable::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        auto p = split(line, '|');
        if (p.size() != 5) continue;
        Student s{p[0], p[1], p[2], std::stoi(p[3]), std::stod(p[4])};
        insert(s);
    }
    return count_ > 0;
}

}  // namespace campus
