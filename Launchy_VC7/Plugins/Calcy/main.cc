#include <stdio.h>
#include <math.h>

     int      yylex (void)
     {
       int c;
     
       /* Skip white space.  */
       while ((c = getchar ()) == ' ' || c == '\t')
         ;
       /* Process numbers.  */
       if (c == '.' || isdigit (c))
         {
           ungetc (c, stdin);
           scanf ("%lf", &yylval);
           return NUM;
         }
       /* Return end-of-input.  */
       if (c == EOF)
         return 0;
       /* Return a single char.  */
       return c;
     } 

void main(void) {
    return yyparse();
}
