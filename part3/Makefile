
all:
	g++ -DCOARSE_LOCK main.cpp -pthread -o coarse_lock_stack
	g++ -DRW_LOCK -std=c++17 main.cpp -pthread -o rw_lock_stack
	g++ -DSWAPTOP -std=c++17 main.cpp -pthread -o swaptop_stack

clean:
	rm -f coarse_lock_stack rw_lock_stack swaptop_stack *~

test:
	g++ -DCOARSE_LOCK -fsanitize=thread main.cpp -pthread -o coarse_lock_stack_test
	g++ -DRW_LOCK -fsanitize=thread -std=c++17 main.cpp -pthread -o rw_lock_stack_test
	g++ -DSWAPTOP -fsanitize=thread -g -std=c++17 main.cpp -pthread -o swaptop_stack_test

sequential:
	g++ -DCOARSE_LOCK main_seq.cpp -pthread -o coarse_lock_stack_seq
	g++ -DRW_LOCK -std=c++17 main_seq.cpp -pthread -o rw_lock_stack_seq
	g++ -DSWAPTOP -std=c++17 main_seq.cpp -pthread -o swaptop_stack_seq