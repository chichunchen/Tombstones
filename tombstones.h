/////////////////////////////////////////////////////////////////////////////
// tombstones.h, expected interface for CS254 assignment 5
/////////////////////////////////////////////////////////////////////////////

#if !defined(__TOMBSTONES_H__)
#define __TOMBSTONES_H__
#include <iostream>

template <class T> class Pointer;
template <class T> void free(Pointer<T>& obj);

template <class T>
void dangling_pointer_error(Pointer<T>& obj) {
    std::cerr << "Dangling reference, address: " << (obj.ptr)->content << std::endl;
    exit(1);
    //std::terminate();
}

template <class T>
struct Tomb {
    
    T* content;
    int ref_cnt;
};

template <class T>
class Pointer {
public:
    Tomb<T>* ptr;
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
    Tomb<T> tomb;
    tomb.content = NULL;
    tomb.ref_cnt = 0;
    ptr = &tomb;
}

template <class T>
Pointer<T>::Pointer(Pointer<T> &p) {
    ptr = p.ptr;
    if (!(ptr->content)) {
        (ptr->ref_cnt)++;
    }

}

template <class T>
Pointer<T>::Pointer(T* p) {
    // if (!p) ptr =NULL;
    // else { ptr = new T; *ptr = *p; }
    Tomb<T> tomb;
    tomb.content = p;
    std::cout << "bootstrap: " << (p == NULL) << (tomb.content == NULL);
    if (!(tomb.content)) {
        tomb.ref_cnt = 0;
    }
    else {
        tomb.ref_cnt = 1;
    }
    ptr = &tomb;
}

template <class T>
Pointer<T>::~Pointer() {
    // Need revise
    if (ptr->ref_cnt > 0) delete ptr;
}

template <class T>
T& Pointer<T>::operator*() const {
    return *(ptr->content);
}

template <class T>
T* Pointer<T>::operator->() const {
    return ptr->content;
}

template <class T>
Pointer<T>& Pointer<T>::operator=(const Pointer<T>& assignment) {
    ptr = assignment.ptr;
    if (ptr) {
        ptr->ref_cnt++;
    }
    return *this;
}

template <class T>
bool Pointer<T>::operator==(const Pointer<T>& other) const {
    return ptr->content == (other.ptr)->content;
}

template <class T>
bool Pointer<T>::operator!=(const Pointer<T>& other) const {
    return ptr->content != (other.ptr)->content;
}

// Generally, "new" will not return a NULL pointer,
// so the two overload here will return a not NULL result.
template <class T>
bool Pointer<T>::operator==(const int comp) const {
    return (!(ptr->content) && comp == 0) ? 1 : 0;
}

template <class T>
bool Pointer<T>::operator!=(const int comp) const {
    return (!(ptr->content) && comp == 0) ? 0 : 1;
}

template <class T>
void free(Pointer<T>& obj) {
    std::cout << "Freeing address: " << (obj.ptr)->content << std::endl;
    if (!((obj.ptr)->content)) {
        dangling_pointer_error(obj);
    }
    else {
        free((obj.ptr)->content);
        (obj.ptr)->content = NULL;
    }
}


#endif // __TOMBSTONES_H__
