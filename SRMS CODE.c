#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CREDENTIAL_FILE "credentials.txt"
#define STUDENT_FILE    "students.txt"

struct student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[20];
char currentRole[20];

int  loginsystem(void);
void mainmenu(void);
void admin_menu(void);
void user_menu(void);
void add_student(void);
void display_students(void);
void search_student(void);
void update_student(void);
void delete_student(void);
void change_password(void);
void view_my_record(void);
void list_toppers(void);
int  compare_students_desc(const void *a, const void *b);

int main(void) {
    if (loginsystem()) {
        mainmenu();
    } else {
        printf("Login failed! Exiting...\n");
    }
    return 0;
}

int loginsystem(void) {
    char username[20], password[20];
    char fileUser[20], filepass[20], filerole[10];

    printf("============== LOGIN SCREEN ============\n");
    printf("Enter username: ");
    scanf("%19s", username);
    printf("Enter password: ");
    scanf("%19s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error opening credentials file.\n");
        return 0;
    }

    while (fscanf(fp, "%19s %19s %9s", fileUser, filepass, filerole) != EOF) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filepass) == 0) {
            strcpy(currentUser, fileUser);
            strcpy(currentRole, filerole);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void mainmenu(void) {
    if (strcmp(currentRole, "admin") == 0) {
        admin_menu();
    } else {
        user_menu();
    }
}

void admin_menu(void) {
    int choice;

    while (1) {
        printf("\n=== Admin Menu ===\n");
        printf("1. Add new Student\n");
        printf("2. Display all Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. List toppers (sorted by marks)\n");
        printf("7. Change my password\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        switch (choice) {
            case 1: add_student(); break;
            case 2: display_students(); break;
            case 3: search_student(); break;
            case 4: update_student(); break;
            case 5: delete_student(); break;
            case 6: list_toppers(); break;
            case 7: change_password(); break;
            case 8:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

void user_menu(void) {
    int choice;

    while (1) {
        printf("\n=== User Menu ===\n");
        printf("1. Display all Students\n");
        printf("2. Search Student\n");
        printf("3. View my record\n");
        printf("4. Change my password\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        switch (choice) {
            case 1: display_students(); break;
            case 2: search_student(); break;
            case 3: view_my_record(); break;
            case 4: change_password(); break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

void add_student(void) {
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("Error opening student file.\n");
        return;
    }

    printf("Enter roll number: ");
    scanf("%d", &st.roll);
    printf("Enter name (no spaces): ");
    scanf("%49s", st.name);
    printf("Enter marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);
    printf("Student added successfully.\n");
}

void display_students(void) {
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening student file or no records yet.\n");
        return;
    }

    printf("\n=== Student List ===\n");
    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) != EOF) {
        printf("Roll: %d, Name: %s, Marks: %.2f\n",
               st.roll, st.name, st.marks);
    }
    fclose(fp);
}

void search_student(void) {
    int roll, found = 0;
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening student file.\n");
        return;
    }

    printf("Enter roll number to search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) != EOF) {
        if (st.roll == roll) {
            printf("Student found: Roll: %d, Name: %s, Marks: %.2f\n",
                   st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with roll number %d not found.\n", roll);
    }

    fclose(fp);
}

void update_student(void) {
    int roll, found = 0;
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening student file.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter roll number to update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) != EOF) {
        if (st.roll == roll) {
            printf("Enter new name (no spaces): ");
            scanf("%49s", st.name);
            printf("Enter new marks: ");
            scanf("%f", &st.marks);
            found = 1;
            printf("Student record updated successfully.\n");
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    if (!found) {
        printf("Student with roll number %d not found.\n", roll);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);
}

void delete_student(void) {
    int roll, found = 0;
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening student file.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter roll number to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) != EOF) {
        if (st.roll == roll) {
            found = 1;
            printf("Student with roll number %d deleted successfully.\n", roll);
            continue;   // skip this record
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    if (!found) {
        printf("Student with roll number %d not found.\n", roll);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);
}

void change_password(void) {
    char oldpass[20], newpass[20], conf[20];
    char user[20], pass[20], role[10];
    int found = 0;

    printf("Enter old password: ");
    scanf("%19s", oldpass);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    FILE *temp = fopen("cred_temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening credentials file.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    while (fscanf(fp, "%19s %19s %9s", user, pass, role) != EOF) {
        if (strcmp(user, currentUser) == 0 && strcmp(pass, oldpass) == 0) {
            printf("Enter new password: ");
            scanf("%19s", newpass);
            printf("Confirm new password: ");
            scanf("%19s", conf);
            if (strcmp(newpass, conf) != 0) {
                printf("Passwords do not match. Keeping old password.\n");
                fprintf(temp, "%s %s %s\n", user, pass, role);
            } else {
                fprintf(temp, "%s %s %s\n", user, newpass, role);
                printf("Password changed successfully.\n");
            }
            found = 1;
        } else {
            fprintf(temp, "%s %s %s\n", user, pass, role);
        }
    }

    fclose(fp);
    fclose(temp);
    remove(CREDENTIAL_FILE);
    rename("cred_temp.txt", CREDENTIAL_FILE);

    if (!found) {
        printf("User not found or old password incorrect.\n");
    }
}

void view_my_record(void) {
    struct student st;
    int found = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening student file.\n");
        return;
    }

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) != EOF) {
        if (strcmp(st.name, currentUser) == 0) {
            printf("My record -> Roll: %d, Name: %s, Marks: %.2f\n",
                   st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No record found for user '%s'.\n", currentUser);
    }
}

int compare_students_desc(const void *a, const void *b) {
    const struct student *s1 = (const struct student *)a;
    const struct student *s2 = (const struct student *)b;
    if (s2->marks > s1->marks) return 1;
    if (s2->marks < s1->marks) return -1;
    return 0;
}

void list_toppers(void) {
    struct student arr[500];
    int n = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening student file.\n");
        return;
    }

    while (n < 500 &&
           fscanf(fp, "%d %49s %f",
                  &arr[n].roll, arr[n].name, &arr[n].marks) != EOF) {
        n++;
    }
    fclose(fp);

    if (n == 0) {
        printf("No students to display.\n");
        return;
    }

    qsort(arr, n, sizeof(struct student), compare_students_desc);

    printf("\n=== Toppers (sorted by marks) ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. Roll: %d, Name: %s, Marks: %.2f\n",
               i + 1, arr[i].roll, arr[i].name, arr[i].marks);
    }
}
