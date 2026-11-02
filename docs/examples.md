# Examples

What the examples in the repository show and how to run them.

| Example | Runs on | Shows |
| --- | --- | --- |
| [`examples/generic`](../examples/generic/main.cpp) | Linux, macOS, Windows | A loopback transport, NMT start and an SDO write to `0x6060`, checked byte by byte |
| [`examples/esp32`](../examples/esp32/REFERENCE_ONLY.md) | Reference only | Intended ESP32 scenarios (`app_config.yml`); no flashable project, see below |

## Run the host example

```bash
cd examples/generic
cmake -B build -S .
cmake --build build
./build/vortex_driver_generic_check
```

It prints `hf-vortex-driver generic self-test OK (2 frames)` and exits 0, or names the frame that differed and
exits 1.

## On hardware

A bus test needs a board, its CAN driver and a CANopen node at the other end, which this portable repository does
not carry. Build it in the firmware that owns the bus: wrap the board's `BaseCan` with the
[adapter](porting.md#over-basecan), create a `VortexHostSession` and send to the node, as in the
[Quickstart](quickstart.md).

Next: [Troubleshooting](troubleshooting.md).
