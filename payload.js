// payload.js - THE AGNOSTIC HOST DRIVER
export const payload = {
  id: "UNIVERSAL_GPU_EXECUTOR",
  js: async (p, state) => {
    // 1. Agnostic Buffer Creation
    const storageSize = p.bufferSize || 1024;
    const sBuf = state.device.createBuffer({
      size: storageSize,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_SRC | GPUBufferUsage.COPY_DST
    });
    
    // 2. Agnostic Pipeline Layout
    const bgl = state.device.createBindGroupLayout({
      entries: [{ binding: 0, visibility: GPUShaderStage.COMPUTE | GPUShaderStage.FRAGMENT, buffer: { type: 'storage' } }]
    });
    
    // 3. Template-Based Shader Injection
    const shaderModule = state.device.createShaderModule({
      code: p.wgsl // The math/logic comes entirely from the JSON
    });

    // 4. Execution Logic defined by JSON 'type'
    if (p.type === 'compute') {
      const pipeline = await state.device.createComputePipelineAsync({
        layout: state.device.createPipelineLayout({ bindGroupLayouts: [bgl] }),
        compute: { module: shaderModule, entryPoint: 'main' }
      });
      // ... dispatch logic based on p.dispatch
    } else if (p.type === 'render') {
      // ... render logic based on p.vertex/p.fragment
    }
    
    state.buffers[p.id] = sBuf;
  }
};
