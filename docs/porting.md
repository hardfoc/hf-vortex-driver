# Porting

Connect the driver to a CAN controller: the one method you implement, its contract, and a working adapter for
HardFOC's `BaseCan`.

## The interface

| Method | Called | Must |
| --- | --- | --- |
| `bool send(const VortexCanFrame& frame) noexcept` | Once per NMT or SDO call, from the caller's thread | Queue or send the frame and return true, or return false; never throw |

That is the whole interface. The session calls it synchronously and passes its result back to your code.

## Rules for `send()`

- **Copy the frame.** It lives on the session's stack; do not keep the reference.
- **Classic CAN.** `dlc` is 0 to 8, `extended` and `rtr` are false for every frame the session builds. Send it as a
  classic (non-FD) frame.
- **Decide on blocking.** Return at once with false when the TX queue is full (the adapter below does), or block
  with a short timeout; either way, do not block forever.
- **Report bus errors as false.** Bus-off, controller not started, no acknowledgement: the caller retries or
  reports.
- **Be thread-safe if you share it.** Two sessions on one transport in two tasks call `send()` concurrently.

## Over BaseCan

[hf-internal-interface-wrap](https://github.com/hardfoc/hf-internal-interface-wrap)'s `BaseCan` is the CAN
interface the HardFOC HALs expose (`EspCan` on the ESP32-C6 TWAI today). Its `hf_can_message_t` holds FD-sized
payloads; a classic frame uses the first 8 bytes.

```cpp title="tests/docs/basecan_transport.cpp"
--8<-- "tests/docs/basecan_transport.cpp:basecan"
```

`TrySendMessage()` is `SendMessage(message, 0)`: it queues the frame or fails at once with `CAN_ERR_QUEUE_FULL` or
`CAN_ERR_BUS_OFF`. On Vortex V1, `CommChannelsManager::GetCanBus()` returns the `BaseCan*` (null when the bus did not
initialise).

## For tests

A transport that records frames lets host tests check the exact bytes, as this repository's own check does:

```cpp
--8<-- "tests/docs/loopback_transport.hpp:loopback"
```

## How to check a port

<div class="hf-steps" markdown>

1. Run `examples/generic` on the host and note the two frames it checks.
2. Send the same calls through your transport and watch the bus with an analyser: IDs `0x000` and `0x601`, the
   bytes in [Hardware](hardware.md#sdo-expedited-download-frame).
3. Disconnect the bus (no acknowledgement) and check that `send()` eventually returns false rather than hanging.

</div>

Next: [Examples](examples.md).
