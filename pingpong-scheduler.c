// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.1 -- Julho de 2016

// Teste do escalonador por prioridades dinâmicas

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"

task_t Pang, Peng, Ping, Pong, Pung ;

// corpo das threads
void Body (void * arg)
{
  int i ;

  printf ("%s: inicio (eet %d)\n", (char *) arg, task_get_eet(NULL)) ;

  for (i=0; i<10; i++)
  {
    printf ("%s: %d\n", (char *) arg, i) ;
    task_yield ();
  }
  printf ("%s: fim\n", (char *) arg) ;
  task_exit (0) ;
}

int main (int argc, char *argv[])
{
  printf ("main: inicio\n");

  ppos_init () ;

  task_create (&Pang, Body, "    Pang") ;
  task_set_eet (&Pang, 90000);

  task_create (&Peng, Body, "        Peng") ;
  task_set_eet (&Peng, 80000);

  task_create (&Ping, Body, "            Ping") ;
  task_set_eet (&Ping, 40000);

  task_create (&Pong, Body, "                Pong") ;
  task_set_eet (&Pong, 60000);

  task_create (&Pung, Body, "                    Pung") ;
  task_set_eet (&Pung, 50000);

  task_join(&Pang);
  task_join(&Peng);
  task_join(&Ping);
  task_join(&Ping);
  task_join(&Pung);

  printf ("main: fim\n");
  exit (0) ;
}
