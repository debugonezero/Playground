// PRIME_HUNT_V5 - DEBUGONEZERO RECOVERY
export const payload = {
  id: "PRIME_VIZ_RECOVERY",
  html: `
    <div id='host' style='height:100%; display:flex; flex-direction:column; background:#000;'>
      <div id='msg' style='padding:10px; color:#0f0; font-family:monospace; font-size:11px;'>● KERNEL_INIT</div>
      <canvas id='gpuCanvas' style='flex:1; width:100%; height:100%; image-rendering:pixelated;'></canvas>
    </div>
  `,
  js: `
    const lim = 1048576;
    const bgl = state.device.createBindGroupLayout({
      entries: [{ binding: 0, visibility: 20, buffer: { type: 'storage' } }]
    });
    const pl = state.device.createPipelineLayout({ bindGroupLayouts: [bgl] });
    const shader = \`
      @group(0) @binding(0) var<storage, read_write> s : array<atomic<u32>>;
      @vertex fn vs(@builtin(vertex_index) i: u32) -> @builtin(position) vec4<f32> {
        var p = array<vec2<f32>, 4>(vec2(-1,1), vec2(1,1), vec2(-1,-1), vec2(1,-1));
        return vec4<f32>(p[i], 0.0, 1.0);
      }
      @fragment fn fs(@builtin(position) pos: vec4<f32>) -> @location(0) vec4<f32> {
        let i = u32(pos.y) * 1024u + u32(pos.x);
        if ((atomicLoad(&s[i/32u]) >> (i%32u)) & 1u) == 0u {
          return vec4(0.0, 1.0, 0.5, 1.0);
        }
        return vec4(0.0, 0.05, 0.0, 1.0);
      }
    \`;
    const pipe = await state.device.createRenderPipelineAsync({
      layout: pl,
      vertex: { module: state.device.createShaderModule({ code: shader }), entryPoint: 'vs' },
      fragment: { module: state.device.createShaderModule({ code: shader }), entryPoint: 'fs', targets: [{ format: navigator.gpu.getPreferredCanvasFormat() }] },
      primitive: { topology: 'triangle-strip' }
    });
    const sBuf = state.device.createBuffer({ size: Math.ceil(lim/32)*4, usage: 136 });
    const bg = state.device.createBindGroup({ layout: bgl, entries: [{binding:0, resource:{buffer:sBuf}}] });
    
    function draw() {
      const enc = state.device.createCommandEncoder();
      const pass = enc.beginRenderPass({
        colorAttachments: [{ view: state.ctx.getCurrentTexture().createView(), loadOp: 'clear', storeOp: 'store', clearValue: [0,0,0,1] }]
      });
      pass.setPipeline(pipe); pass.setBindGroup(0, bg); pass.draw(4); pass.end();
      state.device.queue.submit([enc.finish()]);
      requestAnimationFrame(draw);
    }
    draw();
    document.getElementById('msg').innerText = '● VIZ_ACTIVE';
  `
};
