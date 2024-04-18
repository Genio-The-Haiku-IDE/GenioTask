/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TaskRoster.h"

#include <iostream>


TaskRoster gTaskRoster;

TaskRoster::TaskRoster()
{
}


TaskRoster*
TaskRoster::Get()
{
	return &gTaskRoster;
}


int32
TaskRoster::CountTasks() const
{
	return fTasks.size();
}


TaskDescriptor*
TaskRoster::TaskAt(int32 i) const
{
	return fTasks.at(i);
}


TaskDescriptor*
TaskRoster::TaskByThreadID(thread_id id) const
{
	std::vector<TaskDescriptor*>::const_iterator i;
	for (i = fTasks.begin(); i != fTasks.end(); i++) {
		if ((*i)->id == id)
			return *i;
	}
	return nullptr;
}


TaskDescriptor*
TaskRoster::TaskByName(const char* name) const
{
	std::vector<TaskDescriptor*>::const_iterator i;
	for (i = fTasks.begin(); i != fTasks.end(); i++) {
		if ((*i)->name == name)
			return *i;
	}
	return nullptr;
}


bool
TaskRoster::AddTask(TaskDescriptor* task)
{
	fTasks.push_back(task);
	return true;
}


// TaskDescriptor
TaskDescriptor::TaskDescriptor(const char* taskName, thread_id threadID)
	:
	name(taskName),
	id(threadID)
{
}


void
TaskDescriptor::PrintToStream()
{
	std::cout << "Task: ";
	std::cout << name << ", id: " << id << " status : ";
	if (status == STATUS_RUNNING)
		std::cout << "RUNNING" << std::endl;
	else if (status == STATUS_STOPPED)
		std::cout << "STOPPED" << std::endl;
}


