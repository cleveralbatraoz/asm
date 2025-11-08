#include <stdint.h>
#include <stdio.h>

float fpu_atan(float x, uint32_t n);
float scalar_atan(float x, uint32_t n);
float simd_atan(float x, uint32_t n);

float atan_impl(float x, uint32_t n)
{
    float result = 0;
    float current_term = x;
    float denominator = 1;
    const float multiplier = -x * x;
    for (uint32_t i = 0; i < n; ++i)
    {
        result += current_term / denominator;
        current_term *= multiplier;
        denominator += 2;
    }
    return result;
}

int main()
{
    printf("=== Comparison: FPU vs Scalar SSE vs SIMD ===\n\n");

    printf("Test 1: atan(1.0, 1000) - should be ~0.785398\n");
    {
        float f_fpu = fpu_atan(1.0, 1000);
        float f_scalar = scalar_atan(1.0, 1000);
        float f_simd = simd_atan(1.0, 1000);
        float f_c = atan_impl(1.0, 1000);
        printf("  fpu_atan:    %.15f\n", f_fpu);
        printf("  scalar_atan: %.15f\n", f_scalar);
        printf("  simd_atan:   %.15f\n", f_simd);
        printf("  C reference: %.15f\n", f_c);
    }
    printf("\n");

    printf("Test 2: atan(0.5, 10) - should be ~0.463648\n");
    {
        float f_fpu = fpu_atan(0.5, 10);
        float f_scalar = scalar_atan(0.5, 10);
        float f_simd = simd_atan(0.5, 8); // SIMD needs multiple of 4
        float f_c = atan_impl(0.5, 10);
        printf("  fpu_atan:    %.15f\n", f_fpu);
        printf("  scalar_atan: %.15f\n", f_scalar);
        printf("  simd_atan:   %.15f (n=8)\n", f_simd);
        printf("  C reference: %.15f\n", f_c);
    }
    printf("\n");

    printf("Test 3: atan(-0.7, 10) - should be ~-0.610708\n");
    {
        float f_fpu = fpu_atan(-0.7, 10);
        float f_scalar = scalar_atan(-0.7, 10);
        float f_simd = simd_atan(-0.7, 8); // SIMD needs multiple of 4
        float f_c = atan_impl(-0.7, 10);
        printf("  fpu_atan:    %.15f\n", f_fpu);
        printf("  scalar_atan: %.15f\n", f_scalar);
        printf("  simd_atan:   %.15f (n=8)\n", f_simd);
        printf("  C reference: %.15f\n", f_c);
    }
    printf("\n");

    printf("Test 4: atan(0.25, 20) - small value\n");
    {
        float f_fpu = fpu_atan(0.25, 20);
        float f_scalar = scalar_atan(0.25, 20);
        float f_simd = simd_atan(0.25, 20);
        float f_c = atan_impl(0.25, 20);
        printf("  fpu_atan:    %.15f\n", f_fpu);
        printf("  scalar_atan: %.15f\n", f_scalar);
        printf("  simd_atan:   %.15f\n", f_simd);
        printf("  C reference: %.15f\n", f_c);
    }
    printf("\n");

    printf("Test 5: atan(0.9, 100) - near convergence limit\n");
    {
        float f_fpu = fpu_atan(0.9, 100);
        float f_scalar = scalar_atan(0.9, 100);
        float f_simd = simd_atan(0.9, 100);
        float f_c = atan_impl(0.9, 100);
        printf("  fpu_atan:    %.15f\n", f_fpu);
        printf("  scalar_atan: %.15f\n", f_scalar);
        printf("  simd_atan:   %.15f\n", f_simd);
        printf("  C reference: %.15f\n", f_c);
    }
    printf("\n");

    printf("Test 6: atan(-0.5, 50) - negative value, more iterations\n");
    {
        float f_fpu = fpu_atan(-0.5, 50);
        float f_scalar = scalar_atan(-0.5, 50);
        float f_simd = simd_atan(-0.5, 48); // SIMD needs multiple of 4
        float f_c = atan_impl(-0.5, 50);
        printf("  fpu_atan:    %.15f\n", f_fpu);
        printf("  scalar_atan: %.15f\n", f_scalar);
        printf("  simd_atan:   %.15f (n=48)\n", f_simd);
        printf("  C reference: %.15f\n", f_c);
    }
    printf("\n");

    return 0;
}
