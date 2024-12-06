import matplotlib.pyplot as plt
import numpy as np

# Data
categories = ["r0 == 1 && r1 == 1", "r0 == 1 && r1 == 0", "r0 == 0 && r1 == 1", "r0 == 0 && r1 == 0"]

# Local Data
local_relaxed = [
    [2, 31792, 230304, 46],
    [1, 31635, 230489, 19],
    [0, 32487, 229634, 23]
]
local_sync = [
    [0, 251136, 7156, 3852],
    [0, 242707, 12331, 7106],
    [1, 250953, 8643, 2547]
]
local_sc = [
    [544, 252829, 8771, 0],
    [1346, 238664, 22134, 0],
    [4, 240488, 21652, 0]
]

# Server Data
server_relaxed = [
    [0, 493, 65042, 1],
    [3, 851, 64682, 0],
    [0, 393, 65143, 0]
]
server_sync = [
    [1094, 63582, 708, 152],
    [579, 63877, 970, 110],
    [518, 64223, 680, 115]
]
server_sc = [
    [109, 63623, 1804, 0],
    [106, 63741, 1689, 0],
    [92, 63952, 1492, 0]
]

# Function to plot histograms
def plot_histogram(data, title, filename):
    x = np.arange(len(categories))
    bar_width = 0.25
    
    # Plotting the data
    fig, ax = plt.subplots(figsize=(10, 6))
    for i, run in enumerate(data):
        ax.bar(x + i * bar_width, run, bar_width, label=f"Run {i+1}")
    
    # Formatting the chart
    ax.set_title(title)
    ax.set_xlabel("Categories")
    ax.set_ylabel("Frequency")
    ax.set_xticks(x + bar_width)
    ax.set_xticklabels(categories, rotation=45, ha="right")
    ax.legend()
    
    # Save and show plot
    plt.tight_layout()
    plt.savefig(filename)
    plt.show()

# Plotting histograms
plot_histogram(local_relaxed, "Local: Relaxed Store Buffering", "local_relaxed.png")
plot_histogram(local_sync, "Local: Synchronized Store Buffering", "local_sync.png")
plot_histogram(local_sc, "Local: Disallowed Store Buffering", "local_sc.png")

plot_histogram(server_relaxed, "Server: Relaxed Store Buffering", "server_relaxed.png")
plot_histogram(server_sync, "Server: Synchronized Store Buffering", "server_sync.png")
plot_histogram(server_sc, "Server: Disallowed Store Buffering", "server_sc.png")
