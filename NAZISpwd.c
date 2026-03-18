#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MIN_OUTPUT 1000000
#define MAX_OUTPUT 50000000
#define MAX_LEN 128

typedef struct {
    const char *years[35];
    const char *numbers[35];
    const char *specials[45];
    const char *suffixes[30];
} Patterns;

Patterns patterns = {
    .years={"2020","2021","2022","2023","2024","2025","2026","2027","2028","2029","2019","2018","2017","2016","2015","2010","2009","2008","2007","2000","1999","1998","1997","1990","1989","1980","1979","1991","1992","1993",NULL},
    .numbers={"1","12","123","1234","12345","123456","1234567","12345678","123456789","0","01","012","0123","01234","987654321","111","222","333","444","555","666","777","888","999","100","200","300","400","500","1000",NULL},
    .specials={"!","@","#","$","%","^","&","*","(",")","-","_","+","=","{","}","[","]","|",":",";","<",">",",",".","?","/","`","~","!!","@@","##","$$","%%","^^","&&","**","((",NULL},
    .suffixes={"123","1234","12345","!","@","#","$","pass","pwd","admin","user","test","abc","xyz","qwe","asd","1","12","123a","a123","Ah123","test","login","root","sudo",NULL}
};

int total_generated = 0;
long long target_count;
FILE *output_file;

void generate_patterns(const char *base);
void generate_random(const char *base);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <base> <count_M> <output>\n", argv[0]);
        printf("Examples:\n  %s ahmed 10 ahmed_10M.txt\n  %s admin 50 admin_50M.txt\n", argv[0], argv[0]);
        return 1;
    }
    
    int count_m = atoi(argv[2]);
    if (count_m < 1 || count_m > 50) {
        printf("Count must be 1-50M\n");
        return 1;
    }
    
    target_count = (long long)count_m * 1000000LL;
    output_file = fopen(argv[3], "w");
    if (!output_file) { perror("File error"); return 1; }
    
    printf("🔥 Generating %lld pw for '%s' -> %s\n", target_count, argv[1], argv[3]);
    clock_t start = clock();
    srand(time(NULL));
    
    generate_patterns(argv[1]);
    generate_random(argv[1]);
    
    fclose(output_file);
    printf("\n✅ %d/%lld saved (%.1fs)\n", total_generated, target_count, 
           (clock()-start)/(double)CLOCKS_PER_SEC);
    return 0;
}

void generate_patterns(const char *base) {
    char pass[MAX_LEN];
    fprintf(output_file, "%s\n", base); total_generated++;
    
    // Capitalize
    snprintf(pass, MAX_LEN, "%s", base); 
    if (pass[0]) pass[0] = toupper(pass[0]);
    fprintf(output_file, "%s\n", pass); total_generated++;
    
    // Patterns
    for (int i = 0; patterns.suffixes[i] && total_generated < target_count; i++) {
        fprintf(output_file, "%s%s\n", base, patterns.suffixes[i]); total_generated++;
    }
    for (int i = 0; patterns.numbers[i] && total_generated < target_count; i++) {
        fprintf(output_file, "%s%s\n", patterns.numbers[i], base); total_generated++;
        fprintf(output_file, "%s%s\n", base, patterns.numbers[i]); total_generated++;
    }
    for (int i = 0; patterns.years[i] && total_generated < target_count; i++) {
        fprintf(output_file, "%s%s\n", base, patterns.years[i]); total_generated++;
    }
}

void generate_random(const char *base) {
    char pass[MAX_LEN];
    while (total_generated < target_count) {
        int t = rand() % 20;
        switch(t) {
            case 0: case 1: snprintf(pass, MAX_LEN, "%s%08d", base, rand()%100000000); break;
            case 2: snprintf(pass, MAX_LEN, "%08d%s", rand()%100000000, base); break;
            case 3: snprintf(pass, MAX_LEN, "%s%s%05d", base, patterns.specials[rand()%40], rand()%100000); break;
            case 4: snprintf(pass, MAX_LEN, "%s%d%d%d", base, rand()%100,rand()%100,rand()%100); break;
            case 5: snprintf(pass, MAX_LEN, "%s_%d", base, rand()%100000); break;
            case 6: snprintf(pass, MAX_LEN, "%s%s", base, base); break;
            case 7: snprintf(pass, MAX_LEN, "%s%d!", base, 1970+rand()%57); break;
            default: snprintf(pass, MAX_LEN, "%s%s%d", base, patterns.specials[rand()%40], rand()%10000); break;
        }
        fprintf(output_file, "%s\n", pass);
        total_generated++;
        
        if (total_generated % 1000000 == 0) {
            printf("%d/%lld\r", total_generated, target_count);
            fflush(stdout); fflush(output_file);
        }
    }
}
