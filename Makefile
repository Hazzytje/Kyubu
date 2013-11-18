LIBS			= -lGL -lGLEW -lglfw -lz -lX11 -lm -L/usr/lib64 -lstdc++

sourcefiles		:= $(wildcard *.cpp)
objfiles		:= $(patsubst %.cpp,%.o,$(sourcefiles))

all : makebin/kyubu

makebin/kyubu: $(objfiles)
	gcc -ggdb -std=c++11 $(LIBS) -o makebin/kyubu $(objfiles)

%.o : %.cpp
	gcc -ggdb -std=c++11  $(LIBS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o makebin/kyubu
