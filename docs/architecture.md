# Architecture

```mermaid
flowchart LR
  subgraph def["hf-vortex-driver"]
    VT[VortexCanTransport]
    VS[VortexHostSession]
  end
  subgraph utils["hf-utils-canopen"]
    CF[CanFrame + builders]
  end
  subgraph hal["hf-hal — driver-handlers"]
    VB[VortexBaseCanTransport]
    BC[BaseCan / EspCan]
  end
  VS --> VT
  VB -->|implements| VT
  VB --> BC
  CF -.->|same byte layout| VS
```

- **Override point:** subclass `VortexCanTransport` (or use `VortexBaseCanTransport` in HAL) — same idea as `Tmc9660SpiCommInterface` + `BaseSpi`, not CRTP.
- **CANopenNode** (optional) sits beside application code: `CO_driver` calls `BaseCan`, not the reverse.

⬅ [integration.md](integration.md) · [Back to index](index.md)
