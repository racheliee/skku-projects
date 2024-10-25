echo "8 CORES (no yield) =============================================" > part1-1.txt
echo "filter_mutex" >> part1-1.txt
./filter_mutex 8 >> part1-1.txt
echo "bakery_mutex" >> part1-1.txt
./bakery_mutex 8 >> part1-1.txt
echo "cpp_mutex" >> part1-1.txt
./cpp_mutex 8 >> part1-1.txt

echo "32 CORES (no yield) =============================================" >> part1-1.txt
echo "filter_mutex" >> part1-1.txt
./filter_mutex 32 >> part1-1.txt
echo "bakery_mutex" >> part1-1.txt
./bakery_mutex 32 >> part1-1.txt
echo "cpp_mutex" >> part1-1.txt
./cpp_mutex 32 >> part1-1.txt
