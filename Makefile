
wav2png: wav2png.cpp
	g++ -O3 wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile

all: wav2png

test: wav2png
	./wav2png /media/psf/Home/Downloads/Nexidia\ Workbench\ SDK/Test/test_short.wav

profile:
	g++ -O3 wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile -g -pg
	./wav2png baked.wav
	gprof ./wav2png|less
