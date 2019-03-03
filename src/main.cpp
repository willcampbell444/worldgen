#include <memory>
#include <thread>
#include <iostream>

#include "visualize.h"
#include "taskQueue.h"

#define WORKER_COUNT 4

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
	glfwMakeContextCurrent(m_visualize.getWindow());
	m_visualize.draw();
	std::unique_ptr<Task> updateTask = std::make_unique<UpdateTask>(m_visualize);
	runAfter(std::move(updateTask));
}

void UpdateTask::execute() {
	std::list<std::unique_ptr<Task>> nextTasks;
	m_visualize.update(nextTasks);
	std::unique_ptr<Task> renderTask = std::make_unique<RenderTask>(m_visualize);
	runAfter(std::move(renderTask));
}

bool running = true;

class EndCheck: public Task {
	Visualize& m_visualize;

public:
	EndCheck(Visualize& v): m_visualize(v) { }
	void execute() override {
		running = !glfwWindowShouldClose(m_visualize.getWindow());
		std::unique_ptr<Task> endCheck = std::make_unique<EndCheck>(m_visualize);
		runAfter(std::move(endCheck));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
};

void workerLoop(TaskQueue &taskQueue) {
	while (running) {
		taskQueue.runTask();
	}
}

		
int main() {
	Visualize visualize;

	TaskQueue taskQueue;
	taskQueue.insert(std::make_unique<UpdateTask>(visualize));
	taskQueue.insert(std::make_unique<EndCheck>(visualize));

	std::vector<std::thread> threads;
	for (int i = 0; i < WORKER_COUNT; i++) {
		threads.emplace_back(workerLoop, std::ref(taskQueue));
	}

	for (auto& thread : threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}

	return 0;
}
