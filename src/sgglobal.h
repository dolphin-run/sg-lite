#ifndef SGGLOBAL_H
#define SGGLOBAL_H

//export and import
#ifdef SG_BUILDING_PLUGIN
    #ifdef _MSC_VER
        #define SG_DECL_EXPORT __declspec(dllexport)
    #else
        #define SG_DECL_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define SG_DECL_EXPORT __declspec(dllimport)
    #else
        #define SG_DECL_EXPORT __attribute__((visibility("default")))
    #endif
#endif

#define SG_DISABLE_COPY(Class) \
    Class(const Class &) =delete;\
    Class &operator=(const Class &) =delete;

//use in Public class
#define SG_DECLARE_PRIVATE(Class) friend class Class##Private; \
Class##Private *d_ptr=nullptr;

//use in Private Class
#define SG_DECLARE_PUBLIC(Class) \
friend class Class; \
Class *q_ptr=nullptr; \
inline void setPublic(Class *ptr) { q_ptr = ptr; }

//initialize&free Private Object in Public Class
#define SG_INIT_PRIVATE(Class) \
d_ptr = new Class##Private(); d_ptr->setPublic(this);
#define SG_FREE_PRIVATE() \
delete d_ptr; d_ptr = nullptr;

#endif // SGGLOBAL_H
