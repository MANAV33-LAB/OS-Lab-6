#include "types.h"
#include "stat.h"
#include "user.h"

#define N 4 
#define M 3 

int wait_for[N][N];
int visited[N];
int recStack[N];
int cycle_path[N];
int cycle_len = 0;

void build_wait_for(int alloc[N][M], int req[N][M]) {
    int p_req, res, p_alloc;
    for(int i=0; i<N; i++) for(int j=0; j<N; j++) wait_for[i][j] = 0;

    for(p_req = 0; p_req < N; p_req++) {
        for(res = 0; res < M; res++) {
            if(req[p_req][res] > 0) { 
                for(p_alloc = 0; p_alloc < N; p_alloc++) {
                    if(alloc[p_alloc][res] > 0 && p_req != p_alloc) {
                        wait_for[p_req][p_alloc] = 1; 
                    }
                }
            }
        }
    }
}

int dfs(int v) {
    if(visited[v] == 0) {
        visited[v] = 1;
        recStack[v] = 1;
        cycle_path[cycle_len++] = v;

        for(int i = 0; i < N; i++) {
            if(wait_for[v][i]) {
                if(!visited[i] && dfs(i)) return 1;
                else if(recStack[i]) {
                    cycle_path[cycle_len++] = i;
                    return 1;
                }
            }
        }
    }
    recStack[v] = 0;
    cycle_len--;
    return 0;
}

void detect_deadlock() {
    for(int i=0; i<N; i++) { visited[i] = 0; recStack[i] = 0; }
    cycle_len = 0;
    int cycle_found = 0;

    for(int i = 0; i < N; i++) {
        if(!visited[i]) {
            if(dfs(i)) {
                cycle_found = 1;
                break;
            }
        }
    }

    if(cycle_found) {
        printf(1, "Deadlock Detected! Cycle: ");
        for(int i = 0; i < cycle_len; i++) printf(1, "P%d -> ", cycle_path[i]);
        printf(1, "P%d\n", cycle_path[cycle_len-1]); 
    } else {
        printf(1, "No deadlock detected. Wait-for graph is acyclic.\n");
    }
}

void print_wait_for() {
    printf(1, "Wait-for Graph Edges:\n");
    int edges = 0;
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            if(wait_for[i][j]) {
                printf(1, "P%d -> P%d\n", i, j);
                edges = 1;
            }
        }
    }
    if(!edges) printf(1, "No edges.\n");
}

int main() {
    printf(1, "=== Deadlock Detection (RAG) ===\n\n");

    printf(1, "--- Scenario 1: No Deadlock ---\n");
    int alloc1[N][M] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0, 0}};
    int req1[N][M]   = {{0, 1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    build_wait_for(alloc1, req1);
    print_wait_for();
    detect_deadlock();

    printf(1, "\n--- Scenario 2: Deadlock (3-Process Circular Wait) ---\n");
    int alloc2[N][M] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};
    int req2[N][M]   = {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}, {0, 0, 0}};
    build_wait_for(alloc2, req2);
    print_wait_for();
    detect_deadlock();

    exit();
}
