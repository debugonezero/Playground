# Axiomatic Remote Playground

## Architecture: Passive Polling Bridge
The hardware (iMac/Phone) acts as a **Receiver**. It polls `payload.json` every 2s.
If `payload.id` changes, it executes the new `wgsl` and `js`.

## Payload Schema (STRICT JSON ONLY)
- `id`: Unique string (triggers the update).
- `wgsl`: Raw WGSL shader code.
- `html`: UI overlay injected into the GUI.
- `js`: Logic string executed via `new Function(p, state)`.

## Device Support
- **Desktop:** Full resolution WebGPU.
- **Mobile:** Responsive viewport scaling via `devicePixelRatio`.
