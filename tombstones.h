/////////////////////////////////////////////////////////////////////////////
// tombstones.h, expected interface for CS254 assignment 5
/////////////////////////////////////////////////////////////////////////////

#if !defined(__TOMBSTONES_H__)
#define __TOMBSTONES_H__
#include <iostream>
#include <cstring>

template <class T> class Pointer;
template <class T> void free(Pointer<T>& obj);

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
    bool tomb_allocated;                        // true if tomb is allocated
    bool is_null;                               // tomb content is null or not
    bool fleeting = false;

    Pointer<T>();                               // default constructor
    Pointer<T>(Pointer<T>&);                        // copy constructor
    Pointer<T>(T*);                             // bootstrapping constructor
        // argument should always be a call to new
    ~Pointer<T>();                              // destructor
    T& operator*() const;                   // deferencing
    T* operator->() const;                  // field dereferencing
    Pointer<T>& operator=(const Pointer<T>&);       // assignment
    Pointer<T>& operator=(T*);                // assignment2
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

//    if (p.tomb->ref_cnt <= 0) {
//        dangling_pointer_error();
//    }
    if (!(tomb->content)) {
        tomb->ref_cnt = 0;
    }
    else {
        tomb->ref_cnt++;
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

    if (!(tomb->content)) {
        tomb->ref_cnt = 0;
    }
    else {
        tomb->ref_cnt = 1;
    }
}

template <class T>
Pointer<T>::~Pointer() {
    // Need revise
    std::cout << "destructor address: " << this << std::endl;
    tomb->ref_cnt--;
    if (tomb->ref_cnt == 0 && !fleeting)
        leak_memory_error();
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

    tomb->ref_cnt--;
    if (tomb->ref_cnt == 0) {
        //
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
Pointer<T>& Pointer<T>::operator=(T* t) {
    Pointer<T> assignment(t);

    tomb->ref_cnt--;
    tomb = new Tomb<T>;
    tomb->content = assignment.tomb->content;
    tomb->ref_cnt = assignment.tomb->ref_cnt;
    
    assignment.fleeting = true;

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
    // if null and reference count is 0
    if (obj.tomb->ref_cnt != 1) {
        dangling_pointer_error();
    }
    else {
        free((obj.tomb)->content);
        obj.tomb->ref_cnt = 0;
        (obj.tomb)->content = NULL;
    }
}


#endif // __TOMBSTONES_H__
