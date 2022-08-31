dep=-Wall -lglfw -lGL -lGLU -lglut -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lrt -ldl -lXinerama -lXcursor


interpolation:
	g++ interpolation.cpp -o app ${dep}

index:
	g++ indexDraws.cpp -o app ${dep}
	

clean:
	rm -f app