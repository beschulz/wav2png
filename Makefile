
version.hpp: Makefile version.txt
	echo "#ifndef VERSION_HPP__" > version.hpp
	echo "#define VERSION_HPP__" >> version.hpp
	echo "namespace {" >> version.hpp
	echo "	namespace version {" >> version.hpp
	echo "		static const std::string date=\"`date`\";" >> version.hpp
	echo "		static const std::string version=\"`cat version.txt`\";" >> version.hpp
	echo "	}; /* namespace version */" >> version.hpp
	echo "} /* anonymous namespace */" >> version.hpp
	echo "#endif /* VERSION_HPP__ */" >> version.hpp

wav2png: Makefile wav2png.cpp options.hpp version.hpp
	g++ -O3 -Wall -Werror wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile -lboost_program_options

all: wav2png

clean:
	rm -f wav2png
	rm -f gmon.out
	rm -f version.hpp

test: wav2png
	time -v ./wav2png \
		--foreground-color=ffb40044 \
		--background-color=143c57ff \
		/media/psf/Home/Downloads/Nexidia\ Workbench\ SDK/Test/baked.wav

profile:
	g++ -O3 wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile -g -pg
	./wav2png baked.wav
	gprof ./wav2png|less

examples: wav2png README.md
	cat README.md|grep wav2png|grep examples/|grep -v github|while read line; do $$line; done
