#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


#define MAX_NOTES 100
#define MAX_TEXT 256
#define FISIER "D:/CLion/programe/folder_notite.txt"
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define ANSI_RESET   "\x1b[0m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"


void clear_screen()
{
#ifdef _WIN32
    system("cls");
    /*#else
        system("clear");*/  ///daca sistemul de operare este Linux/MacOs
#endif
}

typedef struct
{
    int day,month,year,hour, minute;
    char text[MAX_TEXT];
} Note;

Note notite[MAX_NOTES];

void salvare_notita_in_fisier(Note notita)
{
    FILE *file=fopen(FISIER,"a");
    if(!file)
    {
        printf("Eroare la deschiderea fisierului in salvare_notita_in_fisier");
        return;
    }
    fprintf(file,"%02d.%02d.%04d %02d:%02d %s\n",notita.day,notita.month,notita.year,notita.hour, notita.minute, notita.text);
    fclose(file);
}

void scriere_notita()
{
    Note new_note;
    clear_screen();
    printf("    ---Adauga o notita---\n\n");

    // Citire cu validare
    int ok = 0;
    while (!ok)
    {
        printf(" Introdu data si ora pentru notita (zz.ll.aaaa hh:mm): ");
        int result = scanf("%d.%d.%d %d:%d", &new_note.day, &new_note.month, &new_note.year, &new_note.hour, &new_note.minute);
        getchar(); // consumă newline

        if (result != 5)
        {
            printf(" " ANSI_RED  "Format invalid."ANSI_RESET" \n Te rog foloseste formatul: zz.ll.aaaa hh:mm\n");
            while (getchar() != '\n'); // curata buffer
            continue;
        }

        if (new_note.day < 1 || new_note.day > 31 ||
            new_note.month < 1 || new_note.month > 12 ||
            new_note.year < 1900 || new_note.year > 2100 ||
            new_note.hour < 0 || new_note.hour > 23 ||
            new_note.minute < 0 || new_note.minute > 59)
        {
            printf(" Data sau ora invalida. Te rog introdu valori corecte.\n");
        }
        else
        {
            ok = 1;
        }
    }

    printf(" Introdu textul notitei (max %d caractere):\n", MAX_TEXT - 1);
    fgets(new_note.text, MAX_TEXT, stdin);
    new_note.text[strcspn(new_note.text, "\n")] = 0;

    salvare_notita_in_fisier(new_note);
    printf(" Notita a fost salvata cu succes!\n");
}

///varianta fara fisier

/*void add_note()
{

    clear_screen();
    printf("    ---Adauga o notita---\n\n");
    printf("Introduceti ziua, luna, anul, ora si minutul: \n");
    printf("Introduceti textul notitei: \n");
    getchar();
    printf("Notita salvata cu succes!\n\n\n");
    getchar();
}*/

///varianta fara fisier

/*void view_notes()
{
    clear_screen();
    printf("    ---Vizualizare notita---\n\n");
    printf("Introduceti ziua, luna si anul: \n\n\n");
    getchar();
}*/

void edit_note()
{
    int d, m, y, h, min;
    char new_text[MAX_TEXT];
    int found = 0;

    clear_screen();
    printf("    ---Editeaza o notita---\n\n");

    // Validare dată
    do
    {
        printf(" Introdu data si ora notitei de editat (zz.ll.aaaa hh:mm): ");
        if (scanf("%d.%d.%d %d:%d", &d, &m, &y, &h, &min) != 5 ||
            d < 1 || d > 31 || m < 1 || m > 12 || y < 1900 || y > 2100 ||
            h < 0 || h > 23 || min < 0 || min > 59)
        {
            printf(" Data sau ora invalida. Incearca din nou.\n");
            while (getchar() != '\n'); // curata buffer
        }
        else break;
    } while (1);

    getchar(); // curata newline

    printf(" Introdu noul text al notitei: ");
    fgets(new_text, MAX_TEXT, stdin);
    new_text[strcspn(new_text, "\n")] = 0;

    FILE *file = fopen(FISIER, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp)
    {
        printf(" Eroare la accesarea fisierului.\n");
        return;
    }

    int fd, fm, fy, fh, fmin;
    char ftext[MAX_TEXT];

    while (fscanf(file, "%d.%d.%d %d:%d", &fd, &fm, &fy, &fh, &fmin) == 5)
    {
        fgetc(file); // consumă spațiu
        fgets(ftext, MAX_TEXT, file);
        ftext[strcspn(ftext, "\n")] = 0;

        if (fd == d && fm == m && fy == y && fh == h && fmin == min)
        {
            fprintf(temp, "%02d.%02d.%04d %02d:%02d %s\n", d, m, y, h, min, new_text);
            found = 1;
        }
        else
        {
            fprintf(temp, "%02d.%02d.%04d %02d:%02d %s\n", fd, fm, fy, fh, fmin, ftext);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FISIER);
    rename("temp.txt", FISIER);

    if (found)
        printf(" Notita a fost editata cu succes.\n");
    else
        printf(" Nu a fost gasita nicio notita la data si ora specificata.\n");
}


void delete_note()
{
    int d, m, y, h, min;
    int found = 0;

    clear_screen();
    printf("    ---Sterge o notita---\n\n");

    // Validare data și oră
    do
    {
        printf(" Introdu data si ora notitei de sters (zz.ll.aaaa hh:mm): ");
        if (scanf("%d.%d.%d %d:%d", &d, &m, &y, &h, &min) != 5 ||
            d < 1 || d > 31 || m < 1 || m > 12 || y < 1900 || y > 2100 ||
            h < 0 || h > 23 || min < 0 || min > 59)
        {
            printf(" Data sau ora invalida. Incearca din nou.\n");
            while (getchar() != '\n'); // curăță bufferul
        }
        else break;
    } while (1);

    getchar(); // consumă newline-ul rămas

    FILE *file = fopen(FISIER, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp)
    {
        printf(" Eroare la accesarea fisierului.\n");
        return;
    }

    int fd, fm, fy, fh, fmin;
    char ftext[MAX_TEXT];

    while (fscanf(file, "%d.%d.%d %d:%d", &fd, &fm, &fy, &fh, &fmin) == 5)
    {
        fgetc(file); // consumă spațiu sau newline
        if (fgets(ftext, MAX_TEXT, file) == NULL)
            break;

        ftext[strcspn(ftext, "\n")] = 0;

        if (fd == d && fm == m && fy == y && fh == h && fmin == min)
        {
            found = 1;
            // NU scriem această notiță în fișierul temporar
            continue;
        }
        else
        {
            fprintf(temp, "%02d.%02d.%04d %02d:%02d %s\n", fd, fm, fy, fh, fmin, ftext);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FISIER);
    rename("temp.txt", FISIER);

    if (found)
        printf(" Notita a fost stearsa cu succes!\n");
    else
        printf(" Nu a fost gasita nicio notita la data si ora specificata.\n");
}


void load_specific_note()
{
    setlocale(LC_ALL, "en_US.UTF-8");

    int d, m, y, h, min;
    clear_screen();
    printf("   ---Vizualizare notita---\n\n");
    printf(" Introdu data si ora notitei cautate (zz.ll.aaaa hh:mm): ");
    scanf("%d.%d.%d %d:%d", &d, &m, &y, &h, &min);
    getchar();

    FILE *file = fopen(FISIER, "r");
    if (!file)
    {
        printf(RED " Fisierul nu a fost gasit sau este gol!\n" RESET);
        return;
    }

    int fd, fm, fy, fh, fmin;
    char ftext[MAX_TEXT];
    int found = 0;

    while (fscanf(file, "%d.%d.%d %d:%d", &fd, &fm, &fy, &fh, &fmin) == 5)
    {
        fgetc(file);
        fgets(ftext, MAX_TEXT, file);
        ftext[strcspn(ftext, "\n")] = 0;

        if (fd == d && fm == m && fy == y && fh == h && fmin == min)
        {
            printf("\n  Data: " GREEN "%02d.%02d.%04d" RESET "  Ora: " YELLOW "%02d:%02d\n" RESET, fd, fm, fy, fh, fmin);
            printf(" Textul notitei: " BLUE "%s\n" RESET, ftext);
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found)
    {
        printf(RED "  Nu s-a gasit nicio notita la data si ora specificata!\n" RESET);
    }
}



void date_notite()
{
    int fd, fm, fy, fh, fmin;
    FILE *file = fopen(FISIER, "r");
    if (!file)
    {
        printf(RED " Fisierul nu a fost gasit sau este gol!\n" RESET);
        return;
    }

    char linie[100];
    while (fgets(linie, sizeof(linie), file))
    {
        if (sscanf(linie, "%d.%d.%d %d:%d", &fd, &fm, &fy, &fh, &fmin) == 5)
        {
            printf(GREEN "%02d.%02d.%04d " RESET YELLOW "%02d:%02d\n" RESET, fd, fm, fy, fh, fmin);
        }
        else
        {
            printf(RED "Linie ignorata: %s" RESET, linie);
        }
    }

    fclose(file);
}


void menu()
{
    int choice;
    do
    {
        clear_screen();
        printf(ANSI_MAGENTA "=== Meniu Notite ===" ANSI_RESET "\n\n");
        printf("1. " ANSI_CYAN "Adauga o notita" ANSI_RESET "\n");
        printf("2. " ANSI_CYAN "Vizualizeaza o notita" ANSI_RESET "\n");
        printf("3. " ANSI_CYAN "Editeaza o notita" ANSI_RESET "\n");
        printf("4. " ANSI_CYAN "Sterge o notita" ANSI_RESET "\n");
        printf("5. " ANSI_CYAN "Afiseaza toate datele" ANSI_RESET "\n");
        printf("6. " ANSI_RED  "Iesire" ANSI_RESET "\n\n");

        printf("Alege o optiune (cifra): ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                scriere_notita();
            break;
            case 2:
                load_specific_note();
            break;
            case 3:
                edit_note();
            break;
            case 4:
                delete_note();
            break;
            case 5:
                date_notite();
            break;
            case 6:
                printf(ANSI_GREEN "Iesire din aplicatie...\n" ANSI_RESET);
            break;
            default:
                printf(ANSI_RED "Optiune invalida!\n" ANSI_RESET);
        }

        if (choice != 6)
        {
            printf("\nApasa Enter pentru a continua...");
            getchar();
        }
    }
    while (choice != 6);
}


int main()
{
    menu();
    return 0;
}
