# Axiomatic Hardware Execution Skill

## Overview
This skill allows an AI Agent to execute low-level hardware diagnostics via a WebGPU-enabled playground.

## Operation Protocol
1. **Payload Structure**: Generate a JSON packet containing:
   - `wgsl`: Compute shader code (WebGPU).
   - `html`: UI structure for the playground.
   - `js`: Orchestration logic for the canvas and telemetry.
2. **Execution**: Use the `execute_payload` tool or push the JSON string to the `Shared Playground` session.
3. **Observation**: After execution, monitor `telemetry.log` or the `Shared Console` for results.

## Constraints
- Max WGSL complexity: 5.0 (O(n) per thread).
- Use `requestAnimationFrame` for all visual and telemetry loops.
- Report all metrics (FPS, load, stability) back to the session log.
