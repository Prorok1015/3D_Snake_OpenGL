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
		bool has_value() const {
			auto it = data.find(Type::value<T>());
			if (it != data.end()) {
				return it->second.has_value();
			}
			return false;
		}

		template<class T>
		std::shared_ptr<T> require_shared()
		{
			ASSERT_MSG(has_value<T>(), "Missing the required type");

			using cast_type = std::shared_ptr<T>;
			return std::any_cast<cast_type>(data[Type::value<T>()]);
		}

		template<class T>
		T& require() 
		{
			return *require_shared<T>();
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