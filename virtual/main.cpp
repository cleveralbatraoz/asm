#include <cstdint>
#include <iostream>

struct base
{
    explicit base(int16_t x) : x(x)
    {
    }

    virtual int16_t f() const = 0;

  protected:
    int16_t x;
};

struct derived : base
{
    using base::base;

    int16_t f() const override
    {
        return x;
    }
};

extern "C" int16_t call_f(derived *obj);

int main()
{
    uint16_t x = 42;
    derived obj(42);
    if (call_f(&obj) != 42)
    {
        return 1;
    }
    return 0;
}
