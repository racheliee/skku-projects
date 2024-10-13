# run part 1 of the homework and save the output to p1.txt

# run the python script
echo "UNROLL FACTOR 1 =============================================" > p1.txt
python3 part1/main.py 64 1 >> p1.txt
echo "UNROLL FACTOR 2 =============================================" >> p1.txt
python3 part1/main.py 64 2 >> p1.txt
echo "UNROLL FACTOR 4 =============================================" >> p1.txt
python3 part1/main.py 64 4 >> p1.txt
echo "UNROLL FACTOR 8 =============================================" >> p1.txt
python3 part1/main.py 64 8 >> p1.txt
echo "UNROLL FACTOR 16 =============================================" >> p1.txt
python3 part1/main.py 64 16 >> p1.txt
echo "UNROLL FACTOR 32 =============================================" >> p1.txt
python3 part1/main.py 64 32 >> p1.txt
echo "UNROLL FACTOR 64 =============================================" >> p1.txt
python3 part1/main.py 64 64 >> p1.txt