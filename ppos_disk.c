#include "ppos.h"
#include "ppos_data.h"
#include "ppos-core-globals.h"
#include "disk.h"
#include "ppos_disk.h"

#include <signal.h>

mutex_t mutex;
task_t* task_queue;

static void signal_handler(int signum)
{
    if(task_queue)
    {
        first_come_first_served();
    }
}

int disk_mgr_init (int *numBlocks, int *blockSize) 
{
    int status_init =  disk_cmd (DISK_CMD_INIT,0,0);
    *numBlocks = disk_cmd (DISK_CMD_DISKSIZE,0,0);
    *blockSize = disk_cmd (DISK_CMD_BLOCKSIZE,0,0);
    mutex_create(&mutex);
    signal(SIGUSR1, signal_handler);
    return status_init;
}

// leitura de um bloco, do disco para o buffer
int disk_block_read (int block, void *buffer)
{
    int status = disk_cmd (DISK_CMD_STATUS,0,0);
    if(status == 0)
    {
        printf("Erro! Disco não inicializado");
        return -1;
    }
    mutex_lock(&mutex);
    int read_status = disk_cmd(DISK_CMD_READ,block,buffer);
    task_suspend(taskExec,&task_queue);
    task_yield();
    mutex_unlock(&mutex);
    return read_status;
}

// escrita de um bloco, do buffer para o disco
int disk_block_write (int block, void *buffer) 
{
    int status = disk_cmd (DISK_CMD_STATUS,0,0);
    if(status == 0)
    {
        printf("Erro! Disco não inicializado");
        return -1;
    }
    mutex_lock(&mutex);
    int write_status = disk_cmd(DISK_CMD_WRITE,block,buffer);
    task_suspend(taskExec,&task_queue);
    task_yield();
    mutex_unlock(&mutex);
    return write_status;
}

void first_come_first_served()
{
    task_resume(task_queue);
}