#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <jansson.h>


typedef struct _Date{
    char *date;
}Date;

typedef struct _Currencies{
    Date *date;
    size_t ncurr;
    double * value;
}Currencies;

typedef struct _Rates{
    char *base;
    size_t size;
    Currencies * currencies;
}Rates;

int get_rates(Date start_at, Date end_at,unsigned n_currencies,char *base,
                                         char *currencies[], Rates *result);
void free_rates(Rates *rates);