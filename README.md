# wav2png

    Author: Benjamin Schulz
    email: beschulz[the a with the circle]betabugs.de  
    license: GPL

    create waveform pngs out of audio files.

    all audio formats, that are readable by libsndfile are supported.


    The generated images look like the ones you can find on soundcloud.
    They are ment to be used in webpages. They are not anti-aliased, but look really good when scaled down by the browser.

# Installation

## install dependencies
install dependencies: libsndfile++, libsndfile, libpng

on debian, ubuntu:

    apt-get install libsndfile1-dev libpng++-dev libpng12-dev

## Build
    make all

# Usage
    wav2png audio_file.wav

an png-file called audio_file.wav.png is created.
      
If you find any issues, feel free to contact me.

# TODO
* add command line options for:
    * width
    * height
    * foreground color
    * background color
    * output file name

* ensure, that unicode paths are working
    
and most important: enjoy and have fun :D
