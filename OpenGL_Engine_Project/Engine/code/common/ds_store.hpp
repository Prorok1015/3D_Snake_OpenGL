#pragma once
#include "common.h"
#include "ds_type_id.hpp"
#include <any>

namespace data_struct {
	class DataStorage
	{
	public:
		static DataStorage& instance() {
			static DataStorage inst;
			return inst;
		}

		template<class T>
		T& require() 
		{
			try
			{
				return *std::any_cast<std::shared_ptr<T>&>(data[Type::value<T>()]);
			}
			catch (const std::bad_any_cast& e)
			{
				ASSERT_FAIL("bad require()");
				static T obj{};
				return obj;
			}
		}

		template<class T, class ...ARGS>
		T& construct(ARGS&&... args)
		{
			data[Type::value<T>()] = std::make_any<std::shared_ptr<T>>(std::make_shared<T>(std::forward<ARGS>(args)...));
			return require<T>();
		}

		template<class T>
		T& construct()
		{
			data[Type::value<T>()] = std::make_shared<T>();
			return require<T>();
		}

	private:
		std::unordered_map<size_t, std::any> data;
	};
}

namespace ds = data_struct;