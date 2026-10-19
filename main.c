#include <stdio.h>

int main(void) {
    double num1, num2, result;
    char op;
    char again;

    do {
        printf("Enter an expression (e.g., 5 + 3): ");
        if (scanf("%lf %c %lf", &num1, &op, &num2) != 3) {
            printf("Invalid input.\n");
            while (getchar() != '\n');   /* clear input buffer */
            continue;
        }

        switch (op) {
            case '+':
                result = num1 + num2;
                printf("Result: %.2f\n", result);
                break;
            case '-':
                result = num1 - num2;
                printf("Result: %.2f\n", result);
                break;
            case '*':
                result = num1 * num2;
                printf("Result: %.2f\n", result);
                break;
            case '/':
                if (num2 == 0)
                    printf("Error: Division by zero.\n");
                else
                    printf("Result: %.2f\n", num1 / num2);
                break;
            case '%':
                if ((int)num2 == 0)
                    printf("Error: Modulus by zero.\n");
                else
                    printf("Result: %d\n", (int)num1 % (int)num2);
                break;
            default:
                printf("Error: Unknown operator '%c'.\n", op);
        }

        printf("Calculate again? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    printf("Goodbye!\n");
    return 0;
}
