#include "ppos.h"
#include "ppos_data.h"
#include "ppos-core-globals.h"
#include "disk.h"
#include "ppos_disk.h"

#include <signal.h>
#include <stddef.h>
#include <math.h>


mutex_t mutex;
task_t* task_queue;
requisition* req_queue;
disk_t disk;
int current_id = 0;

static void signal_handler(int signum)
{
    first_come_first_served();
}

int disk_mgr_init (int *numBlocks, int *blockSize) 
{
    int status_init =  disk_cmd (DISK_CMD_INIT,0,0);
    *numBlocks = disk_cmd (DISK_CMD_DISKSIZE,0,0);
    *blockSize = disk_cmd (DISK_CMD_BLOCKSIZE,0,0);
    disk.current_position = 0;
    disk.initial_time = systemTime;
    disk.total_blocks = 0;
    disk.total_time = 0;
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
    requisition* r = createRequisition(block,taskExec);
    mutex_lock(&mutex);
    int read_status = disk_cmd(DISK_CMD_READ,block,buffer);
    disk.total_blocks = disk.total_blocks + abs(block - disk.current_position);
    disk.current_position = block;
    task_suspend(taskExec,&task_queue);
    task_yield(taskExec);
    mutex_unlock(&mutex);
    deleteRequisition(r);
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
    requisition* r = createRequisition(block,taskExec);
    mutex_lock(&mutex);
    int write_status = disk_cmd(DISK_CMD_WRITE,block,buffer);
    disk.total_blocks = disk.total_blocks + abs(block - disk.current_position);
    disk.current_position = block;
    task_suspend(taskExec,&task_queue);
    task_yield(taskExec);
    mutex_unlock(&mutex);
    deleteRequisition(r);
    return write_status;
}

requisition* createRequisition(int block, task_t* task)
{
    requisition* r = malloc(sizeof(requisition));
    r->block = block;
    r->task = task;
    r->nex = NULL;
    r->pre = NULL;
    r->id = current_id;
    current_id++;
    if(req_queue == NULL)
        req_queue = r;
    else
    {
        requisition* req = req_queue;
        while(req->nex != NULL)
            req = req->nex;
        if(req != r)
        {
            req->nex = r;
            r->pre = req;
        }
    }
    return r;
}

void deleteRequisition(requisition* r)
{
    if(r != NULL)
    {
        if(r->pre)
            r->pre->nex = r->nex;
        if(r->nex)
            r->nex->pre = r->pre;
        r->task = NULL;
        r->nex = NULL;
        r->pre = NULL;
        if(req_queue == r)
            req_queue = NULL;
        free(r); 
    }
}

void first_come_first_served()
{
    requisition* req = req_queue;
    if(req)
    {
        req_queue = req->nex;
        task_resume(task_queue);
    }
}

int get_total_time()
{
    disk.total_time = systemTime - disk.initial_time;
    return disk.total_time;
}

int get_total_blocks()
{
    return disk.total_blocks;
}