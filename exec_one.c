#include <unistd.h>  // execl(), execlp()
#include <stdio.h>
int main() /* exec1 */
{
  printf("> Before the execl call\n");
  execl("/bin/date", "date", NULL);  // execlp("date","date",NULL)
  printf("> Couldn't execute 'date'\n");
}
