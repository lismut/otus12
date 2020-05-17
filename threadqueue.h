//
// Created by nether on 08.05.2020.
//

#pragma once

#include <memory>
#include <atomic>
#include <mutex>
#include "bulk.h"

template<typename T>
class QueueItem
{
public:
    std::unique_ptr<T> data;
    std::shared_ptr<QueueItem<T>> next;

    explicit QueueItem(std::unique_ptr<T> _data) noexcept : data(std::move(_data)), next(nullptr) {}
};

template<typename T>
class QueueTemplate {
public:
    QueueTemplate() : head(nullptr), tail(nullptr) {}
    bool empty() { return head == tail; }
    void enqueue(std::unique_ptr<T> value)
    {
        std::lock_guard<std::mutex> guard(mtx);
        auto item = std::make_shared<QueueItem<T>>(std::move(value));
        if(empty()) {
            head = std::make_shared<QueueItem<T>>(nullptr);
            tail = item;
            head->next = tail;
        } else {
            tail->next = item;
            tail = item;
        }
    }
    std::unique_ptr<T> dequeue()
    {
        std::lock_guard<std::mutex> guard(mtx);
        if (empty())
            return nullptr; // queue is empty
        head = head->next;

        return std::move(head->data);
    }
private:
    std::shared_ptr<QueueItem<T>> head;
    std::shared_ptr<QueueItem<T>> tail;
    std::mutex mtx;
};

typedef QueueTemplate<bulk> Queue;
typedef QueueTemplate<std::string> QueueString;