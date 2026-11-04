#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace stacktrace {

struct Entry {
    uintptr_t programCounter = 0;
    uintptr_t symbolAddress = 0;
    uintptr_t symbolSize = 0;
    uintptr_t symbolOffset = 0;
    int lineNumber = 0;
    std::array<char, 128> symbolName = {};
    std::array<char, 128> functionName = {};
    std::array<char, 256> filename = {};
};

using EntryCallback = bool (*)(const Entry& entry, std::size_t index, void* userData);

void
initialize(char** argv);

void
get_stacktrace(EntryCallback callback, void* userData = nullptr, int skipFrames = 0);

Entry
get_current_entry();

std::size_t
get_entries(Entry* entries, std::size_t entryCount, int skipFrames);

std::size_t
get_entry_count(std::size_t maxEntryCount = SIZE_MAX, int skipFrames = 0);

void
print_entry(const Entry& entry, std::FILE* file);

void
print_stacktrace(std::FILE* file = stderr, int skipFrames = 0);

}  // namespace stacktrace
