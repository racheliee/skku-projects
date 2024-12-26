
all:
	g++ -std=c++17 -O3 -DSCDEKKERS  main.cpp -pthread -o scdekkers
	g++ -std=c++17 -O3 -DRDEKKERS   main.cpp -pthread -o rdekkers
	g++ -std=c++17 -O3 -DTSODEKKERS main.cpp -pthread -o tsodekkers

clean:
	rm -f scdekker scdekkers_test rdekkers tsodekkers *~

test:
	g++ -std=c++17 -O3 -DSCDEKKERS -fsanitize=thread main.cpp -pthread -o scdekkers_test
