all:
	g++ -g -O3 *.cpp -c -std=c++11 -march=native -ljpeg -lm -L/usr/lib/x86_64-linux-gnu -ljpeg -lpthread
	g++ *.o -o raytracer -lpthread -ljpeg -lm -I -L/usr/lib/x86_64-linux-gnu -ljpeg

clean:
	rm -f *.o

