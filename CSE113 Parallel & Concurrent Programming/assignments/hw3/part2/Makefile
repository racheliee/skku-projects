
all:
	g++ -DPART1 -std=c++14 main.cpp -pthread -o static
	g++ -DPART2 -std=c++14 main.cpp -pthread -o global
	g++ -DPART3 -std=c++14 main.cpp -pthread -o stealing
	g++ -DPART4 -std=c++14 main.cpp -pthread -o stealing32

test:
	g++ -DPART1 -std=c++14 -fsanitize=thread main.cpp -pthread -o static_test
	g++ -DPART2 -std=c++14 -fsanitize=thread main.cpp -pthread -o global_test
	g++ -DPART3 -std=c++14 -fsanitize=thread main.cpp -pthread -o stealing_test
	g++ -DPART4 -std=c++14 -fsanitize=thread main.cpp -pthread -o stealing32_test

clean:
	rm -f *~ static static_test global global_test stealing stealing_test stealing32 stealing32_test
