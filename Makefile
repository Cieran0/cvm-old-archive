all:
	cd ../casmParser && $(MAKE) run
	./replaceBytes.py
	g++ -o main *.cpp -Iinclude/

cpp:
	g++ -o main *.cpp -Iinclude/