echo "sjbarrier ========================" > part1.txt
echo "1 core: " >> part1.txt
../part1/sjbarrier 1 >> part1.txt
echo "8 cores: " >> part1.txt
../part1/sjbarrier 8 >> part1.txt
echo "16 cores: " >> part1.txt
../part1/sjbarrier 16 >> part1.txt

echo "srbarrier ========================" >> part1.txt
echo "1 core: " >> part1.txt
../part1/srbarrier 1 >> part1.txt
echo "8 cores: " >> part1.txt
../part1/srbarrier 8 >> part1.txt
echo "16 cores: " >> part1.txt
../part1/srbarrier 16 >> part1.txt

echo "srobarrier ========================" >> part1.txt
echo "1 core: " >> part1.txt
../part1/srobarrier 1 >> part1.txt
echo "8 cores: " >> part1.txt
../part1/srobarrier 8 >> part1.txt
echo "16 cores: " >> part1.txt
../part1/srobarrier 16 >> part1.txt