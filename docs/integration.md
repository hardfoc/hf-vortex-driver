# Integration: HAL, handlers, and hf-utils-canopen

## Layers

1. **hf-vortex-driver** (this repo) — `VortexCanFrame`, `VortexCanTransport`, `VortexHostSession`, `VortexProtocol` constants. No ESP-IDF.
2. **hf-utils-canopen** — `CanFrame`, `CanOpen::BuildNmt`, `CanOpen::BuildSdoDownload`, `CanOpenExtras` parsers. No HAL.
3. **hf-hal-*** (e.g. `hf-hal-vortex-v1`) — `VortexBaseCanTransport` implements `VortexCanTransport` using `BaseCan` from `CommChannelsManager`. Lives under `driver-handlers/`.
4. **Application** (Vortex `main` / Flux `middleware`) — threads, timeouts, and optional CANopenNode `CO_driver` that calls into `BaseCan`.

## Wire compatibility

`VortexHostSession` encodes NMT and expedited SDO the same way as `hf-utils-canopen` helpers. You may build frames in either place; **handlers** can convert `CanFrame` ↔ `VortexCanFrame` with field-wise copy for identical COB-IDs and payload.

## Flux middleware

`MwApi` or a dedicated `VortexHostClient` thread should own continuous polling and reconnection. It obtains `BaseCan*` from `CommChannelsManager`, wraps `VortexBaseCanTransport`, and uses `VortexHostSession` or direct `CanOpen` builders. HAL does not reference Flux apps.

⬅ [Back to index](index.md) · ➡ [architecture.md](architecture.md)
