#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define FILENAME "ParentProcess.cpp"

typedef struct {
    sem_t prod;
    sem_t cons;
    int occupied = 0;
    char data[1024];
} shared_data;

int getSharedBlock(char* filename) {
    key_t key = ftok(filename, 0);
    
    if (key == -1) {
        return -1;
    }

    return shmget(key, sizeof(shared_data), 0644 | IPC_CREAT);
}

shared_data* attachMemoryBlock(char* filename) {
    int sharedBlockId = getSharedBlock(filename);

    if (sharedBlockId == -1) {
        return nullptr;
    }

    shared_data* result = (shared_data*) shmat(sharedBlockId, nullptr, 0);
    return result;
}

bool detachMemoryBlock(char* block) {
    return (shmdt(block) != -1);
}

bool destroyMemoryBlock(char* filename) {
    int sharedBlockId = getSharedBlock(filename);

    if (sharedBlockId == -1) {
        return false;
    }

    return (shmctl(sharedBlockId, IPC_RMID, 0) != -1);
}