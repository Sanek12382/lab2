#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

#include <signal.h>
#include <unistd.h>

#include <iostream>
#include <chrono>
#include "SharedMemory.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Parent process" << endl;

    shared_data* shdata = attachMemoryBlock(FILENAME);

    if (sem_init(&shdata->prod, !0, 0) == -1) {
        cout << "Semaphore error" << endl;
    }
    if (sem_init(&shdata->cons, !0, 1) == -1) {
        cout << "Semaphore error" << endl;
    }

    auto start = chrono::high_resolution_clock::now();

    int readerPid = fork();
    if (readerPid == 0) {
        char* readerArgs[] = {"./reader.out", argv[1], NULL};
        execvp("./reader.out", readerArgs);
    }

    sleep(0.5);

    int writerPid = fork();
    if (writerPid == 0) {
        char* writerArgs[] = {"./writer.out", argv[2], NULL};
        execvp("./writer.out", writerArgs);
    }

    while(wait(NULL) > 0);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "The duration is " << duration.count() << " milliseconds" << endl;

    destroyMemoryBlock(FILENAME);
    return 0;
}