import matplotlib.pyplot as plt
import numpy as np

# Define data for 8-core and 32-core results with and without yield for filter, bakery, and cpp mutexes
# All local Data
data = {
    "8 Cores (No Yield)": {
        "filter_mutex": [66310, 66990, 68178, 68964, 67721, 66876, 70650, 69138],
        "bakery_mutex": [81448, 81032, 80925, 81022, 81079, 80760, 81375, 83516],
        # "cpp_mutex": [546936, 447715, 469227, 678030, 874949, 430885, 504988, 543007]
    },
    "32 Cores (No Yield)": {
        "filter_mutex": [5, 2, 2, 4, 2, 3, 3, 3, 2, 2, 3, 3, 2, 2, 2, 3, 2, 3, 2, 2, 3, 3, 2, 2, 2, 2, 2, 3, 2, 3, 2, 3],
        "bakery_mutex": [0] * 32,
        # "cpp_mutex": [446935, 304013, 89857, 127980, 48068, 120491, 47904, 51332, 38991, 66417, 70491, 95505, 78646, 93891, 66471, 45711,
        #               112565, 38085, 96102, 67690, 53538, 51773, 40480, 79944, 34949, 96890, 109642, 48982, 103913, 33720, 66645, 100077]
    },
    "8 Cores (Yield)": {
        "filter_mutex": [81319, 76791, 74230, 81791, 77622, 77105, 83215, 77253],
        "bakery_mutex": [44330, 44056, 43996, 43994, 43784, 44104, 43554, 43857],
        # "cpp_mutex": [660766, 662838, 610706, 723941, 581639, 665342, 891632, 568076]
    },
    "32 Cores (Yield)": {
        "filter_mutex": [4034, 3134, 2998, 3087, 3554, 3328, 3148, 3303, 3638, 3076, 3221, 3476, 3253, 3203, 3983, 3108,
                         2989, 3314, 3492, 3196, 3082, 4109, 3019, 3509, 3212, 3148, 3371, 3153, 4446, 3301, 3080, 3288],
        "bakery_mutex": [724] * 32,
        # "cpp_mutex": [130898, 82171, 134387, 115504, 102047, 82380, 89041, 93622, 129326, 91911, 83432, 153373, 86445, 139863, 154137, 85008,
        #               155062, 115353, 140088, 73261, 113972, 132449, 97299, 109813, 146891, 113245, 119291, 89629, 126116, 80646, 128853, 146060]
    }
}

# Plot histograms
fig, axes = plt.subplots(4, 3, figsize=(15, 12))
fig.suptitle("Thread Throughput Histograms for Mutex Implementations", fontsize=16)

for idx, (config, values) in enumerate(data.items()):
    row = idx // 3
    for col, (mutex_type, counts) in enumerate(values.items()):
        axes[row, col].hist(counts, bins=10, alpha=0.7, color="skyblue", edgecolor="black")
        axes[row, col].set_title(f"{config} - {mutex_type}")
        axes[row, col].set_xlabel("Throughput")
        axes[row, col].set_ylabel("Frequency")
        axes[row, col].grid(True)

fig.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
