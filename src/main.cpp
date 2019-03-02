#include <memory>

#include "visualize.h"
#include "taskQueue.h"

class RenderTask: public Task {
	Visualize& m_visualize;

public:
	RenderTask(Visualize& v): m_visualize(v) { }
	void execute() override;
};

class UpdateTask: public Task {
	Visualize& m_visualize;

public:
	UpdateTask(Visualize& v): m_visualize(v) { }
	void execute() override;
};

void RenderTask::execute() {
	m_visualize.draw();
	std::unique_ptr<Task> updateTask = std::make_unique<UpdateTask>(m_visualize);
	runAfter(std::move(updateTask));
}

void UpdateTask::execute() {
	m_visualize.update();
	std::unique_ptr<Task> renderTask = std::make_unique<RenderTask>(m_visualize);
	runAfter(std::move(renderTask));
}

int main() {
	Visualize visualize;

	TaskQueue taskQueue;
	taskQueue.insert(std::make_unique<UpdateTask>(visualize));
    while(!glfwWindowShouldClose(visualize.getWindow())
		  && taskQueue.runTask()) {
		  // ...
    }
}
