#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct WiFi
{
    int ap_id;
    char location[50];
    int users;
    float data_usage;
    float duration;
    char time[20];
};

struct WiFi records[MAX];
int count = 0;

void addRecord();
void displayRecords();
void searchRecord();
void modifyRecord();
void sortRecords();
void analyzeTraffic();
void saveRecords();
void loadRecords();

int main()
{
    int choice;

    loadRecords();

    do
    {
        printf("\n========================================\n");
        printf(" CAMPUS WI-FI TRAFFIC PATTERN ANALYZER\n");
        printf("========================================\n");
        printf("1. Add Traffic Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Record\n");
        printf("4. Modify Record\n");
        printf("5. Sort by Data Usage\n");
        printf("6. Traffic Analysis\n");
        printf("7. Save Records\n");
        printf("8. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addRecord();
                break;

            case 2:
                displayRecords();
                break;

            case 3:
                searchRecord();
                break;

            case 4:
                modifyRecord();
                break;

            case 5:
                sortRecords();
                break;

            case 6:
                analyzeTraffic();
                break;

            case 7:
                saveRecords();
                break;

            case 8:
                saveRecords();
                printf("Program exited successfully.\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 8);

    return 0;
}

void addRecord()
{
    if(count >= MAX)
    {
        printf("Record limit reached.\n");
        return;
    }

    printf("\nEnter Access Point ID: ");
    scanf("%d", &records[count].ap_id);

    printf("Enter Location: ");
    scanf(" %[^\n]", records[count].location);

    printf("Enter Number of Users: ");
    scanf("%d", &records[count].users);

    printf("Enter Data Usage (GB): ");
    scanf("%f", &records[count].data_usage);

    printf("Enter Connection Duration (minutes): ");
    scanf("%f", &records[count].duration);

    printf("Enter Access Time: ");
    scanf("%s", records[count].time);

    count++;

    printf("Record added successfully.\n");
}

void displayRecords()
{
    int i;

    if(count == 0)
    {
        printf("No records available.\n");
        return;
    }

    printf("\n-----------------------------------------------------------------------\n");
    printf("AP ID\tLocation\t\tUsers\tData(GB)\tDuration\tTime\n");
    printf("-----------------------------------------------------------------------\n");

    for(i = 0; i < count; i++)
    {
        printf("%d\t%-20s\t%d\t%.2f\t\t%.2f\t\t%s\n",
               records[i].ap_id,
               records[i].location,
               records[i].users,
               records[i].data_usage,
               records[i].duration,
               records[i].time);
    }
}

void searchRecord()
{
    int id, i, found = 0;

    printf("\nEnter Access Point ID: ");
    scanf("%d", &id);

    for(i = 0; i < count; i++)
    {
        if(records[i].ap_id == id)
        {
            printf("\nRecord Found\n");
            printf("Access Point ID : %d\n", records[i].ap_id);
            printf("Location        : %s\n", records[i].location);
            printf("Users           : %d\n", records[i].users);
            printf("Data Usage      : %.2f GB\n", records[i].data_usage);
            printf("Duration        : %.2f minutes\n", records[i].duration);
            printf("Access Time     : %s\n", records[i].time);

            found = 1;
            break;
        }
    }

    if(!found)
        printf("Record not found.\n");
}

void modifyRecord()
{
    int id, i, found = 0;

    printf("\nEnter Access Point ID to modify: ");
    scanf("%d", &id);

    for(i = 0; i < count; i++)
    {
        if(records[i].ap_id == id)
        {
            printf("Enter New Location: ");
            scanf(" %[^\n]", records[i].location);

            printf("Enter New Number of Users: ");
            scanf("%d", &records[i].users);

            printf("Enter New Data Usage: ");
            scanf("%f", &records[i].data_usage);

            printf("Enter New Duration: ");
            scanf("%f", &records[i].duration);

            printf("Enter New Access Time: ");
            scanf("%s", records[i].time);

            printf("Record modified successfully.\n");

            found = 1;
            break;
        }
    }

    if(!found)
        printf("Record not found.\n");
}

void sortRecords()
{
    int i, j;
    struct WiFi temp;

    for(i = 0; i < count - 1; i++)
    {
        for(j = 0; j < count - i - 1; j++)
        {
            if(records[j].data_usage < records[j + 1].data_usage)
            {
                temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    printf("\nRecords sorted by data usage successfully.\n");
    displayRecords();
}

void analyzeTraffic()
{
    int i;
    int totalUsers = 0;
    float totalData = 0;
    float averageData;
    int highest = 0;

    if(count == 0)
    {
        printf("No records available for analysis.\n");
        return;
    }

    for(i = 0; i < count; i++)
    {
        totalUsers += records[i].users;
        totalData += records[i].data_usage;

        if(records[i].users > records[highest].users)
            highest = i;
    }

    averageData = totalData / count;

    printf("\n========== TRAFFIC ANALYSIS REPORT ==========\n");
    printf("Total Records          : %d\n", count);
    printf("Total Connected Users  : %d\n", totalUsers);
    printf("Total Data Usage       : %.2f GB\n", totalData);
    printf("Average Data Usage     : %.2f GB\n", averageData);

    printf("\nHighest Utilized Access Point\n");
    printf("AP ID                  : %d\n", records[highest].ap_id);
    printf("Location               : %s\n", records[highest].location);
    printf("Connected Users        : %d\n", records[highest].users);

    printf("\nUnderutilized Access Points:\n");

    for(i = 0; i < count; i++)
    {
        if(records[i].data_usage <= averageData)
        {
            printf("AP %d - %s - %.2f GB\n",
                   records[i].ap_id,
                   records[i].location,
                   records[i].data_usage);
        }
    }

    printf("=============================================\n");
}

void saveRecords()
{
    FILE *fp;

    fp = fopen("wifi_records.dat", "wb");

    if(fp == NULL)
    {
        printf("Unable to save records.\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(records, sizeof(struct WiFi), count, fp);

    fclose(fp);

    printf("Records saved successfully.\n");
}

void loadRecords()
{
    FILE *fp;

    fp = fopen("wifi_records.dat", "rb");

    if(fp == NULL)
        return;

    fread(&count, sizeof(int), 1, fp);
    fread(records, sizeof(struct WiFi), count, fp);

    fclose(fp);
}

