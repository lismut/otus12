#pragma once

#include <cstddef>
#include "threadqueue.h"

namespace async {

using handle_t = size_t;

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size, std::shared_ptr<QueueString> qq);
void disconnect(handle_t handle);

}
