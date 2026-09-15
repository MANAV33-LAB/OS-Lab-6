#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_PROC 5
#define MAX_RES 3

int check_safety(int n, int m, int available[], int allocation[MAX_PROC][MAX_RES], int need[MAX_PROC][MAX_RES], int safe_seq[]) {
    int work[MAX_RES], finish[MAX_PROC], i, j, k, count = 0;
    for (i = 0; i < m; i++) work[i] = available[i];
    for (i = 0; i < n; i++) finish[i] = 0;

    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int can_allocate = 1;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) { can_allocate = 0; break; }
                }
                if (can_allocate) {
                    for (k = 0; k < m; k++) work[k] += allocation[i][k];
                    safe_seq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (found == 0) return 0;
    }
    return 1;
}

void request_resources(int p_idx, int request[], int n, int m, int available[], int allocation[MAX_PROC][MAX_RES], int need[MAX_PROC][MAX_RES]) {
    int j, safe_seq[MAX_PROC];
    for (j = 0; j < m; j++) {
        if (request[j] > need[p_idx][j]) { printf(1, "Error: Exceeded max need.\n"); return; }
        if (request[j] > available[j]) { printf(1, "Process P%d must wait.\n", p_idx); return; }
    }
    for (j = 0; j < m; j++) { available[j] -= request[j]; allocation[p_idx][j] += request[j]; need[p_idx][j] -= request[j]; }
    
    if (check_safety(n, m, available, allocation, need, safe_seq)) {
        printf(1, "Request granted safely for Process P%d.\nSafe Sequence: ", p_idx);
        for (j = 0; j < n; j++) printf(1, "P%d ", safe_seq[j]);
        printf(1, "\n");
    } else {
        for (j = 0; j < m; j++) { available[j] += request[j]; allocation[p_idx][j] -= request[j]; need[p_idx][j] += request[j]; }
        printf(1, "Request denied - would lead to unsafe state.\n");
    }
}

int main() {
    int n = 5, m = 3;
    int available[3] = {3, 3, 2};
    int max_matrix[5][3] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
    int allocation[5][3] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
    int need[5][3], i, j, safe_seq[5];

    for (i = 0; i < n; i++) for (j = 0; j < m; j++) need[i][j] = max_matrix[i][j] - allocation[i][j];

    printf(1, "=== Banker's Algorithm ===\n");
    if (check_safety(n, m, available, allocation, need, safe_seq)) {
        printf(1, "Initial State SAFE. Sequence: ");
        for (i = 0; i < n; i++) printf(1, "P%d ", safe_seq[i]);
        printf(1, "\n\n");
    }
    printf(1, "--- Scenario 1 ---\nProcess P1 requests [1, 0, 2]\n");
    int req1[3] = {1, 0, 2}; request_resources(1, req1, n, m, available, allocation, need);

    printf(1, "\n--- Scenario 2 ---\nProcess P0 requests [0, 2, 0]\n");
    int req2[3] = {0, 2, 0}; request_resources(0, req2, n, m, available, allocation, need);
    exit();
}
