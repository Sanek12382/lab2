#include <iostream>
#include "SharedMemory.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "usage " << argv[0] << " [fileName]" << endl;
        return -1;
    } else {
        cout << "Start the reader process with file " << argv[1] << endl;
    }

    shared_data* block = attachMemoryBlock(FILENAME);
    FILE* fp = fopen(argv[1], "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    while (1) {
        sem_wait(&block->cons);

        int itemsread = fread(block->data, sizeof(char), 10, fp);
        block->data[itemsread] = 0;
        block->occupied = itemsread + 1;

        sem_post(&block->prod);

        if (itemsread == 0) {
            break;
        }
    }

    cout << "Reader process is done" << endl;

    fclose(fp);
    detachMemoryBlock(FILENAME);
    return 0;
}