cppsrc = $(wildcard ./*.cpp) $(wildcard ../src/*.cpp) $(wildcard ../I2C/src/*.cpp) 
obj = $(cppsrc:.cpp=.o)
dep = $(obj:.o=.d)  # one dependency file for each source
CXXFLAGS = -Wall -I/usr/include -std=c++0x

LDFLAGS = -L/usr/lib -lpthread

test.exe: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

-include $(dep)   # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
%.d: %.cpp
	@$(CPP) $(CXXFLAGS) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) test

.PHONY: cleandep
cleandep:
	rm -f $(dep)

.PHONY: run
run: test.exe
	./test.exe
