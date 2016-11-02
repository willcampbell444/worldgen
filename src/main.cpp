#include <visualize.h>

int main() {
	Visualize visualize;
    while(!glfwWindowShouldClose(visualize.getWindow())) {
        visualize.update();
        visualize.draw();
    }
}