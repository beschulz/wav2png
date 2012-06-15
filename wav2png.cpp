/*
  wav2png
  
  converts audiofiles that are readable via libsndfile into pngs containing their waveforms.
  usage: wav2png audio_file.wav
  
  build: make all

  dependencies: libsndfile++, libsndfile, libpng
  
  on debian, ubuntu: apt-get install libsndfile1-dev libpng++-dev libpng12-dev

  Author: Benjamin Schulz
  email: beschulz[the a with the circle]betabugs.de
  license: GPL
  
  If you find any issues, feel free to contact me.
  
  TODO:
    - add command line options for:
      - width
      - height
      - foreground color
      - background color
      - output file name
      
    - ensure, that unicode paths are working

  and most important: enjoy and have fun :D
*/

#include <sndfile.hh>
#include <png++/png.hpp>

#include <iostream>
#include <vector>
#include <iterator>

template <typename T>
const T& clamp(const T& x, const T& min, const T& max)
{
  return std::max(min, std::min(max, x));
}

template <typename T> struct sample_scale
{};

template <> struct sample_scale<short>
{
  static const unsigned short value = 1 << (sizeof(short)*8-1);
};

template <> struct sample_scale<float>
{
  static const float value = 1.0f;
};

int main(int argc, char** argv)
{
  using std::endl;
  using std::cout;
  using std::cerr;

  if (argc==1)
  {
    cout << "usage: " << argv[0] << " audio_file" << endl;
    return 1;
  }
  
  std::string audio_file_name(argv[1]);
  
  int w=1800;
  int h=280;

  // open sound file
  SndfileHandle wav(audio_file_name);

  // handle error
  if ( wav.error() )
  {
      cerr << "Error opening audio file '" << audio_file_name << "'" << endl;
      cerr << "Error was: '" << wav.strError() << "'" << endl; 
      return 2;
  }

  cerr << "length: " << wav.frames() / wav.samplerate() << " seconds" << endl;

  int frames_per_pixel  = std::max<int>(1, wav.frames() / w);
  int samples_per_pixel = wav.channels() * frames_per_pixel;

  // you can change it to float or short, short was much faster for me.
  typedef short sample_type;

  // buffer for samples from audio file
  std::vector<sample_type> block(samples_per_pixel);

  // create image
  png::image< png::rgba_pixel > image(w,h);
  png::rgba_pixel color(0xef, 0xef, 0xef, 255);
  
  /* the processing works like this:
    for each vertical pixel in the image (x), read frames_per_pixel frames from
    the audio file and find the min and max values.
  */
  for (int x = 0; x < w; ++x)
  {
    // read frames
    sf_count_t n = wav.readf(&block[0], frames_per_pixel) * wav.channels();
    assert(n <= (sf_count_t)block.size());

    // find min and max
    sample_type min(0);
    sample_type max(0);
    for (int i=0; i<n; i+=wav.channels()) // only left channel
    {
      min = std::min( min, block[i] );
      max = std::max( max, block[i] );
    }

    // compute "span" from top of image to min
    int y1 = clamp<int>((h+min*h/sample_scale<sample_type>::value)/2, 0, h);
    assert(0 <= y1 && y1 <= h/2);

    // compute "span" from max to bottom of image
    int y2 = clamp<int>((h+max*h/sample_scale<sample_type>::value)/2, 0, h);
    assert(h/2 <= y2 && y2 <= h);
    
    // fill span top to min
    for(int y=0; y<y1;++y)
      image.set_pixel(x, y, color);

    // fill span max to bottom
    for(int y = y2; y<h; ++y)
      image.set_pixel(x, y, color);
    
    // print progress
    if ( x%(w/100) == 0 )
    {
      cerr << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bconverting: " << 100*x/w << "%";
    }
  }
  
  cerr << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bconverting: 100%" << endl;

  // write image to disk
  image.write(audio_file_name + ".png");

  #ifdef __linux__
  // this prints info about memory usage, in my tests it was: VmRSS: 4320 kB
  /*
  {
    std::ifstream proc("/proc/self/status");
    std::copy(
      std::istreambuf_iterator<char>(proc),
      std::istreambuf_iterator<char>(),
      std::ostream_iterator<char>(std::cerr)
    );
  }*/
  #endif /* __linux__ */

  return 0;
}
