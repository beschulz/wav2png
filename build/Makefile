######

LD_FLAGS=\
	`libpng-config --ldflags`\
    -lsndfile\

UNAME := $(shell uname)
BINARY=../bin/$(UNAME)/wav2png
SRC=../src

ifeq ($(UNAME), Linux)
LD_PLATFORM_FLAGS=$(LD_FLAGS) -lboost_program_options
CC=g++
endif
ifeq ($(UNAME), Darwin)
CC=clang++
LD_PLATFORM_FLAGS=$(LD_FLAGS) -lboost_program_options-mt
INCLUDES=\
	-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/usr/X11/include/\
    -I../dependencies/include
endif

all: $(BINARY)

$(SRC)/version.hpp: Makefile version.txt
	echo "#ifndef VERSION_HPP__" > $(SRC)/version.hpp
	echo "#define VERSION_HPP__" >> $(SRC)/version.hpp
	echo "namespace {" >> $(SRC)/version.hpp
	echo "	namespace version {" >> $(SRC)/version.hpp
	echo "		static const std::string date=\"`date`\";" >> $(SRC)/version.hpp
	echo "		static const std::string platform=\"$(UNAME)\";" >> $(SRC)/version.hpp
	echo "		static const std::string version=\"`cat version.txt`\";" >> $(SRC)/version.hpp
	echo "	}; /* namespace version */" >> $(SRC)/version.hpp
	echo "} /* anonymous namespace */" >> $(SRC)/version.hpp
	echo "#endif /* VERSION_HPP__ */" >> $(SRC)/version.hpp

$(BINARY): $(SRC)/*.cpp $(SRC)/*.hpp $(SRC)/version.hpp
	mkdir -p `dirname $(BINARY)`
	$(CC) -O3 -Wall -Werror $(SRC)/*.cpp $(INCLUDES) $(LD_PLATFORM_FLAGS) -o $(BINARY)

clean:
	rm -f $(BINARY)
	rm -f gmon.out
	rm -f $(SRC)/version.hpp

profile:
	$(CC) -O3 -Wall -Werror $(SRC)/*.cpp $(INCLUDES) $(LD_PLATFORM_FLAGS) -o $(BINARY)_profile -g -pg
	$(BINARY)_profile baked.wav
	gprof $(BINARY)_profile|less

ESCAPED_BINARY=$(shell echo $(BINARY) | sed 's/\//\\\//g' )

../examples/example0.png: $(BINARY) ../README.md
	#cat ../README.md|grep wav2png|grep examples/|grep -v github|grep -v sox|while read line; do cd ..; time ./bin/$(UNAME)/$$line; cd build; done
	cat ../README.md|grep wav2png|grep examples/|grep -v github|while read line; do eval `echo $$line|sed "s/wav2png/$(ESCAPED_BINARY)/g"`; done

examples: ../examples/example0.png

../icon.wav: Makefile
	sox -n ../icon.wav synth 0.5 sine 10.1

ICON_COLORS=--foreground-color=ffb400ff --background-color=2e4562ff
../icons/icon_512.png: ../icon.wav $(BINARY)
	$(BINARY) $(ICON_COLORS) -w2048 -h2048 -o ../icons/icon_2048.png ../icon.wav
	$(BINARY) $(ICON_COLORS) -w1024 -h1024 -o ../icons/icon_1024.png ../icon.wav
	$(BINARY) $(ICON_COLORS) -w512 -h512 -o ../icons/icon_512.png ../icon.wav
	$(BINARY) $(ICON_COLORS) -w256 -h256 -o ../icons/icon_256.png ../icon.wav
	$(BINARY) $(ICON_COLORS) -w128 -h128 -o ../icons/icon_128.png ../icon.wav
	$(BINARY) $(ICON_COLORS) -w64  -h64  -o ../icons/icon_64.png  ../icon.wav
	$(BINARY) $(ICON_COLORS) -w32  -h32  -o ../icons/icon_32.png  ../icon.wav
	$(BINARY) $(ICON_COLORS) -w16  -h16  -o ../icons/icon_16.png  ../icon.wav

icons: ../icons/icon_512.png
