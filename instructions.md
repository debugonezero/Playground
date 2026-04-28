# Axiomatic Remote Execution Spec

## Bridge Architecture
This playground uses a **Passive Polling Bridge**. The agent does not send messages to the window. Instead:
1. Agent updates a JSON payload at a central Gist URL.
2. The Playground (on the hardware) fetches the Gist every 2 seconds.
3. If the `payload.id` has changed, the hardware executes the new `wgsl` and `js`.

## Payload Schema
```json
{
  "id": "unique_string",
  "wgsl": "raw_compute_shader",
  "html": "ui_layout",
  "js": "execution_logic"
}
