CXXFLAGS += `pkg-config --cflags libpng` `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs libpng` `pkg-config --libs opencv`

png_example: png_example.cpp
	${CXX} -o $@ ${CXXFLAGS} $^ ${LDFLAGS}
clean:
	rm -f png_example
