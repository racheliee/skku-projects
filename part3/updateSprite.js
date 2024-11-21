const updateSpriteWGSL = `
    struct Particle {
        pos : vec2<f32>
    };

    struct Particles {
        particles : array<Particle>
    };

    @binding(0) @group(0) var<storage, read_write> particlesA : Particles;
    @binding(1) @group(0) var<storage, read_write> particlesB : Particles;

    @compute @workgroup_size(64)
    fn main(@builtin(global_invocation_id) GlobalInvocationID : vec3<u32>) {
        var index : u32 = GlobalInvocationID.x;

        // Get position of current particle
        var vPos = particlesA.particles[index].pos;
        
        // Variables for finding the nearest neighbor
        var nearestDistance : f32 = 1e10; // Large initial value
        var nearestNeighbor : vec2<f32> = vPos;

        // Iterate over all particles to find the nearest neighbor
        for (var i : u32 = 0; i < arrayLength(&particlesA.particles); i++) {
            if (i == index) {
                continue; // Skip self
            }

            let neighborPos = particlesA.particles[i].pos;
            let dx = neighborPos.x - vPos.x;
            let dy = neighborPos.y - vPos.y;
            let distance = sqrt(dx * dx + dy * dy);

            // Skip particles that are too close (clustered)
            if (distance < 0.1) {
                continue;
            }

            // Update the nearest neighbor if a closer one is found
            if (distance < nearestDistance) {
                nearestDistance = distance;
                nearestNeighbor = neighborPos;
            }
        }

        // Move toward the nearest neighbor
        let dx = nearestNeighbor.x - vPos.x;
        let dy = nearestNeighbor.y - vPos.y;

        // Normalize the movement vector to a step size of 0.01
        if (nearestDistance > 0.1) {
            let stepSize : f32 = 0.01;
            let moveX = dx / nearestDistance * stepSize;
            let moveY = dy / nearestDistance * stepSize;

            vPos.x += moveX;
            vPos.y += moveY;
        }

        // Clamp positions to the range [-1, 1]
        vPos.x = clamp(vPos.x, -1.0, 1.0);
        vPos.y = clamp(vPos.y, -1.0, 1.0);


        // Write new particle data
        particlesB.particles[index].pos = vPos;
    }
`;

export default updateSpriteWGSL;



// ADD YOUR COMPUTATION HERE
// --------------------------------
    
// Example Computation (DELETE THIS)
// if (index % 4 == 0) {
//     vPos.x = vPos.x + 0.001;
//     vPos.y = vPos.y + 0.001;
// } else if (index % 4 == 1) {
//     vPos.x = vPos.x + 0.001;
//     vPos.y = vPos.y - 0.001;
// } else if (index % 4 == 2) {
//     vPos.x = vPos.x - 0.001;
//     vPos.y = vPos.y + 0.001;
// } else {
//     vPos.x = vPos.x - 0.001;
//     vPos.y = vPos.y - 0.001;
// }
// if (vPos.x < -1.0) { vPos.x = 1.0; }
// if (vPos.x > 1.0) { vPos.x = -1.0; }
// if (vPos.y < -1.0) { vPos.y = 1.0; }
// if (vPos.y > 1.0) { vPos.y = -1.0; }

// --------------------------------