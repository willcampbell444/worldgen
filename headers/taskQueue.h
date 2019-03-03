#pragma once

#include <memory>
#include <queue>
#include <list>
#include <mutex>

class Task {
	friend class TaskQueue;

	std::list<std::unique_ptr<Task>> after;

public:
	virtual void execute() = 0;
	virtual ~Task() { }

protected:
	void runAfter(std::unique_ptr<Task> &&task) {
		after.push_back(std::move(task));
	}
};

class TaskQueue {
	std::mutex lock;
	std::queue<std::unique_ptr<Task>> tasks;

	std::unique_ptr<Task> getFirst() {
		std::lock_guard<std::mutex> l(lock);

		if (tasks.empty()) {
			return nullptr;
		} else {
			auto first = std::move(tasks.front());
			tasks.pop();
			return first;
		}
	}
		
public:
	void insert(std::unique_ptr<Task> task) {
		std::lock_guard<std::mutex> l(lock);

		tasks.push(std::move(task));
	}

	bool runTask() {
		auto task = getFirst();
		if (task) {
			task->execute();
			for (auto& newTask : task->after) {
				insert(std::move(newTask));
			}
			return true;
		}

		return false;
	}
};
