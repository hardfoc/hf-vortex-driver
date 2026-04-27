# Generic (host) example

This folder builds a **small native binary** (no ESP-IDF) to validate:

- `VortexCanTransport` + in-memory loopback
- NMT and expedited SDO request encoding in `VortexHostSession`

## Build (Linux / macOS / WSL)

```bash
cd examples/generic
cmake -B build -S . && cmake --build build
./build/vortex_driver_generic_check
# expect: hf-vortex-driver generic self-test OK (2 frames)
```

## Build (MSVC)

```powershell
cmake -B build -S .
cmake --build build --config Release
./build/Release/vortex_driver_generic_check.exe
```

## Relationship to `hf-utils-canopen`

The wire format matches `CanOpen::BuildNmt` / `BuildSdoDownload` in `hf-utils-canopen`. In production firmware, you may use either this session class **or** build frames with `hf-utils-canopen` and send through `VortexBaseCanTransport` (HAL) — the bytes on the bus should be identical for the same node id / indices.
