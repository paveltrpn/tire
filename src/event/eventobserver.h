
#pragma

namespace tire {

struct Event;

struct EventObserver {
    virtual auto handleEvent( Event* event ) -> void = 0;
    virtual ~EventObserver() = default;

private:
};

}  // namespace tire
