//
// Created by LJ on 2018/1/27.
//

#ifndef JEWELBATTLE_SINGLETON_H
#define JEWELBATTLE_SINGLETON_H

#define DECLARE_SINGLETON(T) \
	private: \
        T(){} \
        static T *m_instance; \
	public:\
		static T *GetInstance() \
		{ \
			if (nullptr == m_instance) \
				m_instance = new T(); \
			return m_instance; \
		}
#define IMPLEMENT_SINGLETON(T) \
    T * T::m_instance = nullptr;
#endif //JEWELBATTLE_SINGLETON_H
