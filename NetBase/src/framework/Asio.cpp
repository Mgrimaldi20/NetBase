#define ASIO_SEPARATE_COMPILATION

#if defined(NETBASE_MSVC)
#pragma warning(push)
#pragma warning(disable: 6255 26439 26495 6387 6031 6258 6031 26110 28112 26819 26437)

#elif defined(NETBASE_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "--"

#elif defined(NETBASE_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "--"
#endif

#include "asio/impl/src.hpp"

#if defined(NETBASE_MSVC)
#pragma warning(pop)

#elif defined(NETBASE_CLANG)
#pragma clang diagnostic pop

#elif defined(NETBASE_GCC)
#pragma GCC diagnostic pop
#endif
