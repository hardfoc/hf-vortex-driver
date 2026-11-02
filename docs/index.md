---
hide:
  - navigation
  - toc
---

<div class="hf-hero" markdown>
<span class="hf-kicker">HardFOC driver</span>

# Vortex Driver

<p>A C++20 CANopen host client for HardFOC Vortex devices. It builds NMT and expedited SDO download frames with
named CiA 301 and CiA 402 objects and hands them to a CAN transport you provide, on any MCU or a PC.</p>

[Quickstart](quickstart.md){ .md-button .md-button--primary }
[API guide](api-guide.md){ .md-button }
[Source on GitHub](https://github.com/hardfoc/hf-vortex-driver){ .md-button }
</div>

The driver has no platform code. You give it an object with one method, `send()`, that puts a classic CAN frame
on the bus; it does the CANopen encoding: COB-IDs, command specifiers, little-endian payloads. It has no heap, no
threads and no dependency on CANopenNode, ESP-IDF or the HardFOC HAL.

<div class="hf-chip hf-chip--board" markdown>
<div class="hf-chip__head">
<span class="hf-chip__kicker">The device on the other end of the bus</span>
<span class="hf-chip__name">Vortex V1</span>
<span class="hf-chip__vendor">HardFOC</span>
</div>

MCU
:   ESP32-C6 with a TWAI (classic CAN) controller

CAN
:   TWAI TX GPIO14, RX GPIO15, external transceiver; 500 kbit/s in the Vortex V1 HAL

Motor control
:   TMC9660 smart gate driver with on-chip FOC

CANopen role
:   Server (slave), with CANopenNode from the HAL's `third_party/`; its object dictionary is authoritative

This driver
:   The client side: NMT commands and expedited SDO downloads to a node ID

[Vortex V1 HAL](https://hardfoc.github.io/hf-hal-vortex-v1/) ·
[Pin map](https://hardfoc.github.io/hf-hal-vortex-v1/latest/hardware/pin-map/)
</div>

## What you get

- **NMT in one call.** `sendNmt()` with `NmtCommand::Start`, `Stop`, `PreOperational`, `ResetNode` or
  `ResetCommunication`, to one node or all. See [API guide](api-guide.md#nmt).
- **Expedited SDO writes.** `sdoDownloadExpedited()` writes 1, 2 or 4 bytes to any index and sub-index, with the
  CiA 301 command specifier for the size. See [API guide](api-guide.md#write-an-object-sdo-download).
- **Named objects.** Controlword, statusword, modes of operation, position and velocity actual value, error
  register and a HardFOC vendor range. See [Hardware](hardware.md#object-dictionary-constants).
- **Any CAN driver.** One virtual method to implement; a BaseCan adapter is ten lines. See [Porting](porting.md).
- **Checked bytes.** The code on these pages is compiled and run on the host against a loopback transport that
  compares every frame. See [Testing and CI](testing.md).

!!! note "Transmit only"
    The session sends requests and does not read the bus: no SDO responses or uploads, no PDOs, heartbeats or
    EMCY. Read those with your CAN driver, [hf-utils-canopen](https://github.com/hardfoc/hf-utils-canopen) or
    CANopenNode. [API guide](api-guide.md#what-the-session-does-not-do) lists what to use for each.

## Where to go next

<div class="grid cards" markdown>

-   :material-rocket-launch-outline: **[Quickstart](quickstart.md)**

    ---

    Wire the bus, write the transport, start a node.

-   :material-sine-wave: **[Hardware](hardware.md)**

    ---

    Bus settings, COB-IDs, frame layouts, objects.

-   :material-code-braces: **[API guide](api-guide.md)**

    ---

    NMT, SDO and the CiA 402 state machine.

</div>
