#include <cstdint>
#include <iostream>

struct Adder
{
    explicit Adder(int32_t x) : x(x)
    {
    }

    int32_t add(int32_t y) const;

  private:
    int32_t x;
};

extern int32_t add(int32_t a, int32_t b);

struct T
{
    T(int32_t x) : x(x)
    {
    }

    virtual int32_t square(int32_t arg) const;

  protected:
    int32_t x;
};

struct T2 : T
{
    T2(int32_t x) : T(x)
    {
    }

    virtual int32_t square(int32_t arg) const override;
};

int32_t T::square(int32_t arg) const
{
    return x * arg;
}

int32_t T2::square(int32_t arg) const
{
    return x + arg;
}

extern int32_t call_square(T const *t, int32_t arg);

int main()
{
    T const *ptr = new T2(10);
    std::cout << call_square(ptr, 3) << std::endl;

    T t(10);
    std::cout << call_square(&t, 3) << std::endl;
    return 0;
}
