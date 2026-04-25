// main.js - The GPU Driver
import { shaderSource } from './shaders.js';

async function init() {
    if (!navigator.gpu) return;
    const adapter = await navigator.gpu.requestAdapter({ powerPreference: "high-performance" });
    const device = await adapter.requestDevice();
    const canvas = document.getElementById('gpuCanvas');
    const context = canvas.getContext('webgpu');
    
    // P3 GAMUT & RETINA RESOLUTION
    const format = 'display-p3';
    const dpr = window.devicePixelRatio || 1;
    canvas.width = window.innerWidth * dpr;
    canvas.height = window.innerHeight * dpr;

    context.configure({ device, format, colorSpace: 'display-p3', alphaMode: 'premultiplied' });

    const pipeline = device.createRenderPipeline({
        layout: 'auto',
        vertex: { 
            module: device.createShaderModule({ code: shaderSource }), 
            entryPoint: 'vs_main' 
        },
        fragment: { 
            module: device.createShaderModule({ code: shaderSource }), 
            entryPoint: 'fs_main', 
            targets: [{ format }] 
        },
        primitive: { topology: 'point-list' }
    });

    const uniformBuffer = device.createBuffer({
        size: 32,
        usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST,
    });

    const bindGroup = device.createBindGroup({
        layout: pipeline.getBindGroupLayout(0),
        entries: [{ binding: 0, resource: { buffer: uniformBuffer } }]
    });

    document.getElementById('status').innerText = "GPU: P3_ACTIVE | 120Hz";

    function render(time) {
        const uniformData = new Float32Array([time * 0.001, 45.0, 1.0, 0, 0, 0, 0, 0]);
        device.queue.writeBuffer(uniformBuffer, 0, uniformData);

        const encoder = device.createCommandEncoder();
        const pass = encoder.beginRenderPass({
            colorAttachments: [{
                view: context.getCurrentTexture().createView(),
                clearValue: [0, 0.01, 0.01, 1],
                loadOp: 'clear', storeOp: 'store'
            }]
        });

        pass.setPipeline(pipeline);
        pass.setBindGroup(0, bindGroup);
        pass.draw(250 * 250);
        pass.end();
        device.queue.submit([encoder.finish()]);
        requestAnimationFrame(render);
    }

    requestAnimationFrame(render);
}

init();
