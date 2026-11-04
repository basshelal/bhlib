# bhlib

Work in progress

## Usage

Everything is one library, add it using `cpm` and link `bhlib::bhlib`:

```cmake
CPMAddPackage(
        NAME bhlib
        GITHUB_REPOSITORY basshelal/bhlib
        GIT_TAG master
)

target_link_libraries(my-app PRIVATE bhlib::bhlib)
```

Headers are included with the `bhlib/` prefix, for example `#include "bhlib/core/core.hpp"`.

Linking `bhlib` wraps `malloc` and friends and replaces the global `operator new` and `delete` of the final binary, this
is what makes `heap::seal_heap()` work and is set up for you. The heap stays usable as normal until it is sealed.

### Static or Shared

`bhlib` is static unless `BUILD_SHARED_LIBS` is set, add `OPTIONS "BUILD_SHARED_LIBS ON"` to `CPMAddPackage` to get a
shared library. This also applies to `fmt` and to the rest of your project.

Prefer static. The linker can only wrap allocations in what it is linking, so in a shared build anything that allocates
inside of another shared library, such as `fmt`, goes unnoticed by `heap::seal_heap()`.

## Requirements

* `cmake`
* `ninja`
* `gcc`
* `make` (to compile `libbacktrace`)
* `mold` (optional) for faster link times
* `uv` (to manage python and additional tools like `clang-tidy`)
`socketcan` ?

## Dependencies

All managed using `cpm`

## Unlocking Memory Limits

`mem::Manager` calls `mlockall` with large numbers which will likely fail depending on your system configuration

### Temporary Solution

`sudo setcap cap_ipc_lock+ep <path-to-binary>`

This will need to be done every time the binary is recompiled

### Permanent Solution

`sudo vim /etc/security/limits.d/99-bhlib-memlock.conf`

Then add to the file:

`@wheel - memlock unlimited`

or swap `@wheel` with only your user, and unlimited with the desired number like `524288`.

logout and back in the limits should be as expected by running `ulimit -l` to see.

## Structure
