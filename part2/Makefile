
all:
	g++ -std=c++17 -O3 -DRELAXED main.cpp -pthread -o relaxed_sb
	g++ -std=c++17 -O3 -DSYNC main.cpp -pthread -o sync_sb
	g++ -std=c++17 -O3 -DDISALLOWED main.cpp -pthread -o sc_sb

clean:
	rm -f relaxed_sb relaxed_sb_test sync_sb sync_sb_test sc_sb sc_sb_test *~

test:
	g++ -std=c++17 -O3 -DRELAXED main.cpp -fsanitize=thread -pthread -o relaxed_sb_test
	g++ -std=c++17 -O3 -DSYNC main.cpp -fsanitize=thread -pthread -o sync_sb_test
	g++ -std=c++17 -O3 -DDISALLOWED -fsanitize=thread main.cpp -pthread -o sc_sb_test
