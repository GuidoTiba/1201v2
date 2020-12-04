target = TP1

extension = cpp

CXXSTD = c++11
CXXFLAGS =  -Wall -pedantic 

LD = $(CXX)

fuentes ?= $(wildcard *.$(extension))

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)

all: $(target)

o_files = $(patsubst %.$(extension),%.o,$(fuentes))

$(target): $(o_files)
	$(LD) $(o_files) -o $(target) $(LDFLAGS)

clean:
	$(RM) $(o_files) $(target)
	
valgrind: 
	valgrind --leak-check=full --track-origins=yes ./TP1 

txtvalgrind:
	valgrind --leak-check=full ./TP1 -d 3  -i entrada_cmd.txt -o salida_cmd.txt 
	
test1:
	./TP0 -i test1.txt -d 3
test2:
	./TP0 -i test2.txt -d 3
test3:
	./TP0 -i test3.txt -d 3
test4:
	./TP0 -i test4.txt -d 3
test5:
	./TP0 -i test5.txt -d 3
test6:
	./TP0 -i test6.txt -d 3
test7:
	./TP0 -i test7.txt -d 3
