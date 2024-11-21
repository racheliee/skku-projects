---
title: CSE113 Homework 4 Design Document
date: \today
geometry: margin=1in
papersize: letter
fontfamily: newtxtext, newtxmath
fontsize: 10pt
numbersections: true
autoEqnLabels: true
header-includes: |
  \usepackage{authblk}
  \author{Rachel (Younwoo) Park}
  \affil{Dept. of Systems Management Engineering\\Sungkyunkwan University}
  \usepackage{fancyhdr}
  \pagestyle{fancy}
  \fancyhead[L]{CSE 113 Homework 4 Design Document}
  \fancyhead[C]{}
  \fancyhead[R]{\thepage}
  \fancyfoot{}
  \usepackage{float}
  \let\origfigure\figure
  \let\endorigfigure\endfigure
  \renewenvironment{figure}[1][2] {\expandafter\origfigure\expandafter[H]} {
    \endorigfigure
  }
#   \usepackage[hangul, nonfrench, finemath]{kotex}
abstract: |
  This is the design document for the fourth homework assignment of CSE 113: Parallel and Concurrent Programming.
---

\tableofcontents

\newpage

# Part 1: Single threaded Web Worker

## **Objective**
Implement a nearest-neighbor attraction simulation using a single-threaded Web Worker. The goal is for particles to move towards their nearest neighbors based on Euclidean distance, clustering over time.

## **Overview**
The simulation computes pairwise distances between particles to determine the nearest neighbor for each particle. Each particle moves one step closer to its nearest neighbor in both the x and y directions. Particles that are too close to others (distance < 3) are considered clustered and do not influence movement.

## **Algorithm**
1. **Initialization**:
   - Unpack `NUMPARTICLES`, `particlesComputeBuffer`, and `particlesRenderBuffer`.
   - Parse the flat arrays into manageable structures for computation.

2. **Compute Nearest Neighbor**:
   - For each particle `i`:
     1. Extract the current position `(x1, y1)` of particle `i`.
     2. Initialize `minDistance` to infinity and `nearestNeighbor` to -1.
     3. Iterate over all other particles `j`:
        - Skip particle `i` itself.
        - Compute the Euclidean distance:  
          `distance = sqrt((x1 - x2)^2 + (y1 - y2)^2)`
        - Skip particle `j` if `distance < 3` (clustered).
        - Update `minDistance` and `nearestNeighbor` if `distance` is smaller than `minDistance`.
   - If a valid nearest neighbor is found:
     1. Move particle `i` one step closer to the nearest neighbor along both axes:
        - For x: `dx = +1 if x2 > x1, -1 if x2 < x1, 0 if x2 == x1`
        - For y: `dy = +1 if y2 > y1, -1 if y2 < y1, 0 if y2 == y1`
     2. Write the updated position `(x1 + dx, y1 + dy)` to `particlesRenderBuffer`.
   - If no valid neighbor is found:
     - Retain the original position `(x1, y1)` in `particlesRenderBuffer`.

3. **Post Data**:
   - Send the updated `particlesRenderBuffer` back to the main thread.

## **Pseudo Code**
```plaintext
Function NearestNeighborSimulation(NUMPARTICLES, particlesComputeBuffer, particlesRenderBuffer):

    For each particle i:
        - Get the current position (x1, y1) of particle i.
        - Initialize:
            minDistance = Infinity
            nearestNeighbor = -1
        
        For each particle j:
            - Skip if i == j
            - Get the position (x2, y2) of particle j
            - Compute distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
            - Skip if distance < 3
            - If distance < minDistance:
                - Update minDistance = distance
                - Update nearestNeighbor = j
        
        If nearestNeighbor is found:
            - Get position (x2, y2) of nearestNeighbor
            - Compute:
                dx = +1 if x2 > x1, -1 if x2 < x1, 0 if x2 == x1
                dy = +1 if y2 > y1, -1 if y2 < y1, 0 if y2 == y1
            - Update particle position to (x1 + dx, y1 + dy)
        
        Else:
            - Keep the original position (x1, y1)
        
        Write updated position to particlesRenderBuffer
```

# Part 2: Multi-threaded Web Worker
## **Objective**
Implement a nearest-neighbor attraction simulation using multiple web workers. Each worker processes a subset of particles (chunk) in parallel. The goal is to leverage multithreading to improve performance and maintain smooth animation.

## **Overview**
The simulation computes pairwise distances between particles to determine the nearest neighbor for each particle. Each particle moves one step closer to its nearest neighbor in both the x and y directions. Particles within a specified cluster distance (`< 3`) are ignored for nearest-neighbor calculations. The particle workload is divided into chunks, and each web worker processes one chunk.

## **Algorithm**
1. **Initialization**:
   - Unpack `NUMPARTICLES`, `STARTINDEX`, `ENDINDEX`, `particlesComputeBuffer`, and `particlesRenderBuffer`.
   - Parse the flat arrays into manageable structures for computation.

2. **Compute Nearest Neighbor for a Chunk**:
   - For each particle `i` in the assigned chunk (`STARTINDEX` to `ENDINDEX`):
     1. Extract the current position `(x1, y1)` of particle `i`.
     2. Initialize `minDistance` to infinity and `nearestNeighbor` to -1.
     3. Iterate over all particles `j`:
        - Skip particle `i` itself.
        - Compute the Euclidean distance:
          ```
          distance = sqrt((x1 - x2)^2 + (y1 - y2)^2)
          ```
        - Skip particle `j` if `distance < 3` (clustered).
        - Update `minDistance` and `nearestNeighbor` if `distance` is smaller than `minDistance`.
   - If a valid nearest neighbor is found:
     1. Move particle `i` one step closer to the nearest neighbor along both axes:
        - For x: `dx = +1 if x2 > x1, -1 if x2 < x1, 0 if x2 == x1`
        - For y: `dy = +1 if y2 > y1, -1 if y2 < y1, 0 if y2 == y1`
     2. Write the updated position `(x1 + dx, y1 + dy)` to `particlesRenderBuffer`.
   - If no valid neighbor is found:
     - Retain the original position `(x1, y1)` in `particlesRenderBuffer`.

3. **Post Data**:
   - Send the updated `particlesRenderBuffer` back to the main thread.

4. **Main Thread Synchronization**:
   - Divide `NUMPARTICLES` into chunks of size `chunkSize = Math.ceil(NUMPARTICLES / NUMWORKERS)`.
   - Create and assign workers to process these chunks.
   - Wait for all workers to finish before rendering the next frame.


### **Multithreading Logic**
- **Worker Initialization**:
  - Create `NUMWORKERS` web workers.
- **Chunk Assignment**:
  - Divide `NUMPARTICLES` into `NUMWORKERS` chunks based on indices.
  - Assign `STARTINDEX` and `ENDINDEX` for each chunk.
- **Synchronization**:
  - Use a counter (`workersCompleted`) to track the number of completed workers for each frame.
  - Render the next frame only after all workers have finished processing their chunks.


## **Pseudo Code**

### **Worker Logic**
```plaintext
Function WorkerLogic(NUMPARTICLES, STARTINDEX, ENDINDEX, 
                     particlesComputeBuffer, particlesRenderBuffer):

    For each particle i in range STARTINDEX to ENDINDEX:
        - Get the current position (x1, y1) of particle i.
        - Initialize:
            minDistance = Infinity
            nearestNeighbor = -1
        
        For each particle j in range 0 to NUMPARTICLES:
            - Skip if i == j
            - Get the position (x2, y2) of particle j
            - Compute distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
            - Skip if distance < 3
            - If distance < minDistance:
                - Update minDistance = distance
                - Update nearestNeighbor = j
        
        If nearestNeighbor is found:
            - Get position (x2, y2) of nearestNeighbor
            - Compute:
                dx = +1 if x2 > x1, -1 if x2 < x1, 0 if x2 == x1
                dy = +1 if y2 > y1, -1 if y2 < y1, 0 if y2 == y1
            - Update particle position to (x1 + dx, y1 + dy)
        
        Else:
            - Keep the original position (x1, y1)
        
        Write updated position to particlesRenderBuffer

    Post particlesRenderBuffer to main thread
```

### **Main Thread Logic**
```plaintext
Function MainLogic(NUMPARTICLES, NUMWORKERS):
    - Create NUMWORKERS workers
    - Divide NUMPARTICLES into chunks of size chunkSize = ceil(NUMPARTICLES / NUMWORKERS)
    
    Function Frame():
        Initialize workersCompleted = 0

        For each worker in NUMWORKERS:
            - Compute STARTINDEX and ENDINDEX for the worker's chunk
            - Send STARTINDEX, ENDINDEX, NUMPARTICLES, particlesComputeBuffer, 
              particlesRenderBuffer to the worker
            - On worker completion:
                Increment workersCompleted
                If workersCompleted == NUMWORKERS:
                    Render the next frame:
                        - Update canvas with particlesRenderBuffer
                        - Copy particlesRenderBuffer to particlesComputeBuffer 
                          for the next iteration
                    Call Frame() recursively
```


# Part 3: WebGPU

## **Objective**
Implement the nearest-neighbor attraction simulation using WebGPU to utilize GPU parallelism for performance improvement. Each particle is processed in parallel by a single GPU thread to compute its movement based on its nearest neighbor.

## **Overview**
The simulation finds the nearest neighbor for each particle and updates the particle's position by moving one step closer to its neighbor. The GPU kernel is launched with one thread per particle. Particle positions are stored in buffers (`particlesA` and `particlesB`), where `particlesA` contains the current positions and `particlesB` is used to store the updated positions.


## **Algorithm**
1. **Initialization**:
   - For each thread (corresponding to a particle with index `index`):
     - Load the particle's position `(x, y)` from `particlesA`.

2. **Find Nearest Neighbor**:
   - Initialize `nearestDistance` to a large value (`1e10`) and `nearestNeighbor` to the current particle's position.
   - Iterate over all particles:
     - Skip the current particle (`i == index`).
     - Compute the Euclidean distance between the current particle and another particle:
       ```
       distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
       ```
     - Skip particles that are part of a cluster (distance < 0.1).
     - Update `nearestDistance` and `nearestNeighbor` if a closer particle is found.

3. **Update Position**:
   - If a valid nearest neighbor is found, compute the normalized movement vector:
     ```
     dx = (nearestNeighbor.x - x) / nearestDistance * stepSize
     dy = (nearestNeighbor.y - y) / nearestDistance * stepSize
     ```
   - Add the movement vector to the current position to move the particle closer to its neighbor.
   - Clamp the position to ensure it remains within `[-1.0, 1.0]`:
     ```
     x = clamp(x, -1.0, 1.0)
     y = clamp(y, -1.0, 1.0)
     ```

4. **Write Output**:
   - Write the updated position to `particlesB`.


## **Pseudo Code**
```plaintext
Function ComputeNearestNeighbor(particlesA, particlesB):

    Get particle index from GlobalInvocationID.x

    If index is out of bounds, return.

    // Load particle position
    vPos = particlesA[index].pos

    // Initialize nearest neighbor variables
    nearestDistance = Infinity
    nearestNeighbor = vPos

    // Iterate over all particles
    For i = 0 to Number of Particles:
        If i == index:
            Continue

        // Get position of other particle
        neighborPos = particlesA[i].pos

        // Compute distance
        dx = neighborPos.x - vPos.x
        dy = neighborPos.y - vPos.y
        distance = sqrt(dx * dx + dy * dy)

        If distance < 0.1:
            Continue // Skip clustered particles

        If distance < nearestDistance:
            nearestDistance = distance
            nearestNeighbor = neighborPos

    // Update position if a valid neighbor is found
    If nearestDistance > 0.1:
        dx = (nearestNeighbor.x - vPos.x) / nearestDistance * stepSize
        dy = (nearestNeighbor.y - vPos.y) / nearestDistance * stepSize
        vPos.x += dx
        vPos.y += dy

    // Clamp position within [-1.0, 1.0]
    vPos.x = clamp(vPos.x, -1.0, 1.0)
    vPos.y = clamp(vPos.y, -1.0, 1.0)

    // Write updated position
    particlesB[index].pos = vPos
```