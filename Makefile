all:main.cpp utility.cpp 
	g++ -std=c++11 -o main main.cpp utility.cpp
	#c++ -std=c++11 -o main main.cpp utility.cpp
clean:
		rm -f main
