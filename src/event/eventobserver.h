
#pragma

#include <memory>

namespace tire {

struct EventBase;

struct EventObserver : public std::enable_shared_from_this<EventObserver> {
    virtual auto handleEvent( std::unique_ptr<EventBase> event ) -> void = 0;
    virtual ~EventObserver() = default;
};

}  // namespace tire
