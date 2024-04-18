/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <OS.h>
#include <String.h>
#include <vector>

class TaskDescriptor {
public:
	TaskDescriptor(const char* name, thread_id threadId);

	BString name;
	thread_id id;

	enum task_status {
		STATUS_STOPPED = 0,
		STATUS_RUNNING
	};

	task_status status;

	void PrintToStream();
};


class TaskRoster {
public:
	TaskRoster();
	static TaskRoster* Get();

	int32 CountTasks() const;
	TaskDescriptor* TaskAt(int32 i) const;
	TaskDescriptor* TaskByThreadID(thread_id id) const;
	TaskDescriptor* TaskByName(const char* name) const;

	bool AddTask(TaskDescriptor* descriptor);

private:
	std::vector<TaskDescriptor*> fTasks;

};
