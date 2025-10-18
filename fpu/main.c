#include <stdint.h>
#include <stdio.h>

float fpuatan(float x, uint32_t n);

int main()
{
    {
        // 0.78
        float f = fpuatan(1.0, 1000);
        printf("%f\n", f);
    }
    {
        // 0.46
        float f = fpuatan(0.5, 10);
        printf("%f\n", f);
    }
    {
        // -0.61
        float f = fpuatan(-0.7, 10);
        printf("%f\n", f);
    }
    return 0;
}
