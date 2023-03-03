
TARGET =translite

DEBUGOP = -O0

PKGS= --static 

CFLAGS = $(DEBUGOP) -Wall  $(shell pkg-config --cflags $(PKGS)) -I. -DUNICODE

CXXFLAGS = $(DEBUGOP) -Wall  $(shell pkg-config --cflags $(PKGS)) -fmessage-length=0

LIBS = -static $(shell pkg-config --libs $(PKGS)) -lm -municode

CPPSRCSCAN := $(wildcard ./**/*.cpp) $(wildcard *.cpp)
CPPSRCS := $(filter %.cpp,$(CPPSRCSCAN))
CPPOBJS := $(patsubst %.cpp,obj/%.o,$(CPPSRCS))

CSRCSCAN := $(wildcard ./**/*.c) $(wildcard *.c)
CSRCS := $(filter %.c,$(CSRCSCAN))
COBJS := $(patsubst %.c,obj/%.o,$(CSRCS))

OBJSCAN_PRE1 := $(wildcard ./obj/**/*.o) $(wildcard ./obj/*.o) $(wildcard ./obj/**/**/*.o)
OBJSCAN := $(TARGET) $(OBJSCAN_PRE1)

$(TARGET):	$(COBJS) $(CPPOBJS)
	g++ -o $(TARGET) $(COBJS) $(CPPOBJS) $(LIBS)

all:	$(TARGET)

$(CPPOBJS):	obj/%.o: %.cpp
	$(shell sh.exe -c "mkdir -p ./$(dir $@)")
	g++ -g -O0 -Wall $(CFLAGS) -c $< -o $@ 

$(COBJS):	obj/%.o: %.c
	$(shell sh.exe -c "mkdir -p ./$(dir $@)")
	gcc -g -O0 -Wall $(CFLAGS) -c $< -o $@ 


clean:
	$(foreach objFile,$(OBJSCAN),$(shell sh.exe -c "rm $(objFile)"))
