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

        // ADD YOUR COMPUTATION HERE
        // --------------------------------
            
        // Example Computation (DELETE THIS)
        if (index % 4 == 0) {
            vPos.x = vPos.x + 0.001;
            vPos.y = vPos.y + 0.001;
        } else if (index % 4 == 1) {
            vPos.x = vPos.x + 0.001;
            vPos.y = vPos.y - 0.001;
        } else if (index % 4 == 2) {
            vPos.x = vPos.x - 0.001;
            vPos.y = vPos.y + 0.001;
        } else {
            vPos.x = vPos.x - 0.001;
            vPos.y = vPos.y - 0.001;
        }
        if (vPos.x < -1.0) { vPos.x = 1.0; }
        if (vPos.x > 1.0) { vPos.x = -1.0; }
        if (vPos.y < -1.0) { vPos.y = 1.0; }
        if (vPos.y > 1.0) { vPos.y = -1.0; }

        // --------------------------------

        // Write new particle data
        particlesB.particles[index].pos = vPos;
    }
`;

export default updateSpriteWGSL;