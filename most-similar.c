#define BUFSIZE 512

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * This implementation allows the costs to be weighted:
 *
 * - w (as in "sWap")
 * - s (as in "Substitution")
 * - a (for insertion, AKA "Add")
 * - d (as in "Deletion")
 *
 */
double calc_similarity(const char *string1, const char *string2)
{
    int len1 = strlen(string1),
        len2 = strlen(string2);
    double w = 0.75, s = 1, a = 1, d = 1,
           *row0 = (double *)&(double[BUFSIZE]){},
           *row1 = (double *)&(double[BUFSIZE]){},
           *row2 = (double *)&(double[BUFSIZE]){};

    for (int j = 0; j <= len2; ++j) {
        row1[j] = j * a;
    }

    for (int i = 0; i < len1; ++i) {
        row2[0] = (i + 1) * d;
        for (int j = 0; j < len2; ++j) {
            /* substitution */
            row2[j + 1] = row1[j] + s * (string1[i] != string2[j]);
            /* swap */
            if (i > 0 && j > 0 &&
                    string1[i - 1] == string2[j] &&
                    string1[i] == string2[j - 1] &&
                    row2[j + 1] > row0[j - 1] + w) {
                row2[j + 1] = row0[j - 1] + w;
            }
            /* deletion */
            if (row2[j + 1] > row1[j + 1] + d) {
                row2[j + 1] = row1[j + 1] + d;
            }
            /* insertion */
            if (row2[j + 1] > row2[j] + a) {
                row2[j + 1] = row2[j] + a;
            }
        }
        double *dummy = row0;
        row0 = row1;
        row1 = row2;
        row2 = dummy;
    }

    return 1.0 - row1[len2] / (len1 > len2 ? len1 : len2);
}

int readline(char *buffer)
{
    int length;
    fgets(buffer, BUFSIZE, stdin);
    if (ferror(stdin) || feof(stdin)) {
        return -1;
    }
    length = strlen(buffer);
    if (length && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    }
    if (length && buffer[length - 1] == '\r') {
        buffer[--length] = '\0';
    }
    return length;
}

int main(int argc, char *argv[])
{
    int opt, min_length = 1, length = 0; 
    double min_similarlity = 0.0, similarlity = 0.0;
    char input[BUFSIZE] = {'\0'}, comparison[BUFSIZE] = {'\0'}, similitude[BUFSIZE] = {'\0'};

    while ((opt = getopt(argc, argv, "i:l:s:h")) != -1) {
        switch (opt) {
            case 'i':
                strncpy(input, optarg, BUFSIZE - 1);
                break;
            case 'l':
                min_length = atoi(optarg);
                break;
            case 's':
                min_similarlity = atof(optarg);
                break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s -i input < comparisons (Splitted by \"\\n\")\n", argv[0]);
                fprintf(stderr, "  [-l min_length=1] [-s min_similarlity=0.0]\n");
                return opt != 'h';
        }
    }

    if (!*input) {
        fputs("No input specified\n", stderr);
        return 1;
    }
    if (*input == '_') {
        return 1;
    }
    similarlity = min_similarlity;

    while ((length = readline(comparison)) > -1) {
        if (length < min_length || length < 1 || *comparison == '_')
            continue;
        double s = calc_similarity(input, comparison);
        if (s < similarlity)
            continue;
        similarlity = s;
        strncpy(similitude, comparison, BUFSIZE - 1);
        if (s == 1.0)
            break;
    }

    if (*similitude == '\0') {
        return 1;
    }
    puts(similitude);
    return 0;
}