/*
*   CSE 113 Assignment 4 Part 3
*   WebGPU
*/

import spriteWGSL from './sprite.js';
import updateSpriteWGSL from './updateSprite.js';

export const main = async() => {
    
    // Number of particles
    const NUMPARTICLES = 1024;
    
    // Set up canvas and other devices
    const canvas = document.getElementById('canvas');
    const adapter = await navigator.gpu.requestAdapter();       
    const device = await adapter.requestDevice();

    const context = canvas.getContext('webgpu');
    const format = 'bgra8unorm';

    context.configure({
        device: device,
        format: format,
        alphaMode: "opaque",
    });

    // Read from sprite.wgsl and create a pipeline for rendering
    const spriteShaderModule = device.createShaderModule({ code: spriteWGSL });
    const renderPipeline = device.createRenderPipeline({
        layout: 'auto',
        vertex: {
            module: spriteShaderModule,
            entryPoint: 'vert_main',
            buffers: [
                {
                    arrayStride: 2 * 4,
                    attributes: [
                        {
                            shaderLocation: 0,
                            offset: 0,
                            format: 'float32x2',
                        }
                    ]
                }
            ]
        },
        fragment: {
            module: spriteShaderModule,
            entryPoint: 'frag_main',
            targets: [{ format: format }],
        },
        primitive: { // Chooses which type of shape 
            topology: 'point-list'
        },
    })

    // Read from updateSprite.wgsl and create a pipeline for computing
    const computePipeline = device.createComputePipeline({
        layout: 'auto',
        compute: {
            module: device.createShaderModule({
                code: updateSpriteWGSL,
            }),
            entryPoint: 'main',
        }
    })

    // Create an array to store each particles' position value
    const particlesData = new Float32Array(NUMPARTICLES * 2);
    
    // Go through all particles then declare their initial position in canvas
    for (let i = 0; i < NUMPARTICLES; ++i) {
        // Get random position of each particle
        // Values are between -1 to 1
        particlesData[2 * i] = 2 * (Math.random() - 0.5); // X position
        particlesData[2 * i + 1] = 2 * (Math.random() - 0.5); // Y position
    }

    // Create a buffer and bind group objects
    const particleBuffers = new Array(2);
    const particleBindGroups = new Array(2);

    // Create a buffer and map memory
    for (let i = 0; i < 2; i++) {
        particleBuffers[i] = device.createBuffer({
            size: particlesData.byteLength,
            usage: GPUBufferUsage.VERTEX | GPUBufferUsage.STORAGE,
            mappedAtCreation: true,
        })
        new Float32Array(particleBuffers[i].getMappedRange()).set(particlesData);
        particleBuffers[i].unmap();
    }

    // Create a bind group
    for (let i = 0; i < 2; i++) {
        particleBindGroups[i] = device.createBindGroup({
            layout: computePipeline.getBindGroupLayout(0),
            entries: [
                {
                    binding: 0,
                    resource: {
                        buffer: particleBuffers[i],
                        offset: 0,
                        size: particlesData.byteLength
                    }
                },
                {
                    binding: 1,
                    resource: {
                        buffer: particleBuffers[(i + 1) % 2],
                        offset: 0,
                        size: particlesData.byteLength
                    }
                },
            ]
        })
    }


    // Variables for performance measurement (fps)
    var updatePerformance = true;
    var currentTime, previousTime;
    currentTime = previousTime = performance.now();
    var totalFramePerSecond = 0;
    var frameCounter = 0;

    let t = 0;
    function frame() {
        const textureView = context.getCurrentTexture().createView();
        const renderPassDescriptor = {
            colorAttachments: [
                {
                    view: textureView,
                    clearValue: {r: 0.0, g: 0.0, b: 0.0, a: 1.0},
                    loadOp: "clear", //background color
                    storeOp: "store"
                }
            ]
        }
            
        const commandEncoder = device.createCommandEncoder();
        {
            // Computation
            const passEncoder = commandEncoder.beginComputePass();
            passEncoder.setPipeline(computePipeline);
            passEncoder.setBindGroup(0, particleBindGroups[t % 2]);
            passEncoder.dispatchWorkgroups(Math.ceil(NUMPARTICLES / 64));
            //passEncoder.endPass();
            passEncoder.end();
        }
        {
            // Rendering
            const passEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);
            passEncoder.setPipeline(renderPipeline);
            passEncoder.setVertexBuffer(0, particleBuffers[(t+1)%2]);
            passEncoder.draw(NUMPARTICLES);
            //passEncoder.endPass();    
            passEncoder.end();  
        }
        // Finished rendering
        device.queue.submit([commandEncoder.finish()]); 
        ++t;
        
        // Measure performance
        currentTime = performance.now();
        var elapsedTime = currentTime - previousTime;
        previousTime = currentTime;
        var framePerSecond = Math.round(1 / (elapsedTime / 1000));
        totalFramePerSecond += framePerSecond;
        frameCounter++;
            
        if(updatePerformance) {
            updatePerformance = false;

            let averageFramePerSecond = Math.round(totalFramePerSecond / frameCounter);
            
            frameCounter = 0;
            totalFramePerSecond = 0;

            document.getElementById("fps").innerHTML = `FPS:  ${averageFramePerSecond}`;

            setTimeout(() => {
                updatePerformance = true;
            }, 50); // update FPS every 50ms
        }

        requestAnimationFrame(frame);
    }
    requestAnimationFrame(frame);
}
main();
