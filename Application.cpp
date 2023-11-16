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

const int int_test = 2023;
const double double_test = 20.23;
const BString string_test("test value");
const BString path_test("/boot/home/workspace/GenioTask");
const BString exception_test("This is an exception!");
const bool bool_test = true;

struct test_struct {
	string str;
	int32 val;
	test_struct(string _str, int32 _val) { str = _str; val = _val; }
};

test_struct TaskFunction_test_struct()
{
	cout << "TaskFunction_test_struct()" << endl; 
	return test_struct(string_test.String(), int_test);
}

auto TaskFunction_SharedPtr()
{
	cout << "TaskFunction_SharedPtr()" << endl; 
	return make_shared<test_struct>(string_test.String(), int_test);
}

auto TaskFunction_std_path()
{
	cout << "TaskFunction_path()" << endl; 
	return path(path_test.String());
}

BPath TaskFunction_bpath()
{
	cout << "TaskFunction_bpath()" << endl; 
	return BPath(path_test);
}

bool TaskFunction_bool()
{
	cout << "TaskFunction_bool()" << endl; 
	return bool_test;
}

int32 TaskFunction_int32()
{
	cout << "TaskFunction_int32()" << endl; 
	return int_test;
}

double TaskFunction_double()
{
	cout << "TaskFunction_double()" << endl; 
	return double_test;
}

BString TaskFunction_BString()
{
	cout << "TaskFunction_BString()" << endl; 
	return string_test;
}

test_struct* TaskFunction_Pointer()
{
	cout << "TaskFunction_Pointer()" << endl; 
	auto pointer = new test_struct(string_test.String(), int_test);
	return pointer;
}

void TaskFunction_void()
{
	cout << "TaskFunction_void()" << endl; 
}

int32 TaskFunction_int_exception()
{
	cout << "TaskFunction_int32()" << endl;
	throw runtime_error(exception_test.String());
	return int_test;
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
#if 0	
	Task<test_struct> task1("test_struct", BMessenger(this), TaskFunction_test_struct);
	task1.Run();
#endif
#if 0
	Task<shared_ptr<test_struct>> task2("test_shared_ptr", BMessenger(this), TaskFunction_SharedPtr);
	task2.Run();
#endif
#if 0		
	Task<std::filesystem::path> task3("test_path", BMessenger(this), &TaskFunction_std_path);
	task3.Run();
#endif	
	Task<bool> task4("test_bool", BMessenger(this), &TaskFunction_bool);
	task4.Run();

	Task<int32> task5("test_int32", BMessenger(this), TaskFunction_int32);
	task5.Run();

	Task<double> task6("test_double", BMessenger(this), TaskFunction_double);
	task6.Run();

	Task<BString> task7("test_bstring", BMessenger(this), TaskFunction_BString);
	task7.Run();

	Task<test_struct*> task8("test_pointer", BMessenger(this), TaskFunction_Pointer);
	task8.Run();

	Task<BPath> task9("test_bpath", BMessenger(this), &TaskFunction_bpath);
	task9.Run();

	Task<BPath> task10("test_void", BMessenger(this), &TaskFunction_void);
	task10.Run();
	
	Task<BPath> task11("test_int_exception", BMessenger(this), &TaskFunction_int_exception);
	task11.Run();
	
}

void MainApp::MessageReceived(BMessage* msg) {
	switch (msg->what) {
		case TASK_RESULT_MESSAGE:
		{
			cout << "TASK_RESULT_MESSAGE" << endl;
			msg->PrintToStream();
			try {
				auto task_name = BString(msg->GetString("TaskResult::TaskName"));
				
				// this test always fails due to bad alloc exception
				if (task_name=="test_path") {
					auto task_result = TaskResult<std::filesystem::path>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << result.c_str() << endl;
					if (result.c_str() == path_test.String())
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_bpath") {
					auto task_result = TaskResult<BPath>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << result.Path() << endl;
					if (result.Path() == path_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_struct") {
					auto task_result = TaskResult<test_struct>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << " {str: " << result.str << " val: " << result.val << "}" << endl;
					if (BString(result.str.c_str()) == string_test && result.val == int_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_shared_ptr") {
					auto task_result = TaskResult<shared_ptr<test_struct>>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << " {str: " << result->str << " val: " << result->val << "}" << endl;
					if (BString(result->str.c_str()) == string_test && result->val == int_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_bool") {
					auto task_result = TaskResult<bool>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << "result: " << result << endl;
					if (result == bool_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_int32") {
					auto task_result = TaskResult<int32>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << "result: " << result << endl;
					if (result == int_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_double") {
					auto task_result = TaskResult<double>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << "result: " << result << endl;
					if (result == double_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_bstring") {
					auto task_result = TaskResult<BString>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " - Result: " << result << endl;
					if (result == string_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete task_result;
				}
				if (task_name=="test_pointer") {
					auto task_result = TaskResult<test_struct*>::Instantiate(msg);
					auto result = task_result->GetResult();
					cout << task_name << " (test_struct*) - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " (test_struct*) - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " (test_struct*) - Type: " << "Type:" << typeid(result).name() << endl;
					cout << task_name << " (test_struct*) - result: " << " {str: " << result->str << " val: " << result->val << "}" << endl;
					if (BString(result->str.c_str()) == string_test && result->val == int_test)
						cout << "Test passed!" << endl;
					else
						cout << "Test failed!" << endl;
					delete result;
					delete task_result;
				}
				if (task_name=="test_void") {
					auto task_result = TaskResult<void>::Instantiate(msg);
					// we can't get any result from a function returning void
					// we just get confirmation that the task is complete if it does not raise an exception
					task_result->GetResult();
					cout << task_name << " (test_struct*) - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " (test_struct*) - ID: " << task_result->GetTaskID() << endl;
					cout << task_name << " (test_struct*) - Type: " << "Type:" << "void" << endl;
					cout << task_name << " (test_struct*) - result: " << " N/A" << endl;
					cout << "Test passed!" << endl;
					delete task_result;
				}
				if (task_name=="test_int_exception") {
					auto task_result = TaskResult<int32>::Instantiate(msg);
					cout << task_name << " - Name: " << task_result->GetTaskName() << endl;
					cout << task_name << " - ID: " << task_result->GetTaskID() << endl;
					try {
						auto result = task_result->GetResult();
						cout << task_name << " - Type: " << "Type:" << typeid(result).name() << endl;
						cout << task_name << " - Result: " << "result: " << result << endl;
						// if we get here no exception has been raised and the test failed
						cout << "Test failed!" << endl;
					} catch (std::exception &ex) {
						cout << "exception: " << ex.what() << endl;
						if (BString(ex.what()) == exception_test)
							cout << "Test passed!" << endl;
						else
							cout << "Test failed!" << endl;
					}
					delete task_result;
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
