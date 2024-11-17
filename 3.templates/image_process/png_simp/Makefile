CXXFLAGS += $(shell pkg-config --cflags libpng)
CXXFLAGS += $(shell pkg-config --cflags opencv)

LDFLAGS += $(shell pkg-config --libs libpng)
LDFLAGS += $(shell pkg-config --libs opencv)

png_simp: png_simp.cc
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

clean:
	rm -f png_simp
