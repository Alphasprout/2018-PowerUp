#include <iostream>
#include "rioreceive.h"

int main()
{
	rioreceive rr;
	rr.init();

	while (true)
	{
		std::vector<int> centre_xs;
		std::vector<double> angles;
		rr.get(centre_xs, angles);
		
		for (int n = 0; n < centre_xs.size(); n++)
		{
			std::cout << centre_xs[n] << " @ " << angles[n]*180/3.14 << '\t';
		}
		std::cout << std::endl;
	}
	return 0;
}
