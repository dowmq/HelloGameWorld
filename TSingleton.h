#pragma once
template<typename T>
class TSingleton
{
public:
	static T& Instance() {
		static T instance;
		return instance;
	}
	TSingleton() = default;
	virtual ~TSingleton() = default;

	TSingleton(const TSingleton&) = delete;
	TSingleton& operator=(const TSingleton&) = delete;
	TSingleton(TSingleton&&) = delete;
	TSingleton& operator=(TSingleton&&) = delete;
};