// service_requests.h  --  Module 5: Service Request Handling   [IN PROGRESS]
// Concepts: Binary min-heap / priority queue (own implementation)
//
// Done   : heap insert / extract-min / peek, FIFO tie-break for equal priority
// TODO   : change priority of an existing request (decrease-key)
// TODO   : auto-escalation of requests that have waited too long
// TODO   : location-aware assignment (use Navigation module to pick nearest staff)
// TODO   : save / load pending requests from a file
#pragma once
#include <string>
#include <vector>

namespace campus {

struct Request {
    int id = 0;
    int priority = 3;          // 1 = emergency ... 5 = low
    std::string location;
    std::string description;
    int seq = 0;               // arrival order, used to break ties
};

class RequestHeap {
public:
    int submit(int priority, const std::string& location, const std::string& description);
    bool peek(Request& out) const;
    bool processNext(Request& out);       // removes and returns the most urgent request
    size_t size() const { return heap_.size(); }
    bool empty() const { return heap_.empty(); }

private:
    std::vector<Request> heap_;
    int nextId_ = 1;
    static bool higher(const Request& a, const Request& b);   // a more urgent than b?
    void siftUp(size_t i);
    void siftDown(size_t i);
};

}  // namespace campus
