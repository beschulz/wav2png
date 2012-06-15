
wav2png: wav2png.cpp
	g++ -O3 -Wall -Werror wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile

all: wav2png

clean:
	rm -f wav2png
	rm -f gmon.out

test: wav2png
	time -v ./wav2png /media/psf/Home/Downloads/Nexidia\ Workbench\ SDK/Test/baked.wav

profile:
	g++ -O3 wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile -g -pg
	./wav2png baked.wav
	gprof ./wav2png|less
