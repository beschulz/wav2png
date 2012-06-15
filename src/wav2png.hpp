#include <sndfile.hh>
#include <png++/png.hpp>

void compute_waveform(
  const SndfileHandle& wav,
  png::image< png::rgba_pixel >& out_image,
  const png::rgba_pixel& bg_color,
  const png::rgba_pixel& fg_color,
  bool use_db_scale,
  float db_min,
  float db_max
);
