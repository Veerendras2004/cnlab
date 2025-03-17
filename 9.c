#include <stdio.h>
#include <stdlib.h>

#define min(x, y) ((x) < (y) ? (x) : (y))

#define MAX 25

int main() {
    int cap, oprt, cont = 0, i = 0, inp[MAX], ch, nsec, drop;

    printf("LEAKY BUCKET ALGORITHM\n");

    printf("\nEnter the bucket size: ");
    scanf("%d", &cap);

    printf("\nEnter the output rate: ");
    scanf("%d", &oprt);

    // Input packet arrivals
    do {
        printf("\nEnter the number of packets entering at %d second(s): ", i + 1);
        scanf("%d", &inp[i]);
        i++;

        printf("\nEnter 1 to insert packet or 0 to quit: ");
        scanf("%d", &ch);
    } while (ch == 1);

    nsec = i; // Total seconds where packets arrived

    printf("\n(SECOND) | (PACK RECVD) | (PACK SENT) | (PACK LEFT IN BUCKET) | (PACK DROPPED)\n");
    printf("-------------------------------------------------------------------------\n");

    drop = 0;

    for (i = 0; i < nsec; i++) {
        cont += inp[i]; // Add incoming packets to the bucket

        if (cont > cap) { // If overflow occurs, drop excess packets
            drop = cont - cap;
            cont = cap;
        } else {
            drop = 0;
        }

        printf("   %d     |      %d      |      %d      |        %d        |       %d\n",
               i + 1, inp[i], min(cont, oprt), cont - min(cont, oprt), drop);

        cont -= min(cont, oprt); // Send packets according to output rate
    }

    // Empty the bucket after last packet arrival
    while (cont > 0) {
        printf("   %d     |      0      |      %d      |        %d        |       0\n",
               i + 1, min(cont, oprt), cont - min(cont, oprt));
        cont -= min(cont, oprt);
        i++;
    }

    return 0;
}
