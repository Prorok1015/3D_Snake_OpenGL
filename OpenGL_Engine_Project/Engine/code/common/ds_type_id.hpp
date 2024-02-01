#pragma once

namespace data_struct {
	struct Type
	{
		using unique_id = size_t;
	public:
		template<class T>
		static unique_id value() {
			static unique_id classId = next();
			return classId;
		}

	private:
		static unique_id next() {
			static unique_id counter = 0;
			return counter++;
		}

	};
}

namespace ds = data_struct;