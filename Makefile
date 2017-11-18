all:
	g++ -o foo1 tests/foo1.cpp -I.
	g++ -o foo2 tests/foo2.cpp -I.
	g++ -o foo3 tests/foo3.cpp -I.
	g++ -o foo4 tests/foo4.cpp -I.
	g++ -o foo5 tests/foo5.cpp -I.
	g++ -o foo6 tests/foo6.cpp -I.
	g++ -o foo7 tests/foo7.cpp -I.
	g++ -o foo8 tests/foo8.cpp -I.

clean:
	rm foo*
