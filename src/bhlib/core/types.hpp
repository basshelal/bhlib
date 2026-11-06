#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

#include "bhlib/core/TypeDef.hpp"

#define DEFINE_TYPE_DEF(Name, Type) \
    struct Name##Tag {};            \
    using Name = bhlib::TypeDef<Type, Name##Tag>

namespace bhlib {
using Address = uintptr_t;
using SizeBytes = std::size_t;
using Alignment = std::size_t;
using Offset = std::size_t;
using Count = std::size_t;
using Index = std::size_t;

// DEFINE_TYPE_DEF(Address, std::uintptr_t);

// DEFINE_TYPE_DEF(SizeBytes, std::size_t);

// DEFINE_TYPE_DEF(Alignment, std::size_t);

// DEFINE_TYPE_DEF(Offset, std::size_t);

// DEFINE_TYPE_DEF(Count, std::size_t);

// DEFINE_TYPE_DEF(Index, std::size_t);

using FileDescriptor = int;
using Milliseconds = std::uint64_t;
using NanoSeconds = std::uint64_t;

using FilePath = std::string_view;

}  // namespace bhlib
