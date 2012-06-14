
wav2png: wav2png.cpp
	g++ -O3 wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile

all: wav2png

test: wav2png
	./wav2png baked.wav
	cp baked.wav.png /media/psf/Home/Downloads/Nexidia\ Workbench\ SDK/Test/baked.wav.png

profile:
	g++ -O3 wav2png.cpp -owav2png `libpng-config --ldflags` -lsndfile -g -pg
	./wav2png baked.wav
	gprof ./wav2png|less
