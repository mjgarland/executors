#ifndef STD_EXPERIMENTAL_BITS_PREFER_H
#define STD_EXPERIMENTAL_BITS_PREFER_H

#include <experimental/bits/has_require_members.h>

namespace std {
namespace experimental {
inline namespace concurrency_v2 {
namespace execution {

template<class Property> struct is_require_only;

namespace prefer_impl {

template<class E> void prefer(const E&, const oneway_t&) = delete;
template<class E> void prefer(const E&, const twoway_t&) = delete;
template<class E> void prefer(const E&, const single_t&) = delete;
template<class E> void prefer(const E&, const bulk_t&) = delete;

template<class Executor, class Property>
constexpr auto prefer(Executor&& ex, Property&& p)
  -> decltype(std::forward<Executor>(ex).require(std::forward<Property>(p)))
{
  return std::forward<Executor>(ex).require(std::forward<Property>(p));
}

template<class Executor, class Property>
constexpr auto prefer(Executor ex, Property&&)
  -> typename std::enable_if<!has_require_members<Executor, Property>::value, Executor>::type
{
  return ex;
}

struct prefer_fn
{
  template<class Executor, class Property>
  constexpr auto operator()(Executor&& ex, Property&& p) const
    noexcept(noexcept(prefer(std::forward<Executor>(ex), std::forward<Property>(p))))
    -> decltype(prefer(std::forward<Executor>(ex), std::forward<Property>(p)))
  {
    return prefer(std::forward<Executor>(ex), std::forward<Property>(p));
  }

  template<class Executor, class Property0, class Property1, class... PropertyN>
  constexpr auto operator()(Executor&& ex, Property0&& p0, Property1&& p1, PropertyN&&... pn) const
    noexcept(noexcept(std::declval<prefer_fn>()(std::declval<prefer_fn>()(std::forward<Executor>(ex), std::forward<Property0>(p0)), std::forward<Property1>(p1), std::forward<PropertyN>(pn)...)))
    -> decltype(std::declval<prefer_fn>()(std::declval<prefer_fn>()(std::forward<Executor>(ex), std::forward<Property0>(p0)), std::forward<Property1>(p1), std::forward<PropertyN>(pn)...))
  {
    return (*this)((*this)(std::forward<Executor>(ex), std::forward<Property0>(p0)), std::forward<Property1>(p1), std::forward<PropertyN>(pn)...);
  }
};

template<class T = prefer_fn> constexpr T customization_point{};

} // namespace prefer_impl
} // namespace execution
} // inline namespace concurrency_v2
} // namespace experimental
} // namespace std

#endif // STD_EXPERIMENTAL_BITS_PREFER_H
