#include <stdio.h>
#include <stdlib.h>

/* Stand-in for the real setProperty() to test the XML parser in isolation */ 
int NFSv3setProperty( char * name, char * typename, char * valuebuffer)
{
   printf("setProperty( name=\"%s\" typename=\"%s\" valuebuffer=\"%s\" )\n", name, typename, valuebuffer);
   return 1;
}

/* Stand-in for the real setArrayProperty() to test the XML parser in isolation */
int NFSv3setArrayProperty( char * name, char * typename, char * valuebuffer)
{
   printf("setArrayProperty( name=\"%s\" typename=\"%s\" valuebuffer=\"%s\" )\n", name, typename, valuebuffer);
   return 1;
}

int main()
{
   int rc;
   while (!(rc = NFSv3xmlyyparse())) printf("\n"); /* Parse all the XML instances until error or EOF */

   if (rc == EOF) rc = 0;
   exit(rc);
}

