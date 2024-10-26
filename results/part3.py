import pandas as pd

# Data for the three runs
data = {
    "coarse_lock_stack": {
        "run1": {"operations": 375277, "pops": 62714, "pushes": 61638, "peeks": 250925},
        "run2": {"operations": 471689, "pops": 79201, "pushes": 78401, "peeks": 314087},
        "run3": {"operations": 453770, "pops": 76239, "pushes": 75424, "peeks": 302107}
    },
    "rw_lock_stack": {
        "run1": {"operations": 419576, "pops": 38887, "pushes": 37888, "peeks": 342801},
        "run2": {"operations": 427906, "pops": 40401, "pushes": 39601, "peeks": 347904},
        "run3": {"operations": 418923, "pops": 36802, "pushes": 36018, "peeks": 346103}
    },
    "swaptop_stack": {
        "run1": {"operations": 409196, "pops": 14902, "pushes": 14350, "peeks": 336252, "swaptops": 43692},
        "run2": {"operations": 402732, "pops": 7994, "pushes": 8731, "peeks": 355993, "swaptops": 30014},
        "run3": {"operations": 390225, "pops": 12792, "pushes": 11510, "peeks": 328514, "swaptops": 37409}
    }
}

# Convert data into DataFrames for each stack
coarse_lock_df = pd.DataFrame(data["coarse_lock_stack"]).T
rw_lock_df = pd.DataFrame(data["rw_lock_stack"]).T
swaptop_stack_df = pd.DataFrame(data["swaptop_stack"]).T

# Calculate the average of each column (operations, pops, pushes, peeks, etc.)
coarse_lock_avg = coarse_lock_df.mean()
rw_lock_avg = rw_lock_df.mean()
swaptop_stack_avg = swaptop_stack_df.mean()

# Combine the results into a single DataFrame
averages_df = pd.DataFrame({
    "coarse_lock_avg": coarse_lock_avg,
    "rw_lock_avg": rw_lock_avg,
    "swaptop_stack_avg": swaptop_stack_avg
})

import ace_tools as tools; tools.display_dataframe_to_user(name="Average Results of Operations", dataframe=averages_df)
