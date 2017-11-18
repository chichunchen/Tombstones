/////////////////////////////////////////////////////////////////////////////
// tombstones.h, expected interface for CS254 assignment 5
/////////////////////////////////////////////////////////////////////////////

#if !defined(__TOMBSTONES_H__)
#define __TOMBSTONES_H__
#include <iostream>
#include <cstring>

template <class T, bool check> class Pointer;
template <class T, bool check> void free(Pointer<T, check>& obj);
template <class T, bool check> bool operator==(const int lhs, const Pointer<T, check>& rhs);
template <class T, bool check> bool operator!=(const int lhs, const Pointer<T, check>& rhs);

// Print dangling pointer error message and gracefully exit
void dangling_pointer_error() {
    std::cerr << "Dangling reference" << std::endl;
    exit(1);
    // std::terminate();
}

// Print memory leak error message and gracefully exit
void leak_memory_error() {
    std::cerr << "Memory leak" << std::endl;
    exit(1);
    // std::terminate();
}

// Defintion of Tomb for Pointer class
template <class T>
struct Tomb {
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

// Tombstone implementation
template <class T, bool check>
class Pointer {
protected:
    Tomb<T>* tomb;
    T* raw_ptr;

public:

    Pointer<T, check>();                               // default constructor
    Pointer<T, check>(Pointer<T, check>&);                        // copy constructor
    Pointer<T, check>(T*);                             // bootstrapping constructor
        // argument should always be a call to new
    ~Pointer<T, check>();                              // destructor
    T& operator*() const;                   // deferencing
    T* operator->() const;                  // field dereferencing
    Pointer<T, check>& operator=(const Pointer<T, check>&);       // assignment
//    Pointer<T>& operator=(T*);                // assignment2
    friend void free<T, check>(Pointer<T, check>&);           // delete pointed-at object
        // This is essentially the inverse of the new inside the call to
        // the bootstrapping constructor.
    // equality comparisons:
    bool operator==(const Pointer<T, check>&) const;
    bool operator!=(const Pointer<T, check>&) const;
    bool operator==(const int) const;
        // true iff Pointer is null and int is zero
    bool operator!=(const int) const;
        // false iff Pointer is null and int is zero
    
    // overloading for (0 == p) and (0 != p) cases.
    template <class U> friend bool operator==(const int, const Pointer<U, check>&);
    template <class U> friend bool operator!=(const int, const Pointer<U, check>&);
};

// default constructor
// create a bare-bone tomb for later use
template <class T, bool check>
Pointer<T, check>::Pointer() {
    if (check) {
        std::cout << "default constructor" << std::endl;
        tomb = new Tomb<T>;
        tomb->content = NULL;
        tomb->ref_cnt = 0;
    } else {
        raw_ptr = NULL;
    }
}

// copy constructor
// tomb points to the argument's tomb
// if the content is not null, then we add the reference count for their shared tomb.
template <class T, bool check>
Pointer<T, check>::Pointer(Pointer<T, check> &p) {
    if (check) {
        std::cout << "copy constructor" << std::endl;

        *&tomb = p.tomb;

        if (!(tomb->content)) {
            tomb->ref_cnt = 0;
        }
        else {
            tomb->ref_cnt++;
        }
    } else {
        raw_ptr = p.raw_ptr;
    }
}

// bootstrap constructor
// tomb's content point to the given pointer
// if the pointer is null then we don't ref count it.
template <class T, bool check>
Pointer<T, check>::Pointer(T* p) {
    if (check) {
        std::cout << "bootstrap constructor" << std::endl;

        tomb = new Tomb<T>;
        tomb->content = p;

        // reference count if p is not null
        if (!(tomb->content)) {
            tomb->ref_cnt = 0;
        }
        else {
            tomb->ref_cnt = 1;
        }
    }
    else {
        raw_ptr = p;
    }
}

// destructor
// This function is not for freeing the memory in tomb->content
// It's for detecting memory leak and tracing of reference count
template <class T, bool check>
Pointer<T, check>::~Pointer() {
    if (check) {
        std::cout << "destructor address: " << this << std::endl;

        // memory leak if reference count is 1
        // if there are multiple Pointer have a same tomb (achieved by bootstrap constructor)
        // then each will minus reference count by one and finally trigger the memory leak message
        if (--(tomb->ref_cnt) == 0) {
            leak_memory_error();
        }
    } else {

    }
}

// deference
template <class T, bool check>
T& Pointer<T, check>::operator*() const {
    if (check) {
        std::cout << "deference" << std::endl;
        if (!tomb->content) {
            dangling_pointer_error();
        }
        return *(tomb->content);
    } else {
        return *raw_ptr;
    }
}

// field deference
template <class T, bool check>
T* Pointer<T, check>::operator->() const {
    if (check) {
        return tomb->content;
    } else {
        return raw_ptr;
    }
}

// assignment
// check leak memory error and then point to the tomb of assignment
template <class T, bool check>
Pointer<T, check>& Pointer<T, check>::operator=(const Pointer<T, check>& assignment) {
    std::cout << check << std::endl;
    if (check) {
        std::cout << "assignment" << std::endl;

        // TODO: it simply invoke leak memory if reference count is one
        // Might have problem in test6
        if (--(tomb->ref_cnt) == 0) {
            leak_memory_error();
        }
        tomb = assignment.tomb;

        // if right hand side has tomb and content is not null
        // we then add the reference count
        if (tomb && tomb->content) {
            tomb->ref_cnt++;
        }
        return *this;
    } else {
        raw_ptr = *&assignment.raw_ptr;
        return *this;
    }
}

template <class T, bool check>
bool Pointer<T, check>::operator==(const Pointer<T, check>& other) const {
    if (check) {
        return tomb->content == (other.tomb)->content;
    } else {
        return raw_ptr == other.raw_ptr;
    }
}

template <class T, bool check>
bool Pointer<T, check>::operator!=(const Pointer<T, check>& other) const {
    if (check) {
        return tomb->content != (other.tomb)->content;
    } else {
        return raw_ptr != raw_ptr;
    }
}

// For foo == 0
// Generally, "new" will not return a NULL pointer,
// so the two overload here will return a not NULL result.
template <class T, bool check>
bool Pointer<T, check>::operator==(const int comp) const {
    if (check) {
        return (!(tomb->content) && comp == 0) ? 1 : 0;
    } else {
        return (!raw_ptr && comp == 0) ? 1 : 0;
    }
}

// For foo != 0
template <class T, bool check>
bool Pointer<T, check>::operator!=(const int comp) const {
    if (check) {
        std::cout << "comp " << comp << " !=, tomb->content " << tomb->content << std::endl;
        return (!(tomb->content) && comp == 0) ? 0 : 1;
    } else {
        return (!raw_ptr && comp == 0) ? 0 : 1;
    }
}

template <class T, bool check>
bool operator==(const int lhs, const Pointer<T, check>& rhs) {
    if (check) {
        return (!(rhs.tomb->content) && lhs == 0) ? 1 : 0;
    } else {
        return (!rhs.raw_ptr && lhs == 0) ? 1 : 0;
    }
}

template <class T, bool check>
bool operator!=(const int lhs, const Pointer<T, check>& rhs) {
    if (check) {
        // std::cout << "comp " << comp << " !=, tomb->content " << tomb->content << std::endl;
        return (!(rhs.tomb->content) && lhs == 0) ? 0 : 1;
    } else {
        return (!rhs.raw_ptr && lhs == 0) ? 0 : 1;
    }
}

// This free function is the opposite of new, not malloc
// Call delete on the content of tomb if pass runtime semantic check
// The runtime semantic check we do now is only allow
template <class T, bool check>
void free(Pointer<T, check>& obj) {
    if (check) {
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
    } else {
        delete obj.raw_ptr;
    }
}


#endif // __TOMBSTONES_H__
