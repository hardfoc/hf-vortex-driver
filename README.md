# HF-VORTEX-DRIVER

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

**Hardware-agnostic** C++20 library for talking to a **HardFOC Vortex** class device over **CAN** using standard CANopen-style frames (NMT, expedited SDO) and named **CiA 301 / 402** index constants.  
Transport is **injected** through `VortexCanTransport` — the same pattern as `TMC9660CommInterface` or `IBNO085Transport` in other HardFOC external drivers.

| Layer | Role |
|-------|------|
| **This repo** | Frame type, protocol constants, `VortexHostSession` (NMT + expedited SDO TX encoding) |
| **hf-utils-canopen** | Optional: `CanFrame` + `BuildNmt` / `BuildSdoDownload` — **same wire format** as `VortexHostSession` |
| **hf-hal-vortex-v1** | `VortexBaseCanTransport` implements `VortexCanTransport` with `BaseCan` from `CommChannelsManager` |
| **Application / middleware** | Threads, timeouts, optional CANopenNode `CO_driver` calling `BaseCan` |

## Repository layout

```
inc/vortex/     Public headers (VortexCanFrame, Transport, Protocol, HostSession)
src/            VortexHostSession.cpp
examples/generic/   Host CMake smoke test (loopback) — **builds without ESP-IDF**
examples/esp32/   app_config.yml + scripts — **reference only** (no bundled IDF + CANopenNode)
docs/           Jekyll markdown + Doxygen output under docs/html
```

## Quick start (host test)

```bash
cd examples/generic
cmake -B build -S .
cmake --build build
./build/vortex_driver_generic_check
```

Expected: `hf-vortex-driver generic self-test OK (2 frames)`.

## HAL integration

See [docs/integration.md](docs/integration.md). In short: obtain `BaseCan*`, construct `VortexBaseCanTransport`, then `VortexHostSession` for high-level sends.

## Documentation

- **Doxygen:** `doxygen Doxyfile` → `docs/html/`
- **Jekyll (GitHub Pages):** `docs/_config.yml` with `index.md`, `integration.md`, `architecture.md`

## CI

- `.github/workflows/ci-generic.yml` — builds and runs `examples/generic`
- `.github/workflows/docs.yml` — Doxygen HTML artifact

## License

GPL-3.0 — see [LICENSE](LICENSE).
