#include <any>
#include <filesystem>
#include <iostream>
#include <memory>

#include <Path.h>

#include "Application.h"
#include "Task.h"

using namespace std;
using namespace std::filesystem;
using namespace Genio::Task;

struct test_struct {
	string str;
	int32 val;
	test_struct(string _str, int32 _val) { str = _str; val = _val; }
};

test_struct TaskFunction_test_struct()
{
	cout << "TaskFunction_test_struct()" << endl; 
	return test_struct("test value", 1978);
}

auto TaskFunction_SharedPtr()
{
	cout << "TaskFunction_SharedPtr()" << endl; 
	return make_shared<test_struct>("test val", 1978);
}

auto TaskFunction_std_path()
{
	cout << "TaskFunction_path()" << endl; 
	return path("/boot/home/workspace/Weather");
}

BPath TaskFunction_bpath()
{
	cout << "TaskFunction_path()" << endl; 
	return BPath("/boot/home/workspace/Weather");
}

bool TaskFunction_bool()
{
	cout << "TaskFunction_bool()" << endl; 
	return true;
}

int32 TaskFunction_int32()
{
	cout << "TaskFunction_int32()" << endl; 
	return 1978;
}

double TaskFunction_double()
{
	cout << "TaskFunction_double()" << endl; 
	return 19.78;
}

BString TaskFunction_BString()
{
	cout << "TaskFunction_BString()" << endl; 
	return "Test passed!";
}

test_struct* TaskFunction_Pointer()
{
	cout << "TaskFunction_double()" << endl; 
	auto pointer = new test_struct("test", 15);
	return pointer;
}

MainApp::MainApp()
	: BApplication ("application/x-vnd.genio-task")
{
}

MainApp::~MainApp()
{
}

void MainApp::ReadyToRun() {
	cout << "ReadyToRun()" << endl; 
	
	Task<test_struct> task1("test_struct", new BMessenger(this), TaskFunction_test_struct);
	task1.Run();
	// 
	Task<shared_ptr<test_struct>> task2("test_shared_ptr", new BMessenger(this), TaskFunction_SharedPtr);
	task2.Run();
	
	Task<std::filesystem::path> task3("test_path", new BMessenger(this), &TaskFunction_std_path);
	task3.Run();
	
	Task<bool> task4("test_bool", new BMessenger(this), &TaskFunction_bool);
	task4.Run();
	
	Task<int32> task5("test_int32", new BMessenger(this), TaskFunction_int32);
	task5.Run();
	
	Task<double> task6("test_double", new BMessenger(this), TaskFunction_double);
	task6.Run();
	
	Task<BString> task7("test_bstring", new BMessenger(this), TaskFunction_BString);
	task7.Run();
	
	Task<test_struct*> task8("test_pointer", new BMessenger(this), TaskFunction_Pointer);
	task8.Run();
	
	Task<BPath> task9("test_bpath", new BMessenger(this), &TaskFunction_bpath);
	task9.Run();
	
}

void MainApp::MessageReceived(BMessage* msg) {
	switch (msg->what) {
		case TASK_RESULT_MESSAGE:
		{
			cout << "TASK_RESULT_MESSAGE" << endl;
			msg->PrintToStream();
			try {
				auto task_name = BString(msg->GetString("TaskResult::TaskName"));
				
				// this test fails due to bad alloc exception
				if (task_name=="test_path") {
					auto task_result = TaskResult<std::filesystem::path>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << result.c_str() << endl;
				}
				if (task_name=="test_bpath") {
					auto task_result = TaskResult<BPath>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << result.Path() << endl;
				}
				if (task_name=="test_struct") {
					auto task_result = TaskResult<test_struct>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << " {str: " << result.str << " val: " << result.val << "}" << endl;
				}
				if (task_name=="test_shared_ptr") {
					auto task_result = TaskResult<shared_ptr<test_struct>>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << "result => str: " << result->str << " val: " << result->val << endl;
				}
				if (task_name=="test_bool") {
					auto task_result = TaskResult<bool>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << "result: " << result << endl;
				}
				if (task_name=="test_int32") {
					auto task_result = TaskResult<int32>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << "result: " << result << endl;
				}
				if (task_name=="test_double") {
					auto task_result = TaskResult<double>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << "result: " << result << endl;
				}
				if (task_name=="test_bstring") {
					auto task_result = TaskResult<BString>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << result << endl;
				}
				if (task_name=="test_pointer") {
					auto task_result = TaskResult<test_struct*>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " (test_struct*) - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " (test_struct*) - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " (test_struct*) - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " (test_struct*) - result: " << " {str: " << result->str << " val: " << result->val << "}" << endl;
					delete result;
				}
								
			} catch(std::exception &ex) {
				cout << "exception: " << ex.what() << endl;
			}
			// Quit();
		}
		break;
		default:
			BApplication::MessageReceived(msg);
	}
}
