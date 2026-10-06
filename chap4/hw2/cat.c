#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
   FILE *fp;
   int c;
   int line;
   int start = 1;
   int number = 0;

   if (argc < 2) {
      fp = stdin;

      while ((c = getc(fp)) != EOF)
         putc(c, stdout);

      fclose(fp);
      return 0;
   }

   if (strcmp(argv[1], "-n") == 0) {
      number = 1;
      start = 2;
   }

   for (int i = start; i < argc; i++) {
      fp = fopen(argv[i], "r");

      if (fp == NULL) {
         fprintf(stderr, "File %s Open Error\n", argv[i]);
         continue;
      }

      line = 1;

      if (number)
         printf("%6d  ", line);

      while ((c = getc(fp)) != EOF) {
         putc(c, stdout);

         if (c == '\n') {
            line++;

            if (number)
               printf("%6d  ", line);
         }
      }

      fclose(fp);
   }

   return 0;
}
