module;

#include <print>

export module test_module:partition1;

namespace tire {

export struct PartitionOneObject final {
    PartitionOneObject() { std::println( "hello from partition one!" ); }
};

}  // namespace tire
