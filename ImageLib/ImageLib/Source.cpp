#ifdef TESTLIB
#include "Image.h"
#include <iostream>

int main()
{
	Image img("test5.jpg");
	img.write("write.png");
	system("pause");
}



#endif // TESTLIB

