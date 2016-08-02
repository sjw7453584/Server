/* Singleton.h
 */

#ifndef _SINGLETON_H
#define _SINGLETON_H
#include <stdlib.h>

/// Should be placed in the appropriate .cpp file somewhere
#define initialiseSingleton( type ) \
template <> type * Singleton < type > :: mSingleton = 0

/// To be used as a replacement for initialiseSingleton( )
///  Creates a file-scoped Singleton object, to be retrieved with getSingleton
#define createFileSingleton( type ) \
initialiseSingleton( type ); \
type the##type

template < class type > class Singleton
{
    public:
        /// Constructor
        Singleton( )
        {
            mSingleton = static_cast<type *>(this);
        }
        /// Destructor
        ~Singleton( )
        {
            mSingleton = 0;
        }

        /// Retrieve the singleton object, if you hit this assert this singleton object doesn't exist yet
        static type & getSingleton( ) { return *mSingleton; }

        /// Retrieve a pointer to the singleton object
        static type * getSingletonPtr( ) { return mSingleton; }

    protected:

        /// Singleton pointer, must be set to 0 prior to creating the object
        static type * mSingleton;
};




#define DECLEAR_SINGLETON(T)\
public:\
	static T* GetSingletonPtr();\
	static void DestructSingleton();\
	static T & GetSingleton();\
private:\
	static T* m_pInstance;\


#define IMPLEMENT_SINGLETON(T)\
T* T::m_pInstance = NULL;\
T* T::GetSingletonPtr()\
{\
	if (!m_pInstance)\
	{\
		m_pInstance = new T();\
	}\
	return m_pInstance;\
}\
T & T::GetSingleton()\
{ \
	if (!m_pInstance)\
	{\
		m_pInstance = new T();\
	}\
	return *m_pInstance;\
}\
void T::DestructSingleton()\
{\
	if (m_pInstance)\
	{\
		delete m_pInstance;\
		m_pInstance = NULL;\
	}\
}\


#endif
