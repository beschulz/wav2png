# wav2png

Author: Benjamin Schulz

email: beschulz[the a with the circle]betabugs.de  

License: GPL

create waveform pngs out of audio files.

all audio formats, that are readable by libsndfile are supported.

The generated images look like the ones you can find on soundcloud.
They are ment to be used in webpages. They are not anti-aliased, but look really good when scaled down by the browser.

# Examples

You can supply a foreground and background color in rgba

	./wav2png --foreground-color=ffb400aa --background-color=2e4562ff -o ./examples/example0.png music.wav
![example0](http://goo.gl/DNSQf)

Transparency works nicely with html.

	./wav2png --foreground-color=ffb400aa --background-color=2e4562ff -o ./examples/example1.png baked.wav
![example1](https://github.com/beschulz/wav2png/raw/master/examples/example1.png)

You can make the waveform fully transparent and stick another image in the background. 
Hint: try a gradient here.

	./wav2png --foreground-color=00000000 --background-color=2e4562ff -o ./examples/example2.png baked.wav
![example2](https://github.com/beschulz/wav2png/raw/master/examples/example2.png)

Or you can just use the foreground color and make the background transparent

	./wav2png --foreground-color=2e4562ff --background-color=00000000 -o ./examples/example3.png baked.wav
![example3](https://github.com/beschulz/wav2png/raw/master/examples/example3.png)

wav2png also works nicely with short samples…

	./wav2png --foreground-color=2e4562ff --background-color=00000000 -o ./examples/example4.png short.wav
![example4](https://github.com/beschulz/wav2png/raw/master/examples/example4.png)

…and with very short samples. In this case, the audio file had only 12 samples

	./wav2png --foreground-color=2e4562ff --background-color=00000000 -o ./examples/example5.png sine.wav
![example5](https://github.com/beschulz/wav2png/raw/master/examples/example5.png)

You can also use decibels as units.

	./wav2png --foreground-color=ffb400aa --background-color=2e4562ff -d -o ./examples/example7.png music.wav
![example7](https://github.com/beschulz/wav2png/raw/master/examples/example7.png)

And you can supply min and max values when using the db scale to better visualize you content.

	./wav2png --foreground-color=ffb400aa --background-color=2e4562ff -d --db-min -40 --db-max 3 -o ./examples/example8.png music.wav
![example8](https://github.com/beschulz/wav2png/raw/master/examples/example8.png)


Note, that you can easily adjust the color of the waveform by changing the background behind it.
But you can also specify colors via --foreground-color and --background-color.

Also gradient overlays and backgrounds look nice.

# Performance
Performance was one of the main goals, because all the other solutions I've tried where incredibly slow.

I'd say, wav2png is as fast as it gets :D

The only idea I've left to improve performance is to use multiple threads. I've decided not to do that because of the following reasons:

* At the current state, we're already way faster than a hard disk - and the data has to come from somewhere
* It would increase code complexity
* You can easily run multiple instances on different audio files in parallel
* I've better things to do :D

It takes about 1.8 seconds to convert a mono 16bit wav file of 2 hours and 11 minutes.

Thus on a 2.4 Ghz i5, the conversion rate was about 1 hour and 10 Minutes of audio per second and core (running inside a VM). Your Milage may vary.

If you have suggestions for performance improvements, please drop a line.

# Installation

## install dependencies
install dependencies: libsndfile++, libsndfile, libpng, libboost-program-options

on debian, ubuntu:

    apt-get install libsndfile1-dev libpng++-dev libpng12-dev libboost-program-options-dev

## Build
    make all

# Usage
    > wav2png --help

	wav2png version 0.6
	written by Benjamin Schulz (beschulz[the a with the circle]betabugs.de)

	usage: wav2png [options] input_file_name
	example: wav2png my_file.wav

	Allowed options:

	Generic options:
	  -v [ --version ]      print version string
	  --help                produce help message

	Configuration:
	  -w [ --width ] arg (=1800)            width of generated image
	  -h [ --height ] arg (=280)            height of generated image
	  -b [ --background-color ] arg (=efefefff)
	                                        color of background in hex rgba
	  -f [ --foreground-color ] arg (=00000000)
	                                        color of background in hex rgba
	  -o [ --output ] arg                   name of output file, defaults to <name 
	                                        of inputfile>.png
	  -c [ --config-file ] arg (=wav2png.cfg)
	                                        config file to use
	  -d [ --db-scale ]                     use logarithmic (e.g. decibel) scale 
	                                        instead of linear scale
	  --db-min arg (=-48)                   minimum value of the signal in dB, that
	                                        will be visible in the waveform
	  --db-max arg (=0)                     maximum value of the signal in dB, that
	                                        will be visible in the waveform. 
	                                        Usefull, if you now, that your signal 
	                                        peaks at a certain level.

# TODO

* add channel interpolation. Currently the max of all channels is used. for stereo signals, it would be cool to be able use the Mid, Side, Left or Right channel.

# Donations
If you find wav2png incredibly usefull nd use it a lot, feel free to make a small [donation via paypal](http://goo.gl/Ey2Bp).
While it is highly appreciated, it is absolutely not necessary to us the software.

If you find any issues, feel free to contact me.
and most important: enjoy and have fun :D
