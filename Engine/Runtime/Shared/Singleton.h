//
//	Singleton.h
//	Good Neighbours
//
//	Created by Diego Revilla 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _SINGLETON__H_
#define _SINGLETON__H_

template<typename T>
class Singleton {
public:
	static T& Instance();

	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton) = delete;

protected:
	Singleton() {}
};

template<typename T>
T& Singleton<T>::Instance() {
	static T instance;
	return instance;
}

#endif