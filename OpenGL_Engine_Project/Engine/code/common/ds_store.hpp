#pragma once
#include "common.h"
#include "ds_type_id.hpp"

namespace data_struct {
	class DataStorage
	{
	//	struct StorableValue
	//	{
	//		struct ValueInterface 
	//		{
	//			virtual size_t get_type_id() = 0;
	//		};

	//		template<class T>
	//		struct ValueImpl : ValueInterface
	//		{
	//			virtual size_t get_type_id() override { return Type::value<T>(); }
	//			ValueImpl(T&& val) : data(std::move(val)) {}
	//			T data;
	//		};

	//		template<class T>
	//		StorableValue(T&& val) 
	//			: self(std::make_shared<ValueImpl<T>>(std::move(val)))
	//		{}

	//		template<class T> T& get()
	//		{
	//			ASSERT_MSG(Type::value<T>() == self->get_type_id(), "Fatal error. It's diferent types.");
	//			return std::static_pointer_cast<ValueImpl<T>>(self)->data;
	//		}

	//		std::shared_ptr<ValueInterface> self = nullptr;
	//	};

	//public:
	//	template<class T>
	//	void store(T&& val) {
	//		data[Type::value<T>()] = { val };
	//	}

	//	template<class T>
	//	T& require() {
	//		return data[Type::value<T>()].get<T>();
	//	}

	//private:
	//	std::unordered_map<size_t, StorableValue> data;
	public:
		static DataStorage& instance() {
			static DataStorage inst;
			return inst;
		}

		template<class T>
		void store(T* ptr) {
			data[Type::value<T>()] = (void*)ptr;
		}

		template<class T>
		T* require() {
			return (T*)data[Type::value<T>()];
		}

	private:
		std::unordered_map<size_t, void*> data;
	};
}

namespace ds = data_struct;