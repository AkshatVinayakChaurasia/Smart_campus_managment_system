// student_records.h  --  Module 4: Student Record Management
// Concepts: Hash table with separate chaining (own implementation, no unordered_map)
#pragma once
#include <functional>
#include <list>
#include <string>
#include <vector>

namespace campus {

struct Student {
    std::string roll;
    std::string name;
    std::string branch;
    int year = 0;
    double cgpa = 0.0;
};

class StudentTable {
public:
    explicit StudentTable(size_t buckets = 101);   // prime size keeps the spread even

    bool insert(const Student& s);                 // false if roll no. already exists
    const Student* search(const std::string& roll) const;
    bool remove(const std::string& roll);

    size_t size() const { return count_; }
    size_t bucketCount() const { return table_.size(); }
    double loadFactor() const { return static_cast<double>(count_) / table_.size(); }
    size_t longestChain() const;

    void forEach(const std::function<void(const Student&)>& fn) const;
    bool loadFromFile(const std::string& path);

private:
    std::vector<std::list<Student>> table_;
    size_t count_ = 0;
    size_t hash(const std::string& key) const;     // polynomial rolling hash
};

}  // namespace campus
