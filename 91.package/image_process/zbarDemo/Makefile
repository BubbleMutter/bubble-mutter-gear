DEMO_CFLAGS=`pkg-config --cflags Magick++`
DEMO_LIBS=`pkg-config --libs Magick++` -lzbar

demo: demo.cpp barCode.cpp
	g++ ${DEMO_CFLAGS} -o $@ $^ ${DEMO_LIBS}

clean:
	rm -f demo
