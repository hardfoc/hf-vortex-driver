# Changelog

All notable changes to hf-vortex-driver. No release is tagged yet; changes are listed by the date they reached
`main`.

## Unreleased

- Fixed: `kIdxErrorBehavior` is now `0x1029` (CiA 301 error behaviour). It was `0x1021`, the "store EDS" object,
  so code that wrote error behaviour through the constant addressed the wrong object.
- Host unit test `tests/protocol` (`HF_VORTEX_BUILD_TESTS`) checks the protocol constants against CiA 301 and
  CiA 402.
- Documentation moved to the HardFOC MkDocs site; the code on the pages is compiled and its frames are checked
  on the host (`HF_VORTEX_BUILD_DOC_SNIPPETS`).

## 2026-04-27

- First version: `VortexCanFrame`, `VortexCanTransport`, `VortexHostSession` (NMT, expedited SDO download),
  `VortexProtocol.hpp` constants, host generic example and CI.
