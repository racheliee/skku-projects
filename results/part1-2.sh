echo "8 CORES (yield) =============================================" > part1-2.txt
echo "filter_mutex" >> part1-2.txt
./filter_mutex 8 yield >> part1-2.txt
echo "bakery_mutex" >> part1-2.txt
./bakery_mutex 8 yield >> part1-2.txt
echo "cpp_mutex" >> part1-2.txt
./cpp_mutex 8 yield >> part1-2.txt

echo "32 CORES (yield) =============================================" >> part1-2.txt
echo "filter_mutex" >> part1-2.txt
./filter_mutex 32 >> part1-2.txt
echo "bakery_mutex" >> part1-2.txt
./bakery_mutex 32 >> part1-2.txt
echo "cpp_mutex" >> part1-2.txt
./cpp_mutex 32 >> part1-2.txt