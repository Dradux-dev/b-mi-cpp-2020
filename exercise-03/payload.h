#pragma once

#include <memory>

/*
 *  Payload serves for testing containers. It
 *  counts how many instances of this type
 *  currently exist, via a static counter. 
 *
 */
class Payload {

public:
    // data
    float x, y, z;

    // constructors
    Payload(float xx=0, float yy=0, float zz=0) : x(xx), y(yy), z(zz) { count_++; }
    Payload(const Payload& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) { count_++; }
    Payload(const Payload&& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) { count_++; }

    // destructor
    ~Payload()  { count_--; }

    // assignment - keeps count the same 
    Payload& operator=(const Payload& rhs) { x=rhs.x; y=rhs.y; z=rhs.z; return *this; }
    Payload& operator=(const Payload&& rhs) { x=rhs.x; y=rhs.y; z=rhs.z; return *this; }

    // comparison
    bool operator==(const Payload& rhs) const { return eq(x,rhs.x) && eq(y,rhs.y) && eq(z,rhs.z); }
    bool operator!=(const Payload& rhs) const { return ! (*this == rhs); }

    // tell count
    static size_t count() { return count_; }

private:
    static size_t count_;

    // this is a hack, due to the STUPID -ffloat-equal warning
    // I actually want to test for exact equality.
    static bool eq(float a,float b) {
        return !(a<b || b<a);
    }

};


