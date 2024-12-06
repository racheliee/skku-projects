echo "relaxed sb ========================" > part2.txt
echo "first run: " >> part2.txt
../part2/relaxed_sb >> part2.txt
echo "second run: " >> part2.txt
../part2/relaxed_sb >> part2.txt
echo "third run: " >> part2.txt
../part2/relaxed_sb >> part2.txt

echo "sync sb ========================" >> part2.txt
echo "first run: " >> part2.txt
../part2/sync_sb >> part2.txt
echo "second run: " >> part2.txt
../part2/sync_sb >> part2.txt
echo "third run: " >> part2.txt
../part2/sync_sb >> part2.txt

echo "disallowed sb ========================" >> part2.txt
echo "first run: " >> part2.txt
../part2/sc_sb >> part2.txt
echo "second run: " >> part2.txt
../part2/sc_sb >> part2.txt
echo "third run: " >> part2.txt
../part2/sc_sb >> part2.txt