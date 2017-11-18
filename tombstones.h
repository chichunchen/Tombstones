/////////////////////////////////////////////////////////////////////////////
// tombstones.h, expected interface for CS254 assignment 5
/////////////////////////////////////////////////////////////////////////////

#if !defined(__TOMBSTONES_H__)
#define __TOMBSTONES_H__
#include <iostream>
#include <cstring>

template <class T> class Pointer;
template <class T> void free(Pointer<T>& obj);
template <class T> bool operator==(const int lhs, const Pointer<T>& rhs);
template <class T> bool operator!=(const int lhs, const Pointer<T>& rhs); 

void dangling_pointer_error() {
    std::cerr << "Dangling reference" << std::endl;
    exit(1);
    // std::terminate();
}

void leak_memory_error() {
    std::cerr << "Memory leak" << std::endl;
    exit(1);
    // std::terminate();
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

    Pointer<T>();                               // default constructor
    Pointer<T>(Pointer<T>&);                        // copy constructor
    Pointer<T>(T*);                             // bootstrapping constructor
        // argument should always be a call to new
    ~Pointer<T>();                              // destructor
    T& operator*() const;                   // deferencing
    T* operator->() const;                  // field dereferencing
    Pointer<T>& operator=(const Pointer<T>&);       // assignment
//    Pointer<T>& operator=(T*);                // assignment2
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
    
    // overloading for (0 == p) and (0 != p) cases.
    template <class U> friend bool operator==(const int, const Pointer<U>&);
    template <class U> friend bool operator!=(const int, const Pointer<U>&); 
};

// default constructor
template <class T>
Pointer<T>::Pointer() {
    tomb = new Tomb<T>;
    tomb->content = NULL;
    tomb->ref_cnt = 0;
    std::cout << "default constructor" << std::endl;
}

// copy constructor
template <class T>
Pointer<T>::Pointer(Pointer<T> &p) {
    *&tomb = p.tomb;

    if (!(tomb->content)) {
        tomb->ref_cnt = 0;
    }
    else {
        tomb->ref_cnt++;
    }

    std::cout << "copy constructor" << std::endl;
}

template <class T>
Pointer<T>::Pointer(T* p) {
    std::cout << tomb << std::endl;

    tomb = new Tomb<T>;
    tomb->content = p;
    std::cout << "bootstrap: " <<
              (p == NULL) << (tomb->content == NULL) << std::endl;

    if (!(tomb->content)) {
        tomb->ref_cnt = 0;
    }
    else {
        tomb->ref_cnt = 1;
    }
}

template <class T>
Pointer<T>::~Pointer() {
    // Still need consideration.
    std::cout << "destructor address: " << this << std::endl;
    if (tomb->ref_cnt > 0) {
        if (--(tomb->ref_cnt) == 0) {
            leak_memory_error();
        }
    }
}

template <class T>
T& Pointer<T>::operator*() const {
    std::cout << "deference" << std::endl;
    if (!tomb->content) {
        dangling_pointer_error();
    }
    return *(tomb->content);
}

template <class T>
T* Pointer<T>::operator->() const {
    return tomb->content;
}

template <class T>
Pointer<T>& Pointer<T>::operator=(const Pointer<T>& assignment) {
    std::cout << "assignment" << std::endl;
    
    if (tomb->ref_cnt > 0) {
        if (--(tomb->ref_cnt) == 0) {
            leak_memory_error();
        }
    }
    tomb = assignment.tomb;

    if (tomb) {
        if (tomb->content) {
            tomb->ref_cnt++;
        }
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
bool operator==(const int lhs, const Pointer<T>& rhs) {
    return (!(rhs.tomb->content) && lhs == 0) ? 1 : 0;
}

template <class T>
bool operator!=(const int lhs, const Pointer<T>& rhs) {
    // std::cout << "comp " << comp << " !=, tomb->content " << tomb->content << std::endl;
    return (!(rhs.tomb->content) && lhs == 0) ? 0 : 1;
}


template <class T>
void free(Pointer<T>& obj) {
    std::cout << "Freeing address: " << (obj.tomb)->content << std::endl;
    // If reference count is not 1, which means either it's RIP 
    // or there are more than 1 pointers pointing to the object.
    if (obj.tomb->ref_cnt != 1) {
        dangling_pointer_error();
    }
    else {
        delete obj.tomb->content;
        obj.tomb->ref_cnt = 0;
        (obj.tomb)->content = NULL;
    }
}


#endif // __TOMBSTONES_H__
