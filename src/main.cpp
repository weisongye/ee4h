/************************************************************************\
| Main code file for the EE4H Assignment (Playing card recognition)      |
|																		 |
| Authors: Yousef Amar and Chris Lewis									 |
| Last Modified: 31/01/2014												 |
|																		 |
| Dependencies: OpenCV-2.4.2											 |
|				- opencv_core242.dll									 |
|				- opencv_imgproc242.dll									 |
|				- opencv_highgui242.dll									 |
|				- tbb.dll (Built for Intel x64)							 |
\************************************************************************/

//Process includes only once
#ifndef INCLUDES_SEEN
#define INCLUDES_SEEN
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../include/preprocessing.h"
#include "../include/utils.h"

#include <iostream>
#include <string>
#include <strstream>
#endif

//Configuration
cv::Scalar 
	line_colour = cv::Scalar(0, 255, 0),
	text_colour = cv::Scalar(255, 255, 0)
;
float
	corner_h_perc = 0.36F,
	corner_v_perc = 0.25F
;

/**
  * Program entry point.
  *
  * Arguments
  *    int argc: Number of arguments
  * char** argv: Array of arguments: [1] - Image path to open
  *
  * Returns
  * int: Error code or 0 if no error occured (Define error codes? Wrap in cout func?)
  */
int main(int argc, char **argv)
{
	cout << endl << "----------------------------------------------" << endl
				 << " EE4H Assignment - Recognising playing cards  " << endl
				 << " By Yousef Amar and Chris Lewis               " << endl 
				 << "----------------------------------------------" << endl << endl;
	
	//Check image is provided
	if(argc > 1)
	{
		cv::Mat input = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
		cv::Size input_size = input.size();

		//Check size is greater than zero
		if(input_size.width > 0 && input_size.height > 0)
		{
			//Show image details
			cout << "'" << argv[1] << "' is " << input_size.width << " by " << input_size.height << " pixels." << endl << endl;

			//Make background black
			cv::Mat working = make_background_black(input, 250);

			//Filter only red
			working = filter_red_channel(working, 0);

			//Is red suit?
			cout << "Is red suit? " << (is_red_suit(working, 250) == true ? "true" : "false") << endl;

			bool red = is_red_suit_by_corners(working, corner_h_perc, corner_v_perc, 250, 2);
			cout << "Is red suit (by corners)? " << (red == true ? "true" : "false") << endl;

			//Show regions searched
			int region_width = (int) (corner_h_perc * (float) input_size.width);
			int region_height = (int) (corner_v_perc * (float) input_size.height);
			
			//Top left
			cv::Point start = cv::Point(0, 0);
			cv::Point finish = cv::Point(region_width, region_height);
			cv::rectangle(working, start, finish, line_colour, 1, 8, 0);

			//Bottom right
			start = cv::Point(input_size.width - region_width, input_size.height - region_height);
			finish = cv::Point(input_size.width, input_size.height);
			cv::rectangle(working, start, finish, line_colour, 1, 8, 0);

			//Suit colour
			cv::putText(working, (red == true ? string("RED") : string("BLACK")), cv::Point(5, 60), cv::FONT_HERSHEY_PLAIN, 5, text_colour, 3, 8, false);

			//Show results until key press
			cv::imshow("Results", working);
			cv::waitKey(0);

			//Finally
			cout << "Processing finished successfully!" << endl;
			return 0;	//No error code
		}
		else
		{
			cout << "Image dimensions must be > 0!" << endl;
			return -2;	//Image size zero code
		}
	}
	else
	{
		cout << "Arguments error. Check image path/format?" << endl;
		return -1;	//Incorrect arguments code
	}
}