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

	//template<class T>
	//struct TypeId
	//{
	//	static TypeId<T> get()
	//	{
	//		return { Type::value<T>() };
	//	}

	//	auto operator <=> (const TypeId<T>&) const = default;

	//	Type::unique_id value() const { return id_; }

	//	Type::unique_id id_ = NULL_ID;
	//	static constexpr Type::unique_id NULL_ID = -1;
	//};
}

namespace ds = data_struct;