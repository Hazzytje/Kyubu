INCLUDES		= -I/home/h/Development/libs/freetype-gl/tmp
LIBS			= -lGL -lGLEW -lglfw -lz -lX11 -lm -L/usr/lib64 -lstdc++ -L/home/h/Development/libs/freetype-gl/tmp/ -lfreetypegl -lfreetype

sourcefiles		:= $(wildcard *.cpp)
objfiles		:= $(patsubst %.cpp,obj/%.o,$(sourcefiles))

all : bin/Kyubu

bin/Kyubu: $(objfiles)
	gcc -ggdb -std=c++11 $(INCLUDES) -o bin/Kyubu $(objfiles) $(LIBS)

obj/%.o : %.cpp
	gcc -ggdb -std=c++11 $(INCLUDES) $(LIBS) -c $< -o $@

.PHONY: clean

clean:
	rm -f obj/*.o bin/Kyubu
