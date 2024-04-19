/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TaskView.h"

#include "TaskRoster.h"

#include <LayoutBuilder.h>
#include <iostream>
#include <ostream>

TaskView::TaskView()
	:
	BView("task view", B_WILL_DRAW)
{
	fListView = new BListView("listview");
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.Add(fListView)
		.End();
	SetFlags(Flags() | B_PULSE_NEEDED);
}


/* virtual*/
void
TaskView::Pulse()
{
	fListView->MakeEmpty();
	TaskRoster* roster = TaskRoster::Get();
	for (int32 i = 0; i < roster->CountTasks(); i++) {
		TaskDescriptor* t = roster->TaskAt(i);
		BString text;
		text << t->name << " (" << t->id << ") ";
		if (t->status == TaskDescriptor::STATUS_RUNNING)
			text << " running";
		else if (t->status == TaskDescriptor::STATUS_STOPPED)
			text << " stopped";

		fListView->AddItem(new BStringItem(text));
	}
}


