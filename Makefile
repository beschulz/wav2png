
all: ./bin/wav2png

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

./bin/wav2png: Makefile ./src/wav2png.cpp ./src/options.hpp ./src/version.hpp
	g++ -O3 -Wall -Werror ./src/wav2png.cpp -o./bin/wav2png `libpng-config --ldflags` -lsndfile -lboost_program_options

clean:
	rm -f ./bin/wav2png
	rm -f gmon.out
	rm -f ./src/version.hpp

profile:
	g++ -O3 ./src/wav2png.cpp -o./bin/wav2png_profile `libpng-config --ldflags` -lsndfile -lboost_program_options -g -pg
	./bin/wav2png_profile baked.wav
	gprof ./bin/wav2png_profile|less

examples/example1.png: ./bin/wav2png README.md
	cat README.md|grep wav2png|grep examples/|grep -v github|while read line; do ./bin/$$line; done

examples: examples/example1.png