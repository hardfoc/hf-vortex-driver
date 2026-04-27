# Scripts (reference)

Place **project-specific** IDF build helpers here if you add a private HAL test app, for example:

- `idf_set_target.sh` — export `IDF_PATH`, select `esp32c6`
- `run_generic_host_test.sh` — `cd ../generic && cmake ...` for CI

The **hf-vortex-driver** package does not clone CANopenNode or start FreeRTOS in-tree; a real bus test belongs in:

- `hf-hal-vortex-v1` (full firmware), or
- a private `vortex-hal-bringup` app that depends on this driver + `hf-utils-canopen` + `EspCan`.
