While you can find binaries here, I am not sure, how usefull they'll be to you.
You do have to install the dependencies, no mater what.

On Linux:
	see README.md

	ldd bin/Linux/wav2png gives:
		linux-vdso.so.1 =>  (0x00007fff2a193000)
		libpng12.so.0 => /lib/libpng12.so.0 (0x00007fbb089c4000)
		libsndfile.so.1 => /usr/lib/libsndfile.so.1 (0x00007fbb0875f000)
		libboost_program_options.so.1.42.0 => /usr/lib/libboost_program_options.so.1.42.0 (0x00007fbb08504000)
		libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007fbb081f0000)
		libm.so.6 => /lib/libm.so.6 (0x00007fbb07f6e000)
		libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00007fbb07d57000)
		libc.so.6 => /lib/libc.so.6 (0x00007fbb079f5000)
		libz.so.1 => /usr/lib/libz.so.1 (0x00007fbb077de000)
		libFLAC.so.8 => /usr/lib/libFLAC.so.8 (0x00007fbb07592000)
		libvorbisenc.so.2 => /usr/lib/libvorbisenc.so.2 (0x00007fbb070c4000)
		libvorbis.so.0 => /usr/lib/libvorbis.so.0 (0x00007fbb06e98000)
		libogg.so.0 => /usr/lib/libogg.so.0 (0x00007fbb06c91000)
		librt.so.1 => /lib/librt.so.1 (0x00007fbb06a89000)
		libpthread.so.0 => /lib/libpthread.so.0 (0x00007fbb0686d000)
		/lib64/ld-linux-x86-64.so.2 (0x00007fbb08bf1000)

On Darwin (Mac OS X):
	architecture: x86_64 (non fat binary)

	otool -L bin/Darwin/wav2png  gives:
		/usr/X11/lib/libpng15.15.dylib (compatibility version 20.0.0, current version 20.0.0)
		/usr/local/lib/libsndfile.1.dylib (compatibility version 2.0.0, current version 2.25.0)
		/usr/local/lib/libboost_program_options-mt.dylib (compatibility version 0.0.0, current version 0.0.0)
		/usr/lib/libstdc++.6.dylib (compatibility version 7.0.0, current version 52.0.0)
		/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 159.1.0)

	I highly recomend homebrew: http://mxcl.github.com/homebrew/
	
	brew install libsndfile boost

	this should be enough.
