#define _POSIX_C_SOURCE 199309L
#include <stdint.h>
#include <stdio.h>
#include <time.h>

float fpu_atan(float x, uint32_t n);
float scalar_atan(float x, uint32_t n);
float simd_atan(float x, uint32_t n);

#define ITERATIONS 1000000
#define BENCHMARK_ITERATIONS 10

static inline double get_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

void benchmark(const char *name, float (*func)(float, uint32_t), float x, uint32_t n)
{
    double total_time = 0;
    float result = 0;

    // Warmup
    for (int i = 0; i < 1000; i++)
    {
        result = func(x, n);
    }

    // Benchmark
    for (int bench = 0; bench < BENCHMARK_ITERATIONS; bench++)
    {
        double start = get_time_ms();
        for (int i = 0; i < ITERATIONS; i++)
        {
            result = func(x, n);
        }
        double end = get_time_ms();
        total_time += (end - start);
    }

    double avg_time = total_time / BENCHMARK_ITERATIONS;
    double time_per_call = avg_time / ITERATIONS;

    printf("%s: %.3f ms total (%.3f ns per call), result = %f\n", name, avg_time, time_per_call * 1000000.0, result);
}

int main()
{
    printf("Performance Benchmark (%d iterations per test, %d runs averaged)\n\n", ITERATIONS, BENCHMARK_ITERATIONS);

    printf("Test 1: atan(1.0, 1000)\n");
    benchmark("fpu_atan    ", fpu_atan, 1.0f, 1000);
    benchmark("scalar_atan ", scalar_atan, 1.0f, 1000);
    benchmark("simd_atan   ", simd_atan, 1.0f, 1000);
    printf("\n");

    printf("Test 2: atan(0.5, 10)\n");
    benchmark("fpu_atan    ", fpu_atan, 0.5f, 10);
    benchmark("scalar_atan ", scalar_atan, 0.5f, 10);
    benchmark("simd_atan   ", simd_atan, 0.5f, 8); // SIMD needs multiple of 4
    printf("\n");

    printf("Test 3: atan(-0.7, 10)\n");
    benchmark("fpu_atan    ", fpu_atan, -0.7f, 10);
    benchmark("scalar_atan ", scalar_atan, -0.7f, 10);
    benchmark("simd_atan   ", simd_atan, -0.7f, 8); // SIMD needs multiple of 4
    printf("\n");

    printf("Test 4: atan(0.5, 100) - more iterations\n");
    benchmark("fpu_atan    ", fpu_atan, 0.5f, 100);
    benchmark("scalar_atan ", scalar_atan, 0.5f, 100);
    benchmark("simd_atan   ", simd_atan, 0.5f, 100);
    printf("\n");

    printf("Test 5: atan(0.25, 20) - small value\n");
    benchmark("fpu_atan    ", fpu_atan, 0.25f, 20);
    benchmark("scalar_atan ", scalar_atan, 0.25f, 20);
    benchmark("simd_atan   ", simd_atan, 0.25f, 20);
    printf("\n");

    printf("Test 6: atan(0.9, 200) - near convergence limit\n");
    benchmark("fpu_atan    ", fpu_atan, 0.9f, 200);
    benchmark("scalar_atan ", scalar_atan, 0.9f, 200);
    benchmark("simd_atan   ", simd_atan, 0.9f, 200);
    printf("\n");

    return 0;
}
