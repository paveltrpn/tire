
#pragma once

#include <vector>
#include <memory>

#include <uv.h>

#include "event_scheduler.h"

namespace tire::io {

struct ReadHandle {
    uv_fs_t open_;
    uv_fs_t read_;
    uv_fs_t close_;
    uv_buf_t buf_;
};

struct WriteHandle {
    uv_fs_t open_;
    uv_fs_t write_;
    uv_fs_t close_;
    uv_buf_t buf_;
};

struct File final : EventScheduler {
private:
    std::vector<std::unique_ptr<ReadHandle>> readPool_{};
    std::vector<std::unique_ptr<WriteHandle>> writPool_{};
};

}  // namespace tire::io