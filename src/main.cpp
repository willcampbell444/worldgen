#include <groundGen.h>
#include <iostream>

int main() {
	GroundGen ground;
	ground.generate(9);

	std::cout << std::endl;
	std::cout << std::endl;
	for (int x = 0; x < 9; x++) {
		for (int z = 0; z < 9; z++) {
			std::cout << ground.getHeight(x, z) << "\t";
		}
		std::cout << std::endl;
	}
}