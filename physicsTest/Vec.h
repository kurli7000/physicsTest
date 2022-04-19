#ifndef Vec_h
#define Vec_h
#include "assert.h"

class Vec
{
public:
    int x, y;
    Vec() {};
    Vec(int xval, int yval) : x(xval), y(yval) {};
    ~Vec() {};
    
    static const int SCALE = 1000;
    
    void Zero() { x = y = 0; }
    bool IsZero() const { return x == 0 && y == 0; }
    
    static uint32_t sqrt(uint32_t input)
    {
        uint32_t op  = input;
        uint32_t res = 0;
        uint32_t one = 1uL << 30; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type
        
        // "one" starts at the highest power of four <= than the argument.
        while (one > op)
        {
            one >>= 2;
        }
            
        while (one != 0)
        {
            if (op >= res + one)
            {
                op = op - (res + one);
                res = res +  2 * one;
            }
            res >>= 1;
            one >>= 2;
        }
        return res;
    }
    
    int GetLength() const
    {
        return sqrt(x * x + y * y);
    }
    
    int GetLengthSquared() const
    {
        return x * x + y * y;
    }
    
    void Normalise()
    {
        int len = GetLength();
        x = x * SCALE / len;
        y = y * SCALE / len;
    }
    
    int Dot(const Vec& v)
    {
        return x * v.x + y * v.y;
    }
    
    int Distance(const Vec &v) const
    {
        int dx = v.x - x;
        int dy = v.y - y;
        
        return sqrt(dx * dx + dy * dy);
    }
    
    void operator=(const Vec &v)
    {
        x = v.x;
        y = v.y;
    }

    Vec operator+(const Vec &v) const
    {
        return Vec(x + v.x, y + v.y);
    }
    
    void operator+=(const Vec &v)
    {
        x += v.x;
        y += v.y;
    }
    
    Vec operator-(const Vec &v) const
    {
        return Vec(x - v.x, y - v.y);
    }
    
    void operator-=(const Vec &v)
    {
        x -= v.x;
        y -= v.y;
    }
    
    Vec operator*(const Vec &v) const
    {
        return Vec(x * v.x, y * v.y);
    }
    
    Vec operator*(const int i) const
    {
        return Vec(x * i, y * i);
    }
    
    void operator*=(const int i)
    {
        x *= i;
        y *= i;
    }
    
    Vec operator/(const int i) const
    {
        return Vec(x / i, y / i);
    }
    
    void operator/=(const int i)
    {
        x /= i;
        y /= i;
    }
        
};

#endif /* Vec_h */
