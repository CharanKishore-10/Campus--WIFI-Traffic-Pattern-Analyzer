#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_LOCATION 50
#define MAX_TIME 20
#define FILE_NAME "wifi_records.dat"

typedef struct {
    int apId;
    char location[MAX_LOCATION];
    int users;
    float dataUsage;
    float duration;
    char time[MAX_TIME];
} WiFiRecord;

void addRecords(WiFiRecord **records, int *n);
void displayRecords(WiFiRecord *records, int n);
void modifyRecord(WiFiRecord *records, int n);
void linearSearch(WiFiRecord *records, int n);
void sortByUsers(WiFiRecord *records, int n);
void binarySearch(WiFiRecord *records, int n);
void statistics(WiFiRecord *records, int n);
void underutilized(WiFiRecord *records, int n);
void saveToFile(WiFiRecord *records, int n);
void loadFromFile(WiFiRecord **records, int *n);
void generateReport(WiFiRecord *records, int n);

void *threadAnalysis(void *arg) {
    WiFiRecord *r = (WiFiRecord *)arg;
    printf("\n[Thread] Concurrent analysis started for AP %d\n", r->apId);
    if (r->users > 50)
        printf("[Thread] High traffic detected.\n");
    else
        printf("[Thread] Traffic is within normal range.\n");
    return NULL;
}

void addRecords(WiFiRecord **records, int *n) {
    int count, i;
    printf("Enter number of records to add: ");
    scanf("%d", &count);

    *records = realloc(*records, (*n + count) * sizeof(WiFiRecord));
    if (*records == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for (i = *n; i < *n + count; i++) {
        printf("\nRecord %d\n", i + 1);
        printf("Access Point ID: ");
        scanf("%d", &(*records)[i].apId);
        printf("Location: ");
        scanf(" %[^\n]", (*records)[i].location);
        printf("Connected Users: ");
        scanf("%d", &(*records)[i].users);
        printf("Data Usage (GB): ");
        scanf("%f", &(*records)[i].dataUsage);
        printf("Connection Duration (minutes): ");
        scanf("%f", &(*records)[i].duration);
        printf("Time Period (example 10:00-11:00): ");
        scanf(" %[^\n]", (*records)[i].time);
    }
    *n += count;
    printf("Records added successfully.\n");
}

void displayRecords(WiFiRecord *records, int n) {
    int i;
    if (n == 0) {
        printf("No records available.\n");
        return;
    }

    printf("\n%-6s %-20s %-8s %-12s %-12s %-15s\n",
           "AP ID", "Location", "Users", "Data(GB)", "Duration", "Time");
    for (i = 0; i < n; i++) {
        printf("%-6d %-20s %-8d %-12.2f %-12.2f %-15s\n",
               records[i].apId, records[i].location, records[i].users,
               records[i].dataUsage, records[i].duration, records[i].time);
    }
}

void modifyRecord(WiFiRecord *records, int n) {
    int id, i;
    printf("Enter AP ID to modify: ");
    scanf("%d", &id);

    for (i = 0; i < n; i++) {
        if (records[i].apId == id) {
            printf("Enter new connected users: ");
            scanf("%d", &records[i].users);
            printf("Enter new data usage: ");
            scanf("%f", &records[i].dataUsage);
            printf("Record updated successfully.\n");
            return;
        }
    }
    printf("Record not found.\n");
}

void linearSearch(WiFiRecord *records, int n) {
    int id, i;
    printf("Enter AP ID to search: ");
    scanf("%d", &id);

    for (i = 0; i < n; i++) {
        if (records[i].apId == id) {
            printf("Found: AP %d | %s | Users: %d | Data: %.2f GB\n",
                   records[i].apId, records[i].location,
                   records[i].users, records[i].dataUsage);
            return;
        }
    }
    printf("Record not found.\n");
}

void sortByUsers(WiFiRecord *records, int n) {
    int i, j;
    WiFiRecord temp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (records[j].users < records[j + 1].users) {
                temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
    printf("Records sorted by user count in descending order.\n");
}

void binarySearch(WiFiRecord *records, int n) {
    int id, low = 0, high = n - 1, mid;

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (records[j].apId > records[j + 1].apId) {
                WiFiRecord t = records[j];
                records[j] = records[j + 1];
                records[j + 1] = t;
            }

    printf("Enter AP ID for binary search: ");
    scanf("%d", &id);

    while (low <= high) {
        mid = (low + high) / 2;
        if (records[mid].apId == id) {
            printf("Found: AP %d | %s | Users: %d\n",
                   records[mid].apId, records[mid].location, records[mid].users);
            return;
        } else if (records[mid].apId < id)
            low = mid + 1;
        else
            high = mid - 1;
    }
    printf("Record not found.\n");
}

void statistics(WiFiRecord *records, int n) {
    int i, totalUsers = 0, maxIndex = 0;
    float totalData = 0;

    if (n == 0) {
        printf("No records available.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        totalUsers += records[i].users;
        totalData += records[i].dataUsage;
        if (records[i].users > records[maxIndex].users)
            maxIndex = i;
    }

    printf("\nTotal Connected Users: %d\n", totalUsers);
    printf("Average Data Usage: %.2f GB\n", totalData / n);
    printf("Highest Utilized AP: %d (%s)\n",
           records[maxIndex].apId, records[maxIndex].location);
    printf("Peak Traffic Period: %s\n", records[maxIndex].time);
}

void underutilized(WiFiRecord *records, int n) {
    int i, found = 0;
    printf("\nUnderutilized APs (less than 10 users):\n");
    for (i = 0; i < n; i++) {
        if (records[i].users < 10) {
            printf("AP %d - %s - %d users\n",
                   records[i].apId, records[i].location, records[i].users);
            found = 1;
        }
    }
    if (!found)
        printf("No underutilized access points found.\n");
}

void saveToFile(WiFiRecord *records, int n) {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return;
    }
    fwrite(&n, sizeof(int), 1, fp);
    fwrite(records, sizeof(WiFiRecord), n, fp);
    fclose(fp);
    printf("Records saved successfully.\n");
}

void loadFromFile(WiFiRecord **records, int *n) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    fread(n, sizeof(int), 1, fp);
    *records = realloc(*records, (*n) * sizeof(WiFiRecord));
    fread(*records, sizeof(WiFiRecord), *n, fp);
    fclose(fp);
    printf("Records loaded successfully.\n");
}

void generateReport(WiFiRecord *records, int n) {
    int i, totalUsers = 0, maxIndex = 0;
    float totalData = 0;
    FILE *fp;

    if (n == 0) {
        printf("No records available.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        totalUsers += records[i].users;
        totalData += records[i].dataUsage;
        if (records[i].users > records[maxIndex].users)
            maxIndex = i;
    }

    fp = fopen("traffic_report.txt", "w");
    if (fp == NULL) {
        printf("Cannot create report file.\n");
        return;
    }

    fprintf(fp, "CAMPUS WI-FI TRAFFIC ANALYSIS REPORT\n");
    fprintf(fp, "===================================\n");
    fprintf(fp, "Total Records: %d\n", n);
    fprintf(fp, "Total Users: %d\n", totalUsers);
    fprintf(fp, "Average Data Usage: %.2f GB\n", totalData / n);
    fprintf(fp, "Highest Utilized AP: %d (%s)\n",
            records[maxIndex].apId, records[maxIndex].location);
    fprintf(fp, "Peak Traffic Period: %s\n", records[maxIndex].time);

    fclose(fp);

    pthread_t tid;
    pthread_create(&tid, NULL, threadAnalysis, &records[maxIndex]);
    pthread_join(tid, NULL);

    printf("Consolidated report generated: traffic_report.txt\n");
}

int main() {
    WiFiRecord *records = NULL;
    int n = 0, choice;

    do {
        printf("\n--- CAMPUS WI-FI TRAFFIC ANALYZER ---\n");
        printf("1. Add Records\n");
        printf("2. Display Records\n");
        printf("3. Modify Record\n");
        printf("4. Linear Search\n");
        printf("5. Sort by Users\n");
        printf("6. Binary Search\n");
        printf("7. Traffic Statistics\n");
        printf("8. Underutilized Access Points\n");
        printf("9. Save to File\n");
        printf("10. Load from File\n");
        printf("11. Generate Report\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addRecords(&records, &n); break;
            case 2: displayRecords(records, n); break;
            case 3: modifyRecord(records, n); break;
            case 4: linearSearch(records, n); break;
            case 5: sortByUsers(records, n); break;
            case 6: binarySearch(records, n); break;
            case 7: statistics(records, n); break;
            case 8: underutilized(records, n); break;
            case 9: saveToFile(records, n); break;
            case 10: loadFromFile(&records, &n); break;
            case 11: generateReport(records, n); break;
            case 12: printf("Exiting program.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 12);

    free(records);
    return 0;
}
# C-campus -WIFI-Traffic-Pattern-Analyzer

