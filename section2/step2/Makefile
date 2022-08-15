dep=-Wall -lglfw -lGL -lGLU -lglut -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lrt -ldl -lXinerama -lXcursor

cherno:
	g++ apps/main2.cpp -o test ${dep}

uniform:
	g++ apps/uniform.cpp -o uniform ${dep}


clean:
	rm -f test uniform