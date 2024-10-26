
all:
	g++ -std=c++14 -DCPP_MUTEX  main.cpp -pthread -o cpp_mutex
	g++ -std=c++14 -DFILTER_MUTEX main.cpp -pthread -o filter_mutex
	g++ -std=c++14 -DBAKERY_MUTEX main.cpp -pthread -o bakery_mutex

clean:
	rm -f cpp_mutex filter_mutex bakery_mutex *~

test:
	g++ -std=c++14 -DCPP_MUTEX  -fsanitize=thread main.cpp -pthread -o cpp_mutex_test
	g++ -std=c++14 -DFILTER_MUTEX main.cpp -fsanitize=thread -pthread -o filter_mutex_test
	g++ -std=c++14 -DBAKERY_MUTEX main.cpp -fsanitize=thread -pthread -o bakery_mutex_test

