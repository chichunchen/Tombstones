/////////////////////////////////////////////////////////////////////////////
// tombstones.h, expected interface for CS254 assignment 5
/////////////////////////////////////////////////////////////////////////////

#if !defined(__TOMBSTONES_H__)
#define __TOMBSTONES_H__
#include <iostream>

template <class T> class Pointer;
template <class T> void free(Pointer<T>& obj);

template <class T>
class Pointer {
private:
    T* ptr;
public:
    Pointer<T>();                               // default constructor
    Pointer<T>(Pointer<T>&);                        // copy constructor
    Pointer<T>(T*);                             // bootstrapping constructor
        // argument should always be a call to new
    ~Pointer<T>();                              // destructor
    T& operator*() const;                   // deferencing
    T* operator->() const;                  // field dereferencing
    Pointer<T>& operator=(const Pointer<T>&);       // assignment
    friend void free<T>(Pointer<T>&);           // delete pointed-at object
        // This is essentially the inverse of the new inside the call to
        // the bootstrapping constructor.
    // equality comparisons:
    bool operator==(const Pointer<T>&) const;
    bool operator!=(const Pointer<T>&) const;
    bool operator==(const int) const;
        // true iff Pointer is null and int is zero
    bool operator!=(const int) const;
        // false iff Pointer is null and int is zero
    
    T* getPointer() const { return ptr; } // Just for test. Of course we can keep this.
};

template <class T>
Pointer<T>::Pointer() {
    ptr = new T();
}

template <class T>
Pointer<T>::Pointer(Pointer<T> &p) {
    if (!p.getPointer()) ptr = NULL;
    else { ptr = new T; *ptr = *(p.getPointer()); }
}

template <class T>
Pointer<T>::Pointer(T* p) {
    if (!p) ptr = NULL;
    else { ptr = new T; *ptr = *p; }
}

template <class T>
Pointer<T>::~Pointer() {
    if (!ptr) delete ptr;
}

template <class T>
T& Pointer<T>::operator*() const {
    return *ptr;
}

template <class T>
T* Pointer<T>::operator->() const {
    return ptr;
}

// I don't know if this is assignment of reference or value.
template <class T>
Pointer<T>& Pointer<T>::operator=(const Pointer<T>& assignment) {
    ptr = assignment.getPointer();
    return *this;
}

template <class T>
bool Pointer<T>::operator==(const Pointer<T>& other) const {
    return ptr == other.getPointer();
}

template <class T>
bool Pointer<T>::operator!=(const Pointer<T>& other) const {
    return ptr != other.getPointer();
}

// Generally, "new" will not return a NULL pointer,
// so the two overload here will return a not NULL result.
template <class T>
bool Pointer<T>::operator==(const int comp) const {
    return (!ptr && comp == 0) ? 1 : 0;
}

template <class T>
bool Pointer<T>::operator!=(const int comp) const {
    return (!ptr && comp == 0) ? 0 : 1;
}

template <class T>
void free(Pointer<T>& obj) {
    obj.~Pointer();
}


#endif // __TOMBSTONES_H__
