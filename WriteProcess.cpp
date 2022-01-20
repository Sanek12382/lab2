#include <iostream>
#include "SharedMemory.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "usage " << argv[0] << " [fileName]" << endl;
        return -1;
    } else {
        cout << "Start the writer process with file " << argv[1] << endl;
    }

    shared_data* block = attachMemoryBlock(FILENAME);
    FILE* fp = fopen(argv[1], "wb");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    while (1) {
        sem_wait(&block->prod);

        if (block->data[0] == 0) {
            break;
        }

        int success = fputs(block->data, fp);

        if (success == EOF) {
            exit(EXIT_FAILURE);
        }

        block->data[0] = 0;
        block->occupied = 0;

        sem_post(&block->cons);
    }

    cout << "Writer process is done" << endl;

    fclose(fp);
    detachMemoryBlock(FILENAME);
    return 0;
}