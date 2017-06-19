#ifndef STD_EXPERIMENTAL_BITS_HAS_REQUIRE_MEMBER_H
#define STD_EXPERIMENTAL_BITS_HAS_REQUIRE_MEMBER_H

#include <type_traits>
#include <utility>

namespace std {
namespace experimental {
inline namespace concurrency_v2 {
namespace execution {
namespace has_require_member_impl {

template<class>
struct type_check
{
  typedef void type;
};

template<class Executor, class Property, class = void>
struct eval : std::false_type {};

template<class Executor, class Property>
struct eval<Executor, Property,
  typename type_check<decltype(
    std::declval<Executor>().require(std::declval<Property>())
  )>::type> : std::true_type {};

} // namespace has_require_member_impl

template<class Executor, class Property>
struct has_require_member : has_require_member_impl::eval<Executor, Property> {};

} // namespace execution
} // inline namespace concurrency_v2
} // namespace experimental
} // namespace std

#endif // STD_EXPERIMENTAL_BITS_HAS_REQUIRE_MEMBER_H