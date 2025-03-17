#include <stdio.h>

// Function to generate Hamming Code (1-indexed)
void generateHammingCode(int data[4], int hamming[8]) {
    // Data bits (1-indexed)
    hamming[3] = data[0]; // D1
    hamming[5] = data[1]; // D2
    hamming[6] = data[2]; // D3
    hamming[7] = data[3]; // D4

    // Parity bits (1-indexed)
    hamming[1] = hamming[3] ^ hamming[5] ^ hamming[7]; // P1
    hamming[2] = hamming[3] ^ hamming[6] ^ hamming[7]; // P2
    hamming[4] = hamming[5] ^ hamming[6] ^ hamming[7]; // P4

    printf("Generated Hamming Code: ");
    for (int i = 1; i <= 7; i++)
        printf("%d ", hamming[i]);
    printf("\n");
}

// Function to detect and correct errors (1-indexed)
void detectAndCorrect(int received[8]) {
    int p1 = received[1] ^ received[3] ^ received[5] ^ received[7];
    int p2 = received[2] ^ received[3] ^ received[6] ^ received[7];
    int p4 = received[4] ^ received[5] ^ received[6] ^ received[7];

    int errorPos = p1 * 1 + p2 * 2 + p4 * 4; // Calculate error position

    if (errorPos == 0) {
        printf("No error detected.\n");
    } else {
        printf("Error at position %d\n", errorPos);
        received[errorPos] ^= 1; // Correct the bit
        printf("Corrected Hamming Code: ");
        for (int i = 1; i <= 7; i++)
            printf("%d ", received[i]);
        printf("\n");
    }
}

int main() {
    int data[4], hamming[8], received[8];

    // Input 4-bit data
    printf("Enter 4-bit data (space-separated): ");
    for (int i = 0; i < 4; i++)
        scanf("%d", &data[i]);

    // Generate Hamming Code
    generateHammingCode(data, hamming);

    // Input received 7-bit Hamming Code
    printf("Enter received 7-bit Hamming code (space-separated, 1-indexed): ");
    for (int i = 1; i <= 7; i++)
        scanf("%d", &received[i]);

    // Detect and correct errors
    detectAndCorrect(received);

    return 0;
}
