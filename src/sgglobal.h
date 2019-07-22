#ifndef SGGLOBAL_H
#define SGGLOBAL_H

//export and import
#ifdef BUILDING_PLUGIN
    #ifdef _MSC_VER
        #define SG_EXPORT __declspec(dllexport)
    #else
        #define SG_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define SG_EXPORT __declspec(dllimport)
    #else
        #define SG_EXPORT __attribute__((visibility("default")))
    #endif
#endif

//use in Public class
#define DECLARE_D(Class) friend class Class##Private; \
Class##Private *d_ptr=nullptr; \
Class##Private& d() { return *d_ptr; } \
const Class##Private& d() const { return *d_ptr; }

//use in Private Class
#define DECLARE_P(Class) \
friend class Class; \
Class *p_ptr=nullptr; \
inline void setPublic(Class *ptr) { p_ptr = ptr; } \
Class& p() { return *p_ptr; } \
const Class& p() const { return *p_ptr; }

//initialize&free Private Object in Public Class
#define INIT_D(Class) \
d_ptr = new Class##Private(); d_ptr->setPublic(this);
#define FREE_D() \
delete d_ptr; d_ptr = nullptr;

//interface with P&D
#define DD(Class) Class##Private& dd = d();
#define PP(Class) Class& pp = p();

#endif // SGGLOBAL_H
