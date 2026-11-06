# TODO

## Classes
* Heap
* Stacktrace
* Contracts
* Function
* Result
* Option
* ScopeGuard (and DEFER?)



* Study carefully the generated code
* Make Function be used by everyone like contracts and maybe even stacktrace, this means we need
  to rethink our structure possibly, we might just have them NOT be libraries? I'm not sure at all!
* Add `ccache` back to improve build times and related to that, consider having some cpp files
  per subsystem maybe (which themselves can be _maybe_ CMake libs but that adds some extra complexity)
* Our own `Function` implementation, with proper tests for it
* Our own `Option` type, with proper tests for it, this will be very similar to `Result`
* Pipe BHLIB CMAKE options from CMAKE into the compile options or definitions or whatever
* Reconsider our stance on modules, we stand to lose a lot and gain little, header only
  will achieve what we want at the slight cost of build times ballooning (which we will use
  ccache for) and `inline` everywhere, which is incredibly frustrating.
  The better strategy is that each of our current modules (henceforth I will call them subsystems)
  becomes its own shared library, a CMake library that has all of its implementation is headers only
  and has a single exposing cpp file which is the one thing compiled by CMake in the library.
  This library thus gets cached for use with `ccache`, which we need to bring back

## Architecture


## Verification

* Build tests to run with ASan, MSan, TSan, UBSan and Valgrind

### Investigate

* Docker build on a CI (very likely GitHub actions)
* Need to have a yocto build on the CI with tests running on the target architecture
* `cppcheck`
* Clang analyzer
* CodeChecker
* Yocto and qemu emulation
* A code coverage tool
* C Bounded Model Checker

## Tooling

* Convert scripts to python
* Make `init` script to install dependencies like `uv` for simple `git clone` which docker will use

## Documentation

* Document memory locking issue a bit better maybe

## Implementation

* Better way of ignoring unused variables?
* IMPORTANT! Need a better way where we can have `mlockall` _just_ work, no need for sudo and no
  hacks or tricks
* Set panic handler and allow panic to take a fmt formattable string!
* Ensure Result is ready to use, use contracts in it and try to test it as much as possible
* Catch2 needs the heap to be unsealed to write the strings when a test fails
* It also needs exceptions to be available in order to not end with a SIGABRT after the first failed test
* IMPORTANT! FIX all in place data holders (like Result and RingBuffer and Array) to ensure
  that copy assignment operators don't happen on uninitialized garbage data, use placement new and remember
  destructor logic too