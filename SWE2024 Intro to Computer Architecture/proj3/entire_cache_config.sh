#!/bin/bash

# applications=("CCa")
applications=("CCa" "CCe" "CCh" "CCh_st" "CCl" "CCm" "CF1" "CRd" "CRf" "CRm" "CS1" "CS3" "DP1d" "DP1f" "DPcvt" "DPT" "DPTd" "ED1" "EF" "EI" "EM1" "EM5" "M_Dyn" "MC" "MCS" "MD" "MI" "MIM" "MIM2" "MIP" "ML2" "ML2_BW_ld" "ML2_BW_ldst" "ML2_BW_st" "ML2_st" "MM" "MM_st" "STc" "STL2" "STL2b")
cache_configurations=("1:256:64" "2:128:64" "4:64:64" "8:32:64" "16:16:64" "32:8:64" "64:4:64" "128:2:64" "256:1:64")

for app in "${applications[@]}"; do
    echo "$app"
    for config in "${cache_configurations[@]}"; do
        echo "$config"
        output=$(./spike --dc="$config" ~swe3005/2023f/proj3/pk ~swe3005/2023f/proj3/bench/"$app".elf)
        last_line=$(echo "$output" | tail -n 1)
        miss_rate=$(echo "$last_line" | grep -oE '[0-9]+\.[0-9]+')
        miss_rate_float=$(echo "$miss_rate" | bc -l)
        echo "$miss_rate_float"
        echo "$output"
    done
done
