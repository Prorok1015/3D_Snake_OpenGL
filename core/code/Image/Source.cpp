#ifdef TESTLIB
#include "Image.h"
#include <iostream>

int main()
{
	Image img;
	img.flip(Image::ImageFlip::VERTICAL);
	img.open("test.jpg");
	img.write("write.png");
	system("pause");
}



#endif // TESTLIB

