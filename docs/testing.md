# Testing and CI

How the driver is checked: on the host, on every pull request, without hardware.

## Host checks

| Check | Command | Covers |
| --- | --- | --- |
| Generic example | `cmake -B build -S examples/generic && cmake --build build && ./build/vortex_driver_generic_check` | NMT start and an SDO write through a loopback transport |
| Documentation frames | `cmake -B build-docs -S . -DHF_VORTEX_BUILD_DOC_SNIPPETS=ON && cmake --build build-docs && ctest --test-dir build-docs` | Every snippet on this site compiles with `-Wall -Wextra -Werror`, and `check_frames` compares each frame it sends with the bytes the pages show |

[`tests/docs/check_frames.cpp`](../tests/docs/check_frames.cpp) checks the NMT and SDO frames of the
Quickstart, the broadcast NMT, the 1-, 2- and 4-byte SDO writes, the refusal of size 3 and the CiA 402 enable
sequence.

## CI workflows

| Workflow | What it checks |
| --- | --- |
| Host generic test (`ci-generic.yml`) | Builds and runs the generic example, then builds the documentation code (with the BaseCan adapter against hf-internal-interface-wrap `main`) and runs the frame check |
| Docs (`ci-docs-publish.yml`) | Strict site build, figure and generated-table drift, Doxygen, offline link check, Lighthouse budget; publishes the site from `main` |

The docs workflow calls the shared, versioned workflow of
[hf-general-ci-tools](https://github.com/hardfoc/hf-general-ci-tools).

## On hardware

There is no hardware test in this repository. Check a port as described in
[Porting](porting.md#how-to-check-a-port), with a bus analyser.
