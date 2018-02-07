#include <stdlib.h>
#include <stdio.h>

int main(){
  int a, b, c;
  scanf("%i", &a);
  fflush(stdin); // clear istream
  scanf("%i", &b);
  fflush(stdin); // clear istream
  scanf("%i", &c);
  fflush(stdin); // clear istream
  printf("%i\n", a);
  fflush(stdout); 
  printf("%i\n", b);
  fflush(stdout);
  printf("%i\n", c);
  fflush(stdout);
  return 0;
}