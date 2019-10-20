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
#define SG_DECLARE_PRIVATE(Class) \
public: \
friend class Class##Private; \
inline const Class##Private *d_func() const { return reinterpret_cast<Class##Private *>(d_ptr); } \
inline Class##Private *d_func() { return reinterpret_cast<Class##Private *>(d_ptr); }

//use in Private Class
#define SG_DECLARE_PUBLIC(Class) \
friend class Class; \
inline const Class *q_func() const { return reinterpret_cast<Class *>(q_ptr); } \
inline Class *q_func() { return reinterpret_cast<Class *>(q_ptr); } \
inline void setPublic(Class *ptr) { q_ptr = ptr; }

#define SG_D(Class) Class##Private * const d = d_func()
#define SG_Q(Class) Class * const q = q_func()

//initialize&free Private Object in Public Class
#define SG_INIT_PRIVATE(Ptr) \
d_ptr = Ptr; d_ptr->setPublic(this);
#define SG_FREE_PRIVATE() \
delete d_ptr; d_ptr = nullptr;

#endif // SGGLOBAL_H
