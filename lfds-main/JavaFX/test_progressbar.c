#include <stdio.h>
#include <unistd.h>

int main() {

    for (int i = 0; i < 101; i += 5) {
        FILE *file = fopen("progressbar.txt", "w");
        fprintf(file, "%d\n", i);
        fclose(file);

        printf("%d\n", i);

        sleep(1);
    }

    return 0;
}
