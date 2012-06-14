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

template <typename T>
const T& clamp(const T& x, const T& min, const T& max)
{
  return std::max(min, std::min(max, x));
}

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

  SndfileHandle wav(audio_file_name);

  if ( wav.frames() <= 0 )
  {
      cerr << "Error opening audio file '" << audio_file_name << "'" << endl;
      return 2;
  }

  int frames_per_pixel  = std::max<int>(1, wav.frames() / w);
  int samples_per_pixel = wav.channels() * frames_per_pixel;
  
  std::vector<float> block(samples_per_pixel);

  png::image< png::rgba_pixel > image(w,h);
  png::rgba_pixel color(0xef, 0xef, 0xef, 255);
  
  for (int x = 0; x < w; ++x)
  {
    sf_count_t n = wav.readf(&block[0], frames_per_pixel) * wav.channels();
    assert(n <= block.size());

    float min = 0.0f;
    float max = 0.0f;
    for (int i=0; i<n; i+=wav.channels()) // only left channel
    {
      min = std::min( min, block[i] );
      max = std::max( max, block[i] );
    }

    int y1 = clamp<int>((h+min*h)*0.5f, 0, h);
    assert(0 <= y1 && y1 <= h/2);

    int y2 = clamp<int>((h+max*h)*0.5f , 0, h);
    assert(h/2 <= y2 && y2 <= h);
    
    for(int y=0; y<y1;++y)
      image.set_pixel(x, y, color);

    for(int y = y2; y<h; ++y)
      image.set_pixel(x, y, color);
    
    if ( x%(w/100) == 0 )
    {
      cerr << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bconverting: " << 100*x/w << "%";
    }
  }
  
  cerr << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bconverting: 100%" << endl;

  image.write(audio_file_name + ".png");

  return 0;
}
