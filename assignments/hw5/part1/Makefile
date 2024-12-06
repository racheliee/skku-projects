
all:
	g++ -std=c++17 -O3 -DSPAWNJOIN main.cpp -pthread -o sjbarrier
	g++ -std=c++17 -O3 -DBARRIER -DSR main.cpp -pthread -o srbarrier
	g++ -std=c++17 -O3 -DBARRIER -DSRO main.cpp -pthread -o srobarrier

clean:
	rm -f sjbarrier sjbarrier_test srbarrier srbarrier_test srobarrier srobarrier_test *~

test:
	g++ -std=c++17 -O3 -DSPAWNJOIN main.cpp -fsanitize=thread -pthread -o sjbarrier_test
	g++ -std=c++17 -O3 -DBARRIER -DSR main.cpp -fsanitize=thread -pthread -o srbarrier_test
	g++ -std=c++17 -O3 -DBARRIER -DSRO main.cpp -fsanitize=thread -pthread -o srobarrier_test

