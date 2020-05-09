//
// Created by nether on 08.05.2020.
//

#pragma once

#include <memory>
#include <atomic>
#include <mutex>
#include "bulk.h"

class QueueItem
{
public:
    std::unique_ptr<bulk> data;
    std::shared_ptr<QueueItem> next;

    explicit QueueItem(std::unique_ptr<bulk> _data) noexcept : data(std::move(_data)), next(nullptr) {}
};


class Queue {
public:
    Queue() : head(nullptr), tail(nullptr) {}
    bool empty() { return head == tail; }
    void enqueue(std::unique_ptr<bulk> value)
    {
        std::lock_guard<std::mutex> guard(mtx);
        auto item = std::make_shared<QueueItem>(std::move(value));
        if(empty()) {
            head = std::make_shared<QueueItem>(nullptr);
            tail = item;
            head->next = tail;
        } else {
            tail->next = item;
            tail = item;
        }
    }
    std::unique_ptr<bulk> dequeue()
    {
        std::lock_guard<std::mutex> guard(mtx);
        if (empty())
            return nullptr; // queue is empty
        head = head->next;

        return std::move(head->data);
    }
private:
    std::shared_ptr<QueueItem> head;
    std::shared_ptr<QueueItem> tail;
    std::mutex mtx;
};
