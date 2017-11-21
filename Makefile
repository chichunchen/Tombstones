all:
	g++ -o foo1 tests/foo1.cpp -I.
	g++ -o foo2 tests/foo2.cpp -I.
	g++ -o foo3 tests/foo3.cpp -I.
	g++ -o foo4 tests/foo4.cpp -I.
	g++ -o foo5 tests/foo5.cpp -I.
	g++ -o foo6 tests/foo6.cpp -I.
	g++ -o foo7 tests/foo7.cpp -I.
	g++ -o foo8 tests/foo8.cpp -I.
	g++ -o foo9 tests/foo9.cpp -I.
	g++ -o foo10 tests/foo10.cpp -I.

raw:
	g++ -o foo1 tests-raw/foo1.cpp -I.
	g++ -o foo2 tests-raw/foo2.cpp -I.
	g++ -o foo3 tests-raw/foo3.cpp -I.
	g++ -o foo4 tests-raw/foo4.cpp -I.
	g++ -o foo5 tests-raw/foo5.cpp -I.
	g++ -o foo6 tests-raw/foo6.cpp -I.
	g++ -o foo7 tests-raw/foo7.cpp -I.
	g++ -o foo8 tests-raw/foo8.cpp -I.
	g++ -o foo9 tests-raw/foo9.cpp -I.
	g++ -o foo10 tests-raw/foo10.cpp -I.

clean:
	rm foo*
