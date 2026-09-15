#include "types.h"
#include "stat.h"
#include "user.h"

void lock(char *name) { while(mkdir(name) < 0) sleep(1); }
void unlock(char *name) { unlink(name); }

void do_work(int id, char *res1, char *res2) {
    printf(1, "Process %d requesting %s...\n", id, res1);
    lock(res1);
    printf(1, "Process %d acquired %s. Requesting %s...\n", id, res1, res2);
    lock(res2);
    printf(1, "Process %d acquired %s. Working...\n", id, res2);
    sleep(10);
    printf(1, "Process %d releasing resources and finishing.\n", id);
    unlock(res2);
    unlock(res1);
    exit();
}

int main() {
    unlink("disk"); unlink("printer"); unlink("scanner");
    printf(1, "=== Q4: Multi-Resource Sync (Deadlock Prevention via Ordering) ===\n");
    
    // 5 processes, requesting 2 of 3 resources. 
    // Deadlock prevented by strictly requesting in alphabetical order: disk -> printer -> scanner
    for(int i = 1; i <= 5; i++) {
        if(fork() == 0) {
            if(i % 3 == 0) do_work(i, "disk", "printer");
            else if(i % 3 == 1) do_work(i, "printer", "scanner");
            else do_work(i, "disk", "scanner");
        }
    }
    
    for(int i = 0; i < 5; i++) wait();
    printf(1, "All 5 processes completed successfully. No deadlock.\n");
    exit();
}
