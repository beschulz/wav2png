#!/usr/bin/python

import os
import subprocess
import platform

# do not change that !
MY_DIR = os.path.abspath(os.path.dirname(__file__))

# path to the directory that will be scanned
INPUT_DIRECTORY  = '/Volumes/Macintosh HD/Data/Samples/free/'

# path to the output directory. Will be created if it does not exist.
OUTPUT_DIRECTORY = '/Users/foo/Desktop/wav2png_batch_output'

# uncomment the following line, if you don't want the files in the same directory as this script!
OUTPUT_DIRECTORY = os.path.join(MY_DIR, 'wav2png_batch_output')

# wav2png settings
WAV2PNG_WIDTH            = 1800        # width of generated image
WAV2PNG_HEIGHT           = 280         # height of generated image
WAV2PNG_BACKGROUND_COLOR = 'efefefff'  # color of background in hex rgba
WAV2PNG_FOREGROUND_COLOR = '00000000'  # color of background in hex rgba
WAV2PNG_DB_SCALE         = False       # use logarithmic (e.g. decibel) scale instead of linear scale
WAV2PNG_DB_MIN           = -48         # minimum value of the signal in dB, that will be visible in the waveform.
WAV2PNG_DB_MAX           = 0           # maximum value of the signal in dB, that will be visible in the waveform.
WAV2PNG_LINE_ONLY        = False       # do a line only (no fill)


""" **************** you should not need to edit settings below this line **************** """

# append bin directory for the current platform to the search path
os.environ["PATH"] += os.pathsep + os.path.join(MY_DIR, 'bin', platform.system())

# if you have installed the wav2png executable somewhere else, uncomment the following line:
#os.environ["PATH"] += os.pathsep + '<insert-path-to-wav2png-here>'

def wav2png(input_path, output_path):
	print('converting: %s => %s' % (input_path, output_path))

	cmd = [
		'wav2png',
		'--width', str(WAV2PNG_WIDTH),
		'--height', str(WAV2PNG_HEIGHT),
		'--background-color', WAV2PNG_BACKGROUND_COLOR,
		'--foreground-color', WAV2PNG_FOREGROUND_COLOR,
		'--output', output_path,
		input_path
	]

	if WAV2PNG_DB_SCALE:
		cmd += [
			'--db-scale',
			'--db-min', WAV2PNG_DB_MIN,
			'--db-max', WAV2PNG_DB_MAX,
		]

	if WAV2PNG_LINE_ONLY:
		cmd += ['--line-only']

	try:
		subprocess.check_call(cmd)
	except subprocess.CalledProcessError:
		print('******************** Failed to convert %s' % input_path)
		print('******************** See output for details')

def main():
	input_directory_name = os.path.basename(os.path.normpath(INPUT_DIRECTORY))

	for root, dirs, files in os.walk(INPUT_DIRECTORY):
		for file in files:
			if os.path.splitext(file)[-1].lower() != '.wav':
				continue

			input_path = os.path.join(root, file)
			png_file_name = os.path.splitext(file)[0] + '.png'
			output_dir = os.path.join(OUTPUT_DIRECTORY, input_directory_name, os.path.relpath(root, INPUT_DIRECTORY))
			output_path = os.path.join(output_dir, png_file_name)

			# only convert, if output file exists or if input file is newer that output file
			if not os.path.exists(output_path) or os.path.getmtime(input_path) > os.path.getmtime(output_path):
				if not os.path.exists(output_dir):
					os.makedirs(output_dir)
				wav2png(input_path, output_path)
			else:
				print('skipping %s (already up to date)' % input_path)


if __name__ == '__main__':
	main()
