#include "wav2png.hpp"

#include <math.h>
#include <iostream>
#include <assert.h>
#include <sndfile.hh>
#include <png++/png.hpp>

/*
  clamp x into range [min...max]
*/
template <typename T>
const T& clamp(const T& x, const T& min, const T& max)
{
  return std::max(min, std::min(max, x));
}

/*
  metaprogramming functions to get value range of sample format T.
*/
template <typename T> struct sample_scale {};

template <> struct sample_scale<short>
{
  static const unsigned short value = 1 << (sizeof(short)*8-1);
};

template <> struct sample_scale<float>
{
  static const int value = 1;
};

/*
  conversion from and to dB
*/
float float2db(float x)
{
  x = fabs(x);

  if (x > 0.0f)
    return 20.0f * log10( x );
  else
    return -9999.9f;
}

float db2float(float x)
{
  return pow(10.0, x/20.f);
}

/*
  map value x in range [in_min...in_max] into range [out_min...out_max]
*/
float map2range(float x, float in_min, float in_max, float out_min, float out_max)
{
  return clamp<float>(
    out_min + (out_max-out_min)*(x-in_min)/(in_max-in_min),
    out_min,
    out_max
  );
}

/*
  compute the waveform of the supplied audio-file and store it into out_image.
*/
void compute_waveform(
  const SndfileHandle& wav,
  png::image< png::rgba_pixel >& out_image,
  const png::rgba_pixel& bg_color,
  const png::rgba_pixel& fg_color,
  bool use_db_scale,
  float db_min,
  float db_max,
  progress_callback_t progress_callback
)
{
  using std::size_t;
  using std::cerr;
  using std::endl;

  const unsigned h = out_image.get_height();

  // you can change it to float or short, short was much faster for me.
  typedef short sample_type;

  //there might not be enough samples, in this case, we're using a smaller image and scale it up later.
  png::image< png::rgba_pixel > small_image;
  bool not_enough_samples = wav.frames() < (sf_count_t)out_image.get_width();

  if (not_enough_samples)
    small_image = png::image< png::rgba_pixel >( wav.frames()>0?wav.frames():1, out_image.get_height() );

  png::image< png::rgba_pixel >& image = not_enough_samples?small_image:out_image;

  assert(image.get_width() > 0);

  int frames_per_pixel  = std::max<int>(1, wav.frames() / image.get_width());
  int samples_per_pixel = wav.channels() * frames_per_pixel;
  std::size_t progress_divisor = std::max<std::size_t>(1, image.get_width()/100);

  // temp buffer for samples from audio file
  std::vector<sample_type> block(samples_per_pixel);

  /*
    the processing works like this:
    for each vertical pixel in the image (x), read frames_per_pixel frames from
    the audio file and find the min and max values.
  */
  for (size_t x = 0; x < image.get_width(); ++x)
  {
    // read frames
    sf_count_t n = const_cast<SndfileHandle&>(wav).readf(&block[0], frames_per_pixel) * wav.channels();
    assert(n <= (sf_count_t)block.size());

    // find min and max
    sample_type min(0);
    sample_type max(0);
    for (int i=0; i<n; i+=1)//wav.channels()) // only left channel
    {
      min = std::min( min, block[i] );
      max = std::max( max, block[i] );
    }

    // compute "span" from top of image to min
    float y1_ = use_db_scale?
      h/2 - map2range( float2db(min / (float)sample_scale<sample_type>::value ), db_min, db_max, 0, h/2):
      map2range( min, -sample_scale<sample_type>::value, 0, 0, h/2);
    assert(0 <= y1_ && y1_ <= h/2);
    size_t y1 = (size_t)y1_;

    // compute "span" from max to bottom of image
    float y2_ = use_db_scale?
      h/2 + map2range( float2db(max / (float)sample_scale<sample_type>::value ), db_min, db_max, 0, h/2):
      map2range( max, 0, sample_scale<sample_type>::value, h/2, h);
    assert(h/2 <= y2_ && y2_ <= h);
    size_t y2 = (float)y2_;
    
    // fill span top to min
    for(size_t y=0; y<y1;++y)
      image.set_pixel(x, y, bg_color);

    // fill span min to max
    for(size_t y=y1; y<y2;++y)
      image.set_pixel(x, y, fg_color);

    // fill span max to bottom
    for(size_t y = y2; y<h; ++y)
      image.set_pixel(x, y, bg_color);
    
    // print progress
    if ( x%(progress_divisor) == 0 )
    {
      if ( progress_callback && !progress_callback( 100*x/image.get_width() ) )
          return;
    }
  }
  
    if ( progress_callback && !progress_callback( 100 ) )
        return;

  // upscale the generated image (nearest neighbour)
  if (not_enough_samples)
  {
    for (std::size_t y=0; y<out_image.get_height(); ++y)
      for(std::size_t x=0; x<out_image.get_width(); ++x)
      {
        std::size_t xx = x*small_image.get_width()/out_image.get_width();
        assert( xx < out_image.get_width() );
        out_image[y][x] = small_image[y][xx];
      }
  }
}
