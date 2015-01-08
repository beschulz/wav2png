wav2png
=======

[![Build Status](https://travis-ci.org/beschulz/wav2png.svg?branch=master)](https://travis-ci.org/beschulz/wav2png)

Author: Benjamin Schulz

email: beschulz[the a with the circle]betabugs.de  

License: GPL, v2 or later
	http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

create waveform pngs out of audio files.

Note: if youre looking for a way to generate waveformjs.org compatible json out of audio file, take a look at
[wav2json](https://github.com/beschulz/wav2json)

all audio formats, that are readable by libsndfile are supported.

The generated images look like the ones you can find on soundcloud.
They are ment to be used in webpages. They are not anti-aliased, but look really good when scaled down by the browser.

# Examples

On Mac OS X, there is a little application you can drop your audio files into. The images are either saved in the same directory as the audio file (autosave checkbox) or can be drag'n'droped into photoshop (or what ever). You can select foreground and background colors and set the dimensions of the generated image.

![DropletScreenshot](https://github.com/beschulz/wav2png/raw/master/examples/DropletScreenshot.png)

The binary can be downloaded [here](http://goo.gl/8UCKd).

You can supply a foreground and background color in rgba

	wav2png --foreground-color=ffb400aa --background-color=2e4562ff -o ../examples/example0.png ../music.wav
![example0](https://github.com/beschulz/wav2png/raw/master/examples/example0.png)

Transparency works nicely with html.

	wav2png --foreground-color=ffb400aa --background-color=2e4562ff -o ../examples/example1.png ../baked.wav
![example1](https://github.com/beschulz/wav2png/raw/master/examples/example1.png)

You can make the waveform fully transparent and stick another image in the background. 
Hint: try a gradient here.

	wav2png --foreground-color=00000000 --background-color=2e4562ff -o ../examples/example2.png ../music.wav
![example2](https://github.com/beschulz/wav2png/raw/master/examples/example2.png)

Or you can just use the foreground color and make the background transparent

	wav2png --foreground-color=2e4562ff --background-color=00000000 -o ../examples/example3.png ../music.wav
![example3](https://github.com/beschulz/wav2png/raw/master/examples/example3.png)

wav2png also works nicely with short samples…

	wav2png --foreground-color=2e4562ff --background-color=00000000 -o ../examples/example4.png ../short.wav
![example4](https://github.com/beschulz/wav2png/raw/master/examples/example4.png)

…and with very short samples. In this case, the audio file had only 12 samples

	wav2png --foreground-color=2e4562ff --background-color=00000000 -o ../examples/example5.png ../sine.wav
![example5](https://github.com/beschulz/wav2png/raw/master/examples/example5.png)

You can also use decibels as units.

	wav2png --foreground-color=ffb400aa --background-color=2e4562ff -d -o ../examples/example7.png ../music.wav
![example7](https://github.com/beschulz/wav2png/raw/master/examples/example7.png)

And you can supply min and max values when using the db scale to better visualize you content.

	wav2png --foreground-color=ffb400aa --background-color=2e4562ff -d --db-min -40 --db-max 3 -o ../examples/example8.png ../music.wav
![example8](https://github.com/beschulz/wav2png/raw/master/examples/example8.png)

In case, that you need co convert mp3s, you can pipe the output of sox like this:

    sox ../song.mp3 -c 1 -t wav - | wav2png -o ../examples/example9.png /dev/stdin
![example9](https://github.com/beschulz/wav2png/raw/master/examples/example9.png)

Note, that you can easily adjust the color of the waveform by changing the background behind it.
But you can also specify colors via --foreground-color and --background-color.

Also gradient overlays and backgrounds look nice.

# Color format

The colors specified via --foreground-color and --background-color are in the following format:

`rrggbbaa` in hexadecimal form.

a few examples:

`ff0000ff` = 100% red, 100% opaque
`ff00007f` = 100% red, ~50% opaque
`00ff00ff` = 100% green, 100% opaque
`0000ffff` = 100% blue, 100% opaque

I usually pick a color in photoshop or some online color picker, copy the values for rrggbb and calculate (or guess) the alpha I want.


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

##
	if you're using a Linux distributing, that supports apt-get or you're on OSX and have homebrew installed, you
	might want to try:

```bash
	cd build
	make install_dependencies
```

## On Linux (Ubuntu, Debian)

### install dependencies
```apt-get install make g++ libsndfile1-dev libpng++-dev libpng12-dev libboost-program-options-dev```

### Build
```bash
	cd build
	make all
```

## On Max OS

### install dependencies
* Get the Xcode command line tools
	* Starting with Xcode 4.3, Apple does not install command line tools by default anymore, so after Xcode installation, go to Preferences > Downloads > Components > Command Line Tools and click Install. You can also directly [download Command Line Tools](https://developer.apple.com/downloads) for Xcode without getting Xcode.
* [Install homebrew](https://github.com/mxcl/homebrew/wiki/installation)
* install libsndfile: in the shell: brew install libsndfile
* install [png++](http://savannah.nongnu.org/projects/pngpp/)
	* put the headers in dependencies/include, so that dependencies/include/png++/png.hpp can be found.
	* alternatively, you can install it anywhere else, where the compiler can find it.

### Build
* either open build/macosx/wav2png.xcodeproj in Xcode to build it there, or
* in the shell: cd build && make all

## On CentOS
```bash
	yum install libsndfile-devel boost-devel libpng-devel gcc-c++
	cd wav2png/dependencies/include
	wget http://download.savannah.gnu.org/releases/pngpp/png++-0.2.5.tar.gz
	tar zxvf png++-0.2.5.tar.gz && rm png++-0.2.5.tar.gz
	mv png++-0.2.5 png++
	cd ../../build
	make # (you might have to remove -Werror in the Makefile)
```

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
