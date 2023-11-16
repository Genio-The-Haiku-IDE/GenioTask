/*
 * Copyright 2023, Nexus6 <nexus6.haiku@icloud.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#pragma once

#include <any>
#include <map>
#include <stdexcept>

#include <Alignment.h>
#include <Archivable.h>
#include <Message.h>
#include <Messenger.h>
#include <String.h>

namespace Genio::Task {

	using namespace std;

	namespace Private {

		struct ThreadData {
			ThreadData(void* function, const BMessenger& srcMessenger, const BString& taskName) {
				target_function = function;
				messenger = srcMessenger;
				name = taskName;
			};
			void *target_function;
			BMessenger messenger;
			thread_id id;
			BString name;
		};

		static exception_ptr current_exception_ptr;
		static map<thread_id, exception_ptr> TaskExceptionMap;
		static map<string, type_code> TypeMap = {
			{typeid(BAlignment).name(), B_ALIGNMENT_TYPE},
			{typeid(BRect).name(), B_RECT_TYPE},
			{typeid(BPoint).name(), B_POINT_TYPE},
			{typeid(BSize).name(), B_SIZE_TYPE},
			{typeid(const char *).name(), B_STRING_TYPE},
			{typeid(BString).name(), B_STRING_TYPE},
			{typeid(std::string).name(), B_STRING_TYPE},
			// {typeid(BStringList).name(), B_STRING_LIST_TYPE},
			{typeid(int8).name(), B_INT8_TYPE},
			{typeid(int16).name(), B_INT16_TYPE},
			{typeid(int32).name(), B_INT32_TYPE},
			{typeid(int64).name(), B_INT64_TYPE},
			{typeid(uint8).name(), B_UINT8_TYPE},
			{typeid(uint16).name(), B_UINT16_TYPE},
			{typeid(uint32).name(), B_UINT32_TYPE},
			{typeid(uint64).name(), B_UINT64_TYPE},
			{typeid(bool).name(), B_BOOL_TYPE},
			{typeid(float).name(), B_FLOAT_TYPE},
			{typeid(double).name(), B_DOUBLE_TYPE},
			{typeid(rgb_color).name(), B_RGB_COLOR_TYPE},
			{typeid(const void *).name(), B_POINTER_TYPE},
			{typeid(BMessenger).name(), B_MESSENGER_TYPE},
			{typeid(entry_ref).name(), B_REF_TYPE},
			{typeid(node_ref).name(), B_NODE_REF_TYPE},
			{typeid(BMessage).name(), B_MESSAGE_TYPE},
		};

		template <typename T>
		struct BMessageType {
			static type_code Get() { return TypeMap[typeid(T).name()]; }
		};
	}

	using namespace Private;

	const int TASK_RESULT_MESSAGE = 'tfwr';

	template <typename ResultType>
	class Task;

	template <typename ResultType>
	class TaskResult: public BArchivable {
	public:
		TaskResult(const BString& name, std::any result, thread_id id)
			:
			fResult(result),
			fId(id),
			fName(name)
		{
		}
		TaskResult(const BMessage &archive)
			:
			fResult(nullptr),
			fId(-1)
		{
			if constexpr (std::is_void<ResultType>::value == false) {
				type_code type;
				if (archive.GetInfo(kResultField, &type) == B_OK) {
					ssize_t size = 0;
					const void *result;
					status_t error = archive.FindData(kResultField, type,
						reinterpret_cast<const void**>(&result), &size);
					if (error == B_OK) {
						fResult = *reinterpret_cast<const ResultType*>(result);
					}
				}
			}

			if (archive.FindInt32(kTaskIdField, &fId) != B_OK) {
					throw runtime_error("Can't unarchive TaskResult instance: Task ID not available");
			}
			if (archive.FindString(kTaskNameField, &fName) != B_OK) {
					throw runtime_error("Can't unarchive TaskResult instance: Task Name not available");
			}
		}

		~TaskResult() {}

		ResultType	GetResult() const
		{
			// auto ptr = TaskExceptionMap[fId];
			// if (ptr) {
			auto i = TaskExceptionMap.find(fId);
			if (i != TaskExceptionMap.end()) {
				auto ptr = std::move(i->second);
				TaskExceptionMap.erase(i);
				rethrow_exception(ptr);
			} else {
				if constexpr (std::is_void<ResultType>::value == false) {
					return any_cast<ResultType>(fResult);
				}
			}
		}

		virtual status_t Archive(BMessage *archive, bool deep) const
		{
			status_t status = BArchivable::Archive(archive, deep);
			if (status != B_OK)
				return status;

			if constexpr (std::is_void<ResultType>::value == false) {
				if (fResult.has_value()) {
					type_code type = BMessageType<ResultType>::Get();
					ResultType result = any_cast<ResultType>(fResult);
					status = archive->AddData(kResultField, type, &result, sizeof(ResultType));
				}
			}
			if (status != B_OK)
				return status;
			status = archive->AddInt32(kTaskIdField, fId);
			if (status != B_OK)
				return status;
			status = archive->AddString(kTaskNameField, fName);
			if (status != B_OK)
				return status;
			status = archive->AddString("class", "TaskResult");
			return status;
		}

		static TaskResult<ResultType>* Instantiate(BMessage* archive)
		{
			if (validate_instantiation(archive, "TaskResult"))
				return new TaskResult<ResultType>(*archive);
			return nullptr;
		}

		thread_id GetTaskID() const { return fId; }
		const char* GetTaskName() const { return fName; }

	private:
		TaskResult() { debugger("called TaskResult private constructor!"); };

		const char*		kResultField = "TaskResult::Result";
		const char*		kTaskIdField = "TaskResult::TaskID";
		const char*		kTaskNameField = "TaskResult::TaskName";

		any				fResult;
		thread_id		fId;
		BString			fName;
	};


	template <typename ResultType>
	class Task {
	public:
		using native_handle_type = thread_id;

		template<typename Function, typename... Args>
		Task(const char *name, const BMessenger& messenger, Function&& function, Args&&... args)
		{
			auto targetFunction =
				new arguments_wrapper<Function, Args...>(std::forward<Function>(function),
				std::forward<Args>(args)...);

			using lambda_type = std::remove_reference_t<decltype(*targetFunction)>;

			ThreadData *threadData = new ThreadData(targetFunction, messenger, name);

			fThreadHandle = spawn_thread(&_CallTarget<ThreadData, lambda_type>,
										name,
										B_NORMAL_PRIORITY,
										threadData);

			threadData->id = fThreadHandle;
			if (fThreadHandle < 0) {
				delete targetFunction;
				delete threadData;
				throw runtime_error("Can't create task");
			}
		}

		~Task() {}

		status_t Run()
		{
			if (fThreadHandle > 0)
				return resume_thread(fThreadHandle);
			return B_ERROR;
		}
		status_t Stop()
		{
			if (fThreadHandle > 0)
				return kill_thread(fThreadHandle);
			return B_ERROR;
		}
	private:
		native_handle_type fThreadHandle;

		template<typename Data, typename Lambda>
		static int32 _CallTarget(void *target)
		{
			Data *data = reinterpret_cast<Data*>(target);
			Lambda* lambda = reinterpret_cast<Lambda*>(data->target_function);
			std::any anyResult;
			try {
				using ret_t = decltype((*lambda)());
				if constexpr (std::is_same_v<void, ret_t>) {
					(*lambda)();
					// TODO: anyResult is not initialized here
				} else {
					auto result = (*lambda)();
					anyResult = result;
				}
			} catch (...) {
				TaskExceptionMap[data->id] = current_exception();
			}

			TaskResult<ResultType> taskResult(data->name, anyResult, data->id);
			BMessenger messenger = data->messenger;

			delete lambda;
			delete data;

			BMessage msg(TASK_RESULT_MESSAGE);
			if (taskResult.Archive(&msg, false) == B_OK) {
				if (messenger.IsValid()) {
					messenger.SendMessage(&msg);
				}
			} else {
				throw runtime_error("Can't create TaskResult message");
			}

			return B_OK;
		}

		template <typename Function, typename ... Args>
		class arguments_wrapper {
			Function callable;
			std::tuple<std::decay_t<Args>...> args;
		public:
			arguments_wrapper(Function&& callable, Args&& ... args)
				:
				callable(std::forward<Function>(callable)),
				args{std::forward<Args>(args)...}
			{
			}

			constexpr decltype(auto) operator()()
			{
				return apply(std::make_index_sequence<sizeof...(Args)>{});
			}

		private:
			template <std::size_t ... indices>
			constexpr decltype(auto) apply(std::index_sequence<indices...>)
			{
				return callable(std::move(std::get<indices>(args))...);
			}

		};
	};
}
