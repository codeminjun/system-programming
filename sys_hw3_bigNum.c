#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 20  // Maximum digits for 100,000 × 100,000 is 10, but set with margin

// Structure to store large numbers
typedef struct {
    int digits[MAX_DIGITS];  // Store each digit (reversed: digits[0] is ones place)
    int length;              // Actual number of digits
} BigInt;

// Initialize BigInt
void init_bigint(BigInt *num) {
    memset(num->digits, 0, sizeof(num->digits));
    num->length = 0;
}

// Convert integer to BigInt
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

// Print BigInt in hexadecimal
void print_hex(BigInt *num) {
    // Convert BigInt to decimal then to hexadecimal
    // 100,000 × 100,000 = 10,000,000,000 = 0x2540BE400
    
    // First calculate decimal value (using long long)
    long long decimal_value = 0;
    long long multiplier = 1;
    
    for (int i = 0; i < num->length; i++) {
        decimal_value += num->digits[i] * multiplier;
        multiplier *= 10;
    }
    
    printf("Decimal result: %lld\n", decimal_value);
    printf("Hexadecimal result: 0x%llX\n", decimal_value);
}

// BigInt multiplication function
void big_mult(long long a, long long b, BigInt *result) {
    BigInt num1, num2;
    int_to_bigint(a, &num1);
    int_to_bigint(b, &num2);
    
    init_bigint(result);
    
    // Multiply each digit
    for (int i = 0; i < num1.length; i++) {
        int carry = 0;
        for (int j = 0; j < num2.length; j++) {
            int pos = i + j;
            int product = num1.digits[i] * num2.digits[j] + result->digits[pos] + carry;
            result->digits[pos] = product % 10;
            carry = product / 10;
        }
        
        // Handle remaining carry
        int pos = i + num2.length;
        while (carry > 0) {
            result->digits[pos] += carry;
            carry = result->digits[pos] / 10;
            result->digits[pos] %= 10;
            pos++;
        }
    }
    
    // Calculate actual length
    result->length = num1.length + num2.length;
    while (result->length > 1 && result->digits[result->length - 1] == 0) {
        result->length--;
    }
}

// Print BigInt in decimal (for debugging)
void print_decimal(BigInt *num) {
    printf("Decimal: ");
    for (int i = num->length - 1; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Large Number Multiplication Program ===\n\n");
    
    long long num1 = 100000;
    long long num2 = 100000;
    
    printf("Calculation: %lld × %lld\n\n", num1, num2);
    
    BigInt result;
    big_mult(num1, num2, &result);
    
    printf("Result:\n");
    print_decimal(&result);
    print_hex(&result);
    
    printf("\n=== Additional Tests ===\n\n");
    
    // Test cases
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
            printf("Correct!\n");
        } else {
            printf("Error! (Expected: %lld)\n", expected);
        }
    }
    
    return 0;
}