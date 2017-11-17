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
    std::cerr << "Dangling reference, address: " << (obj.tomb)->content << std::endl;
//    exit(1);
    std::terminate();
}

template <class T>
void leak_memory_error(Pointer<T>& obj) {
    std::cerr << "Memory leak, address: " << (obj.tomb)->content << std::endl;
    std::terminate();
}

template <class T>
class Tomb {
public:
    T* content;
    int ref_cnt;

    Tomb<T>() {
        content = NULL;
        ref_cnt = 0;
    }
    ~Tomb() {
        content = NULL;
        ref_cnt = 0;
    }
};

template <class T>
class Pointer {
public:
    Tomb<T>* tomb;
    bool tomb_allocated;                        // true if tomb is allocated
    bool is_null;                               // tomb content is null or not

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
    
    T* getPointer() const { return tomb; } // Just for test. Of course we can keep this.
};

// default constructor
template <class T>
Pointer<T>::Pointer() {
    tomb = new Tomb<T>;
    tomb->content = NULL;
    tomb->ref_cnt = 0;
    is_null = true;
    std::cout << "default constructor" << std::endl;
}

// copy constructor
template <class T>
Pointer<T>::Pointer(Pointer<T> &p) {
    *&tomb = p.tomb;
    if (!(tomb->content)) {
        p.tomb->ref_cnt++;
    }
    is_null = p.is_null;
    std::cout << "copy constructor" << std::endl;
}

template <class T>
Pointer<T>::Pointer(T* p) {
    std::cout << tomb << std::endl;

    tomb = new Tomb<T>;
    tomb->content = p;
    is_null = (p == NULL) ? true : false;
    std::cout << "bootstrap: " <<
              (p == NULL) << (tomb->content == NULL) << std::endl;
    tomb->ref_cnt = 1;
}

template <class T>
Pointer<T>::~Pointer() {
    // Need revise
    std::cout << "destructor address: " << this << std::endl;
    if (tomb->ref_cnt > 0)
        leak_memory_error(*this);
}

template <class T>
T& Pointer<T>::operator*() const {
    std::cout << "deference" << std::endl;
    return *(tomb->content);
}

template <class T>
T* Pointer<T>::operator->() const {
    return tomb->content;
}

template <class T>
Pointer<T>& Pointer<T>::operator=(const Pointer<T>& assignment) {
    std::cout << "assignemnt" << std::endl;

    tomb->ref_cnt--;
    tomb = assignment.tomb;
    if (tomb) {
        tomb->ref_cnt++;
    }
    return *this;
}

template <class T>
bool Pointer<T>::operator==(const Pointer<T>& other) const {
    return tomb->content == (other.tomb)->content;
}

template <class T>
bool Pointer<T>::operator!=(const Pointer<T>& other) const {
    return tomb->content != (other.tomb)->content;
}

// Generally, "new" will not return a NULL pointer,
// so the two overload here will return a not NULL result.
template <class T>
bool Pointer<T>::operator==(const int comp) const {
    return (!(tomb->content) && comp == 0) ? 1 : 0;
}

template <class T>
bool Pointer<T>::operator!=(const int comp) const {
    std::cout << "comp " << comp << " !=, tomb->content " << tomb->content << std::endl;
    return (!(tomb->content) && comp == 0) ? 0 : 1;
}

template <class T>
void free(Pointer<T>& obj) {
    std::cout << "Freeing address: " << (obj.tomb)->content << std::endl;
    if (!((obj.tomb)->content)) {
        dangling_pointer_error(obj);
    }
    else {
        free((obj.tomb)->content);
        (obj.tomb)->content = NULL;
    }
}


#endif // __TOMBSTONES_H__
