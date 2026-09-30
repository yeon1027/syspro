#include <stdio.h>

int main(int argc, char *argv[])
{
    int c;
    FILE *fp1, *fp2;

    if (argc != 3) {
        fprintf(stderr, "How to use: %s File1 File2\n", argv[0]);
        return 1;
    }

    fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        fprintf(stderr, "File %s Open Error\n", argv[1]);
        return 2;
    }

    fp2 = fopen(argv[2], "w");
    if (fp2 == NULL) {
        fprintf(stderr, "File %s Open Error\n", argv[2]);
        fclose(fp1);
        return 3;
    }

    while ((c = fgetc(fp1)) != EOF) {
        fputc(c, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}
