/*
*   CSE 113 Assignment 4 Part 2
*   Web Worker (Multithreaded)
*/

function main() {
    // Number of particles
    const NUMPARTICLES = 128;

    // Number of workers
    const NUMWORKERS = 4;

    // Get canvas and context 
    const canvas = document.getElementById('canvas');
    const context = canvas.getContext("2d");

    // Draw canvas background
    context.fillStyle = "black";
    context.fillRect(0, 0, canvas.width, canvas.height);

    // Create buffers for allowing shared memory
    var particlesComputeBuffer = new SharedArrayBuffer(Float32Array.BYTES_PER_ELEMENT * (NUMPARTICLES * 2));
    var particlesRenderBuffer = new SharedArrayBuffer(Float32Array.BYTES_PER_ELEMENT * (NUMPARTICLES * 2));

    // Create arrays to store each particles' position value
    var particlesComputeData = new Float32Array(particlesComputeBuffer); // Use for computation to avoid data race
    var particlesRenderData = new Float32Array(particlesRenderBuffer); // Use for writing new particles' location for rendering

    // Go through all particles then declare their initial position in canvas
    for (let i = 0; i < NUMPARTICLES; i++) {
        // Get random position of each particle
        var xPos = Math.random() * canvas.width;
        var yPos = Math.random() * canvas.height

        particlesComputeData[2 * i] = xPos; // X position
        particlesComputeData[2 * i + 1] = yPos; // Y position

        particlesRenderData[2 * i] = xPos; // X position
        particlesRenderData[2 * i + 1] = yPos; // Y position

        // Draw a particle into canvas
        context.fillStyle = "white";
        context.fillRect(particlesRenderData[2 * i], particlesRenderData[2 * i + 1], 3, 3);
    }

    // Variables for performance measurement (fps)
    var updatePerformance = true;
    var currentTime, previousTime;
    currentTime = previousTime = performance.now();
    var totalFramePerSecond = 0;
    var frameCounter = 0;

    // Initialize a worker
    // var worker = new Worker('./webWorker.js');

    // create array of workers
    const workers = Array.from({ length: NUMWORKERS }, () => new Worker('./webWorker.js'));
    const chunkSize = Math.ceil(NUMPARTICLES / NUMWORKERS);

    // Update Particles
    function frame() {
        let workersCompleted = 0;

        // Divide the work among workers
        workers.forEach((worker, workerIndex) => {
            const startIndex = workerIndex * chunkSize;
            const endIndex = Math.min(startIndex + chunkSize, NUMPARTICLES);

            const transferData = {
                NUMPARTICLES: NUMPARTICLES,
                STARTINDEX: startIndex,
                ENDINDEX: endIndex,
                particlesComputeBuffer: particlesComputeBuffer,
                particlesRenderBuffer: particlesRenderBuffer,
            };

            worker.postMessage(transferData);

            worker.onmessage = function () {
                workersCompleted++;

                // When all workers finish
                if (workersCompleted === NUMWORKERS) {
                    context.clearRect(0, 0, canvas.width, canvas.height);
                    context.fillStyle = "black";
                    context.fillRect(0, 0, canvas.width, canvas.height);

                    for (let i = 0; i < NUMPARTICLES; i++) {
                        context.fillStyle = "white";
                        context.fillRect(particlesRenderData[2 * i], particlesRenderData[2 * i + 1], 3, 3);

                        // Update compute buffer for the next frame
                        particlesComputeData[2 * i] = particlesRenderData[2 * i];
                        particlesComputeData[2 * i + 1] = particlesRenderData[2 * i + 1];
                    }

                    // Measure FPS
                    currentTime = performance.now();
                    const elapsedTime = currentTime - previousTime;
                    previousTime = currentTime;
                    const framePerSecond = Math.round(1 / (elapsedTime / 1000));
                    totalFramePerSecond += framePerSecond;
                    frameCounter++;

                    if (updatePerformance) {
                        updatePerformance = false;
                        const averageFramePerSecond = Math.round(totalFramePerSecond / frameCounter);
                        frameCounter = 0;
                        totalFramePerSecond = 0;

                        document.getElementById("fps").innerHTML = `FPS: ${averageFramePerSecond}`;

                        setTimeout(() => {
                            updatePerformance = true;
                        }, 100);
                    }

                    // Request the next frame
                    requestAnimationFrame(frame);
                }
            };
        });
    }
    requestAnimationFrame(frame);
}
main();
