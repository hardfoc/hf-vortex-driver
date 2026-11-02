# Installation

Add the driver to a CMake or ESP-IDF build.

## Requirements

| Item | Requirement |
| --- | --- |
| Language | C++20 (the library sets `CMAKE_CXX_STANDARD 20`) |
| Build | CMake 3.16 or later, or ESP-IDF |
| Dependencies | None: the standard library only |
| Heap, threads, RTOS | None |

## Submodule

```bash
git submodule add https://github.com/hardfoc/hf-vortex-driver.git external/hf-vortex-driver
```

In HardFOC projects it already arrives through [hf-core-drivers](https://github.com/hardfoc/hf-core-drivers)
as `external/hf-vortex-driver`.

## CMake

```cmake
add_subdirectory(external/hf-vortex-driver)
target_link_libraries(my_app PRIVATE hf_vortex_driver)
```

`hf_vortex_driver` is a static library of `src/VortexHostSession.cpp` with `inc/` as its public include path.
Include headers as `#include "vortex/VortexHostSession.hpp"`.

| Option | Default | Effect |
| --- | --- | --- |
| `HF_VORTEX_BUILD_DOC_SNIPPETS` | `OFF` | Builds the documentation code in `tests/docs` and its frame check (`ctest`) |
| `HF_VORTEX_BASECAN_INCLUDE_DIR` | empty | Path to `hf-internal-interface-wrap/inc/base`; also compiles the BaseCan adapter on the [Porting](porting.md) page |

## ESP-IDF

There is no component wrapper; list the source in your component:

```cmake
idf_component_register(
    SRCS "main.cpp" "${CMAKE_CURRENT_LIST_DIR}/../external/hf-vortex-driver/src/VortexHostSession.cpp"
    INCLUDE_DIRS "." "${CMAKE_CURRENT_LIST_DIR}/../external/hf-vortex-driver/inc")
```

## Layout

| Path | Contents |
| --- | --- |
| `inc/vortex/` | Public headers |
| `src/` | `VortexHostSession.cpp` |
| `examples/generic/` | Host example with a loopback transport |
| `tests/docs/` | The code on this site, compiled and checked |

## Version

No release is tagged yet; pin a commit of `main` in your submodule.

Next: [Hardware](hardware.md).
