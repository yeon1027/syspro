#include <stdio.h> 
#include "bookcreate.h"
int main(int argc, char* argv[])  
{ 
   struct book rec;
   FILE *fp;

   if (argc != 2) {
      fprintf(stderr, "How to use: %s FileName\n",argv[0]);
      exit(1); 
   }

   fp = fopen(argv[1], "wb");
   printf("%-9s %-7s %-4s\n", "id", "boonName", "author", "year", "numofborrow", "borrow"); 
   while (scanf("%d %s %d",&rec.id, rec.boonName, rec.author, rec.year, rec.numofborrow, rec.borrow) == 6) 
      fwrite(&rec, sizeof(rec), 1, fp);

   fclose(fp);
   exit(0);
} 

