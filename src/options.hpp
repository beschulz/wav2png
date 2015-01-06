#ifndef OPTIONS_HPP__
#define OPTIONS_HPP__

#include <boost/program_options.hpp>
#include <string>
#include <png++/png.hpp>
#include "./version.hpp"

struct Options
{
	Options(int argc, char* argv[])
	{
		namespace po = boost::program_options;

		po::options_description generic("Generic options");
			generic.add_options()
    		("version,v", "print version string")
    		("help", "produce help message")    
    	;

    	po::options_description config("Configuration");
		config.add_options()
    		("width,w", po::value<unsigned>(&width)->default_value(1800), 
          		"width of generated image")
    		("height,h", po::value<unsigned>(&height)->default_value(280), 
          		"height of generated image")
    		("background-color,b", po::value<std::string>(&background_color_string)->default_value("efefefff"), 
          		"color of background in hex rgba")
    		("foreground-color,f", po::value<std::string>(&foreground_color_string)->default_value("00000000"), 
          		"color of background in hex rgba")
    		("output,o", po::value<std::string>(&output_file_name)->default_value(""), 
          		"name of output file, defaults to <name of inputfile>.png")
    		("config-file,c", po::value<std::string>(&config_file_name)->default_value("wav2png.cfg"), 
          		"config file to use")
    		("db-scale,d", po::value(&use_db_scale)->zero_tokens()->default_value(false),
    			"use logarithmic (e.g. decibel) scale instead of linear scale")
    		("db-min", po::value(&db_min)->default_value(-48.0f),
    			"minimum value of the signal in dB, that will be visible in the waveform")
		    ("db-max", po::value(&db_max)->default_value(0.0f),
    			"maximum value of the signal in dB, that will be visible in the waveform. "
    			"Usefull, if you now, that your signal peaks at a certain level.")
    		("line-only,l", po::value(&line_only)->zero_tokens()->default_value(false), 
          		"do a line only (no fill)")
    	;

    	po::options_description hidden("Hidden options");
			hidden.add_options()
    		("input-file", po::value< std::string >(&input_file_name), "input file")
    	;
    
		po::positional_options_description p;
		p.add("input-file", -1);


		po::options_description cmdline_options;
		cmdline_options.add(generic).add(config).add(hidden);

		po::options_description config_file_options;
		config_file_options.add(config).add(hidden);

		po::options_description visible("Allowed options");
		visible.add(generic).add(config);

		po::variables_map vm;

		bool parse_error = false;
		try
		{
			po::store(po::command_line_parser(argc, argv).
		    	options(cmdline_options).positional(p).run(), vm);
			po::notify(vm);

			// try to read stuff from config file
			std::ifstream config_file(config_file_name.c_str()); //backward compatibility with older boost versions
			if (!config_file.good())
			{
				if ( config_file_name != "wav2png.cfg" )
				{
					std::cerr << "Error: " << "failed to read config file '" << config_file_name << "'!" << std::endl;
					parse_error = true;
				}
			}
			else
			{
				po::store(po::parse_config_file<char>(config_file,config_file_options),vm);
				po::notify(vm);
			}

			//but options from command line override the ones from the config file, so we just parse them again
			po::store(po::command_line_parser(argc, argv).
		    	options(cmdline_options).positional(p).run(), vm);
			po::notify(vm);
		} catch( std::exception& e )
		{
			std::cerr << "Error: " << e.what() << std::endl;
			parse_error = true;
		}

		//parse colors
		try
		{
			foreground_color = parse_color(foreground_color_string);
			background_color = parse_color(background_color_string);
		}
		catch(std::exception& e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
			parse_error = true;
		}

		if (input_file_name.empty())
		{
			std::cerr << "Error: no input file supplied." << std::endl;
			parse_error = true;
		}

		if (output_file_name.empty())
			output_file_name = input_file_name + ".png";

		if (width == 0)
		{
			std::cerr << "Error: width cannot be 0." << std::endl;
			parse_error = true;
		}

		if (height == 0)
		{
			std::cerr << "Error: height cannot be 0." << std::endl;
			parse_error = true;
		}

		if (parse_error || vm.count("help") )
		{
			std::cout << "wav2png version " << version::version << std::endl;
			std::cout << "written by Benjamin Schulz (beschulz[the a with the circle]betabugs.de)" << std::endl;
			std::cout << std::endl;
			std::cout << "usage: wav2png [options] input_file_name" << std::endl;
			std::cout << "example: wav2png my_file.wav" << std::endl;
			std::cout << std::endl;
			std::cout << visible << std::endl;
			exit(0);
		}

		if (vm.count("version"))
		{
			std::cout << "version: " << version::version << std::endl;
			std::cout << "build date: " << version::date << std::endl;
			exit(0);
		}
	}

	struct color_parse_error : std::exception
	{
		color_parse_error(const std::string& what) :message(what){}

		virtual const char* what () const throw ()
		{
			return message.c_str();
		}

		virtual ~color_parse_error () throw() {}

		private: std::string message;
	};

	static png::rgba_pixel parse_color(const std::string& str)
	{
		if ( str.length() != 8 )
			throw color_parse_error("supplied color does not have four components. try e.g. aabbccff");

		std::stringstream is(str);
		unsigned color;
		is >> std::hex >> color;

		if (!is.eof())
			throw color_parse_error("failed to parse color '" + str + "'. are all characters in range [0-9a-f]?");

		png::rgba_pixel ret(
			(color & 0xff000000) >> 24,
			(color & 0x00ff0000) >> 16,
			(color & 0x0000ff00) >>  8,
			(color & 0x000000ff) >>  0
		);

		return ret;
	}

	unsigned width;
	unsigned height;
	std::string background_color_string;
	std::string foreground_color_string;

	png::rgba_pixel background_color;
	png::rgba_pixel foreground_color;

	std::string output_file_name;
	std::string input_file_name;
	std::string config_file_name;

	bool use_db_scale;
	float db_min;
	float db_max;

	bool line_only;
};

#endif /* OPTIONS_HPP__ */
