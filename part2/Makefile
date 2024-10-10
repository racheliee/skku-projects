
all:
	g++ -std=c++14 -DDEFAULT_MUTEX main.cpp -pthread -o default_mutex
	g++ -std=c++14 -DFAIR_MUTEX main.cpp -pthread -o fair_mutex

clean:
	rm -f default_mutex fair_mutex *~

test:
	g++ -std=c++14 -DDEFAULT_MUTEX -fsanitize=thread main.cpp -pthread -o default_mutex_test
	g++ -std=c++14 -DFAIR_MUTEX main.cpp -fsanitize=thread -pthread -o fair_mutex_test

