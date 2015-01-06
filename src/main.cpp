#include <sndfile.hh>
#include <png++/png.hpp>

#include <iostream>
#include <vector>
#include <iterator>

#include "options.hpp"

#include "wav2png.hpp"

bool progress_callback(int percent)
{
    std::cerr << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bconverting: " << percent << "%";
    return true;
}


int main(int argc, char* argv[])
{
  Options options(argc, argv);

  using std::endl;
  using std::cout;
  using std::cerr;

  // open sound file
  SndfileHandle wav(options.input_file_name.c_str());

  // handle error
  if ( wav.error() )
  {
      cerr << "Error opening audio file '" << options.input_file_name << "'" << endl;
      cerr << "Error was: '" << wav.strError() << "'" << endl; 
      return 2;
  }

  //cerr << "length: " << wav.frames() / wav.samplerate() << " seconds" << endl;

  // create image
  png::image< png::rgba_pixel > image(options.width, options.height);

  //png::rgba_pixel bg_color(0xef, 0xef, 0xef, 255);
  compute_waveform(
    wav,
    image,
    options.background_color,
    options.foreground_color,
    options.use_db_scale,
    options.db_min,
    options.db_max,
    options.line_only,
    progress_callback
  );
  cerr << endl;

  // write image to disk
  image.write(options.output_file_name);

  return 0;
}
