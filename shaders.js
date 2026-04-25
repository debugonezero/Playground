// shaders.js - The Visual Firmware
export const shaderSource = `
struct Uniforms {
    time: f32,
    freq: f32,
    zoom: f32,
    padding: f32,
    pan: vec2f,
};
@group(0) @binding(0) var<uniform> u: Uniforms;

struct VertexOutput {
    @builtin(position) pos: vec4f,
    @location(0) color: vec3f,
};

@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    let res = 250u;
    let x = f32(idx % res) / f32(res) - 0.5;
    let y = f32(idx / res) / f32(res) - 0.5;
    
    let tau = 6.2831853071;
    let t = (y + 0.5) * u.freq + 5.0;
    
    let wave = sin(t * 0.4 - u.time) * cos(x * tau * log(t + 2.0));
    let z = wave * 0.2;

    let view_dist = y + 1.4;
    let px = (x * 2.8) / view_dist;
    let py = (z * 5.0) / view_dist;

    var out: VertexOutput;
    out.pos = vec4f(px, py, y, 1.0);
    
    // P3 High-Saturation Colors
    let zero = smoothstep(0.03, 0.0, abs(z));
    out.color = mix(vec3f(0.0, 1.2, 1.0), vec3f(1.8, 1.2, 0.0), zero);
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    return vec4f(in.color * (1.0 / in.pos.w), 1.0);
}
`;
