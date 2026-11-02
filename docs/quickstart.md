# Quickstart

Connect a host to a Vortex node, write the transport, and start the node in a CiA 402 mode in about 15 lines.

!!! danger "A started drive can move the motor"
    `NmtCommand::Start` puts the node in Operational, and the CiA 402 enable sequence powers the motor. Clamp the
    rotor or remove the load until you have checked the node ID and the objects you write.

## Wiring

| Host | Vortex V1 | Notes |
| --- | --- | --- |
| CAN_H | CAN_H | Twisted pair with CAN_L |
| CAN_L | CAN_L | |
| GND | GND | A common reference for the transceivers |
| 120 Ω termination | 120 Ω termination | One at each end of the bus, none in between |

The bit rate must match on every node: 500 kbit/s is what the Vortex V1 HAL configures. See
[Hardware](hardware.md#bus-settings).

## Add the driver

```bash
git submodule add https://github.com/hardfoc/hf-vortex-driver.git external/hf-vortex-driver
```

```cmake
add_subdirectory(external/hf-vortex-driver)
target_link_libraries(my_app PRIVATE hf_vortex_driver)
```

## The transport

The driver calls one method of yours, `send()`. For a first test on a PC, record the frames instead of sending
them:

```cpp title="tests/docs/loopback_transport.hpp"
--8<-- "tests/docs/loopback_transport.hpp:loopback"
```

On a board, wrap your CAN driver instead: [Porting](porting.md) has a ten-line adapter over `BaseCan`.

## Start a node

```cpp title="tests/docs/quickstart.cpp"
--8<-- "tests/docs/quickstart.cpp:quickstart"
```

## What each step does

| Step | Frame on the bus | Effect on the node |
| --- | --- | --- |
| `VortexHostSession session(bus)` | none | Binds the session to your transport; no state, no allocation |
| `sendNmt(NmtCommand::Start, 5)` | ID `0x000`, 2 bytes: `01 05` | Node 5 enters Operational |
| `sdoDownloadExpedited(5, 0x6060, 0, 3, 1)` | ID `0x605`, 8 bytes: `2F 60 60 00 03 00 00 00` | Writes 3 (profile velocity) to modes of operation |

Both calls return true when your transport accepted the frame. They do not wait for the node: the SDO response
(`0x585`) is yours to read if you need the confirmation.

## If it does not work

| Symptom | Check |
| --- | --- |
| `send()` returns false | The controller is bus-off or its TX queue is full: check termination, bit rate and that another node acknowledges |
| Frames leave, the node does nothing | Wrong node ID; the node is still booting (wait for its boot-up message `0x700 + node`) |
| SDO abort (`0x580 + node`, byte 0 `0x80`) | The object does not exist or is read-only on that node; check its EDS |
| Nothing on the bus at all | Transceiver in standby (on Vortex V1 the standby line is on the PCAL95555 expander) |

Next: [Hardware](hardware.md).
