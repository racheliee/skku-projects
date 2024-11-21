// Perform computation when receive data from main.js
self.onmessage = function (event) {

    // Unpack data received from main.js
    var NUMPARTICLES = event.data.NUMPARTICLES;
    var particlesComputeData = new Float32Array(event.data.particlesComputeBuffer);
    var particlesRenderData = new Float32Array(event.data.particlesRenderBuffer);

    for (let i = 0; i < NUMPARTICLES; i++) {

        /* ADD YOUR COMPUTATION HERE */

        // Example Computation (DELETE THIS)
        // particlesRenderData[2 * i] = particlesComputeData[2 * i] + (Math.random() * 2 - 1)
        // particlesRenderData[2 * i + 1] = particlesComputeData[2 * i + 1] + (Math.random() * 2 - 1)

        // Current particle's position
        const x1 = particlesComputeData[2 * i];
        const y1 = particlesComputeData[2 * i + 1];

        let minDistance = Infinity;
        let nearestNeighbor = -1;

        // Find the nearest neighbor
        for (let j = 0; j < NUMPARTICLES; j++) {
            if (i === j) continue; // Skip self

            const x2 = particlesComputeData[2 * j];
            const y2 = particlesComputeData[2 * j + 1];

            // Calculate Euclidean distance
            const distance = Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));

            // Skip if the particle is part of the cluster
            if (distance < 3) continue;

            // Update nearest neighbor
            if (distance < minDistance) {
                minDistance = distance;
                nearestNeighbor = j;
            }
        }

        // Move towards the nearest neighbor
        if (nearestNeighbor !== -1) {
            const x2 = particlesComputeData[2 * nearestNeighbor];
            const y2 = particlesComputeData[2 * nearestNeighbor + 1];

            // Update x and y by moving one step closer to the nearest neighbor
            const dx = x2 > x1 ? 1 : x2 < x1 ? -1 : 0;
            const dy = y2 > y1 ? 1 : y2 < y1 ? -1 : 0;

            particlesRenderData[2 * i] = x1 + dx;
            particlesRenderData[2 * i + 1] = y1 + dy;
        } else {
            // If no neighbor, stay in the same position
            particlesRenderData[2 * i] = x1;
            particlesRenderData[2 * i + 1] = y1;
        }
    }


    // Send back new data back to main.js
    postMessage(null);
}