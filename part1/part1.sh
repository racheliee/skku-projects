echo "sjbarrier ========================" > part1.txt
echo "1 core: " >> part1.txt
./sjbarrier 1 >> part1.txt
echo "8 cores: " >> part1.txt
./sjbarrier 8 >> part1.txt
echo "16 cores: " >> part1.txt
./sjbarrier 16 >> part1.txt

echo "srbarrier ========================" >> part1.txt
echo "1 core: " >> part1.txt
./srbarrier 1 >> part1.txt
echo "8 cores: " >> part1.txt
./srbarrier 8 >> part1.txt
echo "16 cores: " >> part1.txt
./srbarrier 16 >> part1.txt

echo "srobarrier ========================" >> part1.txt
echo "1 core: " >> part1.txt
./srobarrier 1 >> part1.txt
echo "8 cores: " >> part1.txt
./srobarrier 8 >> part1.txt
echo "16 cores: " >> part1.txt
./srobarrier 16 >> part1.txt