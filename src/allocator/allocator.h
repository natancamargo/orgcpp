#pragma once

namespace orgcpp {
namespace allocator {
template <class T> T *allocate();
template <class T> void deallocate(T *element);
} // namespace allocator
} // namespace orgcpp
