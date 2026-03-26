#if defined(__x86_64__) || defined(_M_X64)
#include <immintrin.h>

int _mm256_cvtsi256_si32(__m256i value) {
    return _mm_cvtsi128_si32(_mm256_castsi256_si128(value));
}

void _mm256_storeu2_m128i(__m128i *hi, __m128i *lo, __m256i value) {
    _mm_storeu_si128((__m128i_u *)lo, _mm256_castsi256_si128(value));
    _mm_storeu_si128((__m128i_u *)hi, _mm256_extracti128_si256(value, 1));
}
#endif
