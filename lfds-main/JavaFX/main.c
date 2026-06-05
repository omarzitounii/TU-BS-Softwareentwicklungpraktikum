#include <stdio.h>

int main(int argc, char *argv[]) {

    int ds, threads, inserts, delets;
    sscanf(argv[1], "%d", &ds);
    sscanf(argv[2], "%d", &threads);
    sscanf(argv[3], "%d", &inserts);
    sscanf(argv[4], "%d", &delets);

    if (ds == 1) {
        printf("QUEUE:\n");
        //lock_free_benchmark(QUEUE, threads, insertion, deletion);
    }
    else if (ds == 2) {
        printf("STACK:\n");
        //lock_based_benchmark(STACK, threads, insertion, deletion);
    }
    else if (ds == 3) {
        printf("SET:\n");
        //lock_based_benchmark(SET, threads, insertion, deletion);
    } else
        printf("Fehler in Main bei der Übergabe Parameter");

    FILE *fp;
    fp = fopen("test.csv", "w");

    if (fp == NULL) {
        printf("Fehler beim Öffnen der Datei.\n");
        return 1;
    }

    fprintf(fp, "%d\n", ds);
    fprintf(fp, "%d\n", threads);
    fprintf(fp, "%d\n", inserts);
    fprintf(fp, "%d\n", delets);

    fclose(fp);

    printf("Datei erfolgreich geschrieben.\n");

    return 0;
}