INCLUDES=\
	-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/usr/X11/include/\
        -I./dependencies/include
        
LD_FLAGS=\
	`libpng-config --ldflags`\
    -lsndfile\

UNAME := $(shell uname)
BINARY=./bin/$(UNAME)/wav2png

ifeq ($(UNAME), Linux)
LD_PLATFORM_FLAGS=$(LD_FLAGS) -lboost_program_options
endif
ifeq ($(UNAME), Darwin)
LD_PLATFORM_FLAGS=$(LD_FLAGS) -lboost_program_options-mt
endif

all: $(BINARY)

./src/version.hpp: Makefile version.txt
	echo "#ifndef VERSION_HPP__" > ./src/version.hpp
	echo "#define VERSION_HPP__" >> ./src/version.hpp
	echo "namespace {" >> ./src/version.hpp
	echo "	namespace version {" >> ./src/version.hpp
	echo "		static const std::string date=\"`date`\";" >> ./src/version.hpp
	echo "		static const std::string version=\"`cat version.txt`\";" >> ./src/version.hpp
	echo "	}; /* namespace version */" >> ./src/version.hpp
	echo "} /* anonymous namespace */" >> ./src/version.hpp
	echo "#endif /* VERSION_HPP__ */" >> ./src/version.hpp

$(BINARY): Makefile ./src/*.cpp ./src/*.hpp
	mkdir -p `dirname $(BINARY)`
	g++ -O3 -Wall -Werror $(INCLUDES) $(LD_PLATFORM_FLAGS) ./src/*.cpp -o $(BINARY)

clean:
	rm -f $(BINARY)
	rm -f gmon.out
	rm -f ./src/version.hpp

profile:
	g++ -static -O3 -Wall -Werror $(INCLUDES) $(LD_PLATFORM_FLAGS) ./src/wav2png.cpp -o $(BINARY)_profile -g -pg
	$(BINARY)_profile baked.wav
	gprof $(BINARY)_profile|less

examples/example1.png: $(BINARY) README.md
	cat README.md|grep wav2png|grep examples/|grep -v github|while read line; do ./bin/$(UNAME)/$$line; done

examples: examples/example1.png
