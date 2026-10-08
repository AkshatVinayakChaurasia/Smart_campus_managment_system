#include "service_requests.h"

#include <utility>

namespace campus {

bool RequestHeap::higher(const Request& a, const Request& b) {
    if (a.priority != b.priority) return a.priority < b.priority;
    return a.seq < b.seq;
}

void RequestHeap::siftUp(size_t i) {
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (!higher(heap_[i], heap_[p])) break;
        std::swap(heap_[i], heap_[p]);
        i = p;
    }
}

void RequestHeap::siftDown(size_t i) {
    size_t n = heap_.size();
    while (true) {
        size_t l = 2 * i + 1, r = 2 * i + 2, best = i;
        if (l < n && higher(heap_[l], heap_[best])) best = l;
        if (r < n && higher(heap_[r], heap_[best])) best = r;
        if (best == i) break;
        std::swap(heap_[i], heap_[best]);
        i = best;
    }
}

int RequestHeap::submit(int priority, const std::string& location, const std::string& description) {
    Request r;
    r.id = nextId_++;
    r.priority = priority;
    r.location = location;
    r.description = description;
    r.seq = r.id;
    heap_.push_back(r);
    siftUp(heap_.size() - 1);          // O(log n)
    return r.id;
}

bool RequestHeap::peek(Request& out) const {
    if (heap_.empty()) return false;
    out = heap_.front();
    return true;
}

bool RequestHeap::processNext(Request& out) {
    if (heap_.empty()) return false;
    out = heap_.front();
    heap_.front() = heap_.back();
    heap_.pop_back();
    if (!heap_.empty()) siftDown(0);   // O(log n)
    return true;
}

}  // namespace campus
