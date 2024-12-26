
all:
	g++ -DPART1 -std=c++14 main.cpp -pthread -o syncQueue
	g++ -DPART2 -std=c++14 main.cpp -pthread -o asyncQueue
	g++ -DPART3 -std=c++14 main.cpp -pthread -o queue8API

test:
	g++ -DPART1 -std=c++14 -fsanitize=thread main.cpp -pthread -o syncQueue_test
	g++ -DPART2 -std=c++14 -fsanitize=thread main.cpp -pthread -o asyncQueue_test
	g++ -DPART3 -std=c++14 -fsanitize=thread main.cpp -pthread -o queue8API_test

clean:
	rm -f *~ syncQueue syncQueue_test asyncQueue asyncQueue_test queue8API queue8API_test


