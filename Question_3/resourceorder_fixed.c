#include "types.h"
#include "stat.h"
#include "user.h"

void lock(char *name) { while(mkdir(name) < 0) sleep(1); }
void unlock(char *name) { unlink(name); }

int main() {
    unlink("lock1"); unlink("lock2");
    printf(1, "=== Deadlock Prevention (Fixed Ordering) ===\n");

    if(fork() == 0) {
        printf(1, "Process A attempting to acquire Lock 1...\n");
        lock("lock1");
        printf(1, "Process A acquired Lock 1.\n");
        sleep(10);
        printf(1, "Process A attempting to acquire Lock 2...\n");
        lock("lock2");
        printf(1, "Process A acquired Lock 2.\n");
        unlock("lock2"); unlock("lock1");
        printf(1, "Process A finished.\n");
        exit();
    } else {
        sleep(2);
        printf(1, "Process B attempting to acquire Lock 1...\n");
        lock("lock1");
        printf(1, "Process B acquired Lock 1.\n");
        sleep(10);
        printf(1, "Process B attempting to acquire Lock 2...\n");
        lock("lock2");
        printf(1, "Process B acquired Lock 2.\n");
        unlock("lock2"); unlock("lock1");
        printf(1, "Process B finished.\n");
        wait();
    }
    exit();
}
