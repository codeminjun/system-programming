#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 20  // 100,000 × 100,000의 최대 자릿수는 10자리이지만 여유있게 설정

// 큰 수를 저장하는 구조체
typedef struct {
    int digits[MAX_DIGITS];  // 각 자리수 저장 (역순: digits[0]이 일의 자리)
    int length;              // 실제 자릿수
} BigInt;

// BigInt 초기화
void init_bigint(BigInt *num) {
    memset(num->digits, 0, sizeof(num->digits));
    num->length = 0;
}

// 정수를 BigInt로 변환
void int_to_bigint(long long n, BigInt *result) {
    init_bigint(result);
    
    if (n == 0) {
        result->digits[0] = 0;
        result->length = 1;
        return;
    }
    
    int i = 0;
    while (n > 0) {
        result->digits[i] = n % 10;
        n /= 10;
        i++;
    }
    result->length = i;
}

// BigInt를 16진수로 출력
void print_hex(BigInt *num) {
    // BigInt를 10진수로 해석하여 16진수로 변환
    // 100,000 × 100,000 = 10,000,000,000 = 0x2540BE400
    
    // 먼저 10진수 값을 계산 (long long 사용)
    long long decimal_value = 0;
    long long multiplier = 1;
    
    for (int i = 0; i < num->length; i++) {
        decimal_value += num->digits[i] * multiplier;
        multiplier *= 10;
    }
    
    printf("10진수 결과: %lld\n", decimal_value);
    printf("16진수 결과: 0x%llX\n", decimal_value);
}

// BigInt 곱셈 함수 
void big_mult(long long a, long long b, BigInt *result) {
    BigInt num1, num2;
    int_to_bigint(a, &num1);
    int_to_bigint(b, &num2);
    
    init_bigint(result);
    
    // 각 자리수끼리 곱셈
    for (int i = 0; i < num1.length; i++) {
        int carry = 0;
        for (int j = 0; j < num2.length; j++) {
            int pos = i + j;
            int product = num1.digits[i] * num2.digits[j] + result->digits[pos] + carry;
            result->digits[pos] = product % 10;
            carry = product / 10;
        }
        
        // 남은 자리올림 처리
        int pos = i + num2.length;
        while (carry > 0) {
            result->digits[pos] += carry;
            carry = result->digits[pos] / 10;
            result->digits[pos] %= 10;
            pos++;
        }
    }
    
    // 실제 길이 계산
    result->length = num1.length + num2.length;
    while (result->length > 1 && result->digits[result->length - 1] == 0) {
        result->length--;
    }
}

// BigInt를 10진수로 출력 (디버깅용)
void print_decimal(BigInt *num) {
    printf("10진수: ");
    for (int i = num->length - 1; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
    printf("\n");
}

int main() {
    printf("=== 큰 수 곱셈 프로그램 ===\n\n");
    
    long long num1 = 100000;
    long long num2 = 100000;
    
    printf("계산: %lld × %lld\n\n", num1, num2);
    
    BigInt result;
    big_mult(num1, num2, &result);
    
    printf("결과:\n");
    print_decimal(&result);
    print_hex(&result);
    
    printf("\n=== 추가 테스트 ===\n\n");
    
    // 테스트 케이스들
    long long test_cases[][2] = {
        {12, 34},
        {123, 456},
        {999, 999},
        {12345, 6789},
        {50000, 50000}
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_tests; i++) {
        printf("%lld x %lld = ", test_cases[i][0], test_cases[i][1]);
        big_mult(test_cases[i][0], test_cases[i][1], &result);
        
        long long expected = test_cases[i][0] * test_cases[i][1];
        long long calculated = 0;
        long long multiplier = 1;
        
        for (int j = 0; j < result.length; j++) {
            calculated += result.digits[j] * multiplier;
            multiplier *= 10;
        }
        
        printf("%lld (0x%llX) ", calculated, calculated);
        
        if (calculated == expected) {
            printf("정확!\n");
        } else {
            printf("오류! (예상값: %lld)\n", expected);
        }
    }
    
    return 0;
}