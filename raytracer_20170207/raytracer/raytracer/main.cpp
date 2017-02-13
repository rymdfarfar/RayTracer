#include "CImg.h"
#include "kf/kf_vector.h"
#include "scene.h"
#include "renderable.h"
#include "kf/kf_ray.h"
#include "light.h"
#include "windows.h"
#include "luascript.h"
#include <iostream>
#include <vector>





struct Block {
	int startWidth;
	int endWidth;
	int startHeight;
	int endHeight;
	Block(int sW = 0, int eW = 0, int sH =0, int eH = 0) : startWidth(sW), endWidth(eW), startHeight(sH), endHeight(eH) {}

};

using namespace cimg_library;

// This controls if the rendering displays progressively. When timing the results, turn this off (otherwise updating the window repeatedly will affect the timing)
// progressiveCount is how many lines are rendered before updating the window.
bool progressiveDisplay = false;
int  progressiveCount   = 10;


// The resolution of the window and the output of the ray tracer. This can be overridden by the Lua startup script.
int windowWidth = 1024;
int windowHeight = 1024;


// The scene object.
Scene g_scene;

// Lua state object used to run the startup script.
lua_State *g_state;




int main(int argc, char **argv)
{
	srand(0);
	
	std::string startupScript = "scene.lua";
	if (argc > 1)
	{
		startupScript = argv[1];
	}

	
	initLua(startupScript);

	
	
	// The floating point image target that the scene is rendered into.
	CImg<float> image(windowWidth * 1.333f , windowHeight * 1.333f, 1, 3, 0);
	// The display object used to show the image.
	CImgDisplay main_disp(image, "Raytrace");
	main_disp.set_normalization(0);	// Normalisation 0 disables auto normalisation of the image (scales to make the darkest to brightest colour fit 0 to 1 range.


	// Record the starting time.
	DWORD startTime = timeGetTime();

	// Primary loop through all screen pixels.
	
	for (int y = 0; y < windowHeight; y++)
	{
		for (int x = 0; x < windowWidth; x++)
		{
			// Retrieve the colour of the specified pixel. The math below converts pixel coordinates (0 to width) into camera coordinates (-1 to 1).
			kf::Colour output = g_scene.trace(float(x - windowWidth / 2) / (windowWidth / 2), -float(y - windowHeight / 2) / (windowHeight / 2)*((float(windowHeight) / windowWidth)));
		
			// Clamp the output colour to 0-1 range before conversion.
			output.saturate();
			// Convert from linear space to sRGB.
			output.toSRGB();
			// Write the colour to the image (scaling up by 255).
			*image.data(x, y, 0, 0) = output.r*255;
			*image.data(x, y, 0, 1) = output.g*255;
			*image.data(x, y, 0, 2) = output.b*255;
			
		}
		
		// Perform progressive display if enabled.
		if (progressiveDisplay)
		{
			if (y % progressiveCount == 0)
			{
				
				main_disp.display(image);
				main_disp.set_title("Current render time: %dms", timeGetTime() - startTime);
			}
		}

		// Check for Escape key.
		if (main_disp.is_keyESC())
			return 0;
	}

	
	// Record ending time.
	DWORD endTime = timeGetTime();
	
	// Display elapsed time in the window title bar.
	main_disp.set_title("Render time: %dms", endTime - startTime);
	main_disp.display(image);

	// Save the output to a bmp file.
	
	image.save_bmp("output.bmp");
	// Keep refreshing the window until it is closed or escape is hit.
	while (!main_disp.is_closed())
	{
		if (main_disp.is_keyESC())
			return 0;
		main_disp.wait();
	}

	return 0;


}
std::vector<Block> MapOut(int width, int height, int squares) {

	int widthPerGrid = width / squares;
	int heightPerGrid = height / squares;
	int startNumWidth = 0;
	int endNumWidth = startNumWidth + widthPerGrid;
	int startNumHeight = 0;
	int endNumHeight = startNumHeight + heightPerGrid;
	std::vector<Block> square;
	for (int i = 0; i < squares; ++i) {
		
		
		if (endNumWidth != width) {
			Block temp(startNumWidth, endNumWidth, startNumHeight, endNumHeight);
			startNumWidth += widthPerGrid;
			endNumWidth += widthPerGrid;
			square.push_back(temp);
		}
		else {

			Block temp(startNumWidth, endNumWidth, startNumHeight, endNumHeight);
			startNumHeight += heightPerGrid;
			endNumHeight += heightPerGrid;
			square.push_back(temp);
		}
	}

}
