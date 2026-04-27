# ESP32: reference-only

This directory documents **how** a future ESP32 example would be structured (`app_config.yml` + `scripts/`) without providing a self-contained, flashable project that embeds:

- CANopenNode,
- a full `CO_driver` for ESP32 TWAI, or
- HardFOC HAL (`EspCan` / `CommChannelsManager`).

**Why:** A minimal bus exercise requires the whole HAL, IDF, and transceiver strapping; that lives in **hf-hal-vortex-v1** and the **Vortex** application, not in this portable driver.

**To verify on real hardware:** use the HAL project’s `main` component, inject `VortexBaseCanTransport` (see `docs/integration.md`), and optionally compare frames against `examples/generic` loopback output.
