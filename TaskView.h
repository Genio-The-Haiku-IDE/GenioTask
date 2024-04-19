/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#pragma once

#include <ListView.h>

class TaskView : public BView {
public:
	TaskView();
	virtual void Pulse();
private:
	BListView* fListView;
};

