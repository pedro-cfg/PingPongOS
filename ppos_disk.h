// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.2 -- Julho de 2017

// interface do gerente de disco rígido (block device driver)

#ifndef __DISK_MGR__
#define __DISK_MGR__

// estruturas de dados e rotinas de inicializacao e acesso
// a um dispositivo de entrada/saida orientado a blocos,
// tipicamente um disco rigido.

// estrutura que representa um disco no sistema operacional
typedef struct
{
  int current_position;
  int total_blocks;
  int initial_time;
  int total_time;
} disk_t ;

typedef struct requisition
{
  struct requisition* nex;
  struct requisition* pre;
  int id;
  task_t* task;
  int block;
} requisition;

// inicializacao do gerente de disco
// retorna -1 em erro ou 0 em sucesso
// numBlocks: tamanho do disco, em blocos
// blockSize: tamanho de cada bloco do disco, em bytes
int disk_mgr_init (int *numBlocks, int *blockSize) ;

// leitura de um bloco, do disco para o buffer
int disk_block_read (int block, void *buffer) ;

// escrita de um bloco, do buffer para o disco
int disk_block_write (int block, void *buffer) ;

static void signal_handler(int signum);

requisition* createRequisition(int block, task_t* task);
void deleteRequisition(requisition* r);

void first_come_first_served();

int get_total_time();
int get_total_blocks();

#endif
