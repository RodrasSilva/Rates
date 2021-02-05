#include "./rates.h"
#include "./http.h"
#define starturl "https://api.exchangeratesapi.io/history?base="

char *strdup(const char *);

static int compar(const void * first, const void * second){
    Currencies* aux1 = (Currencies *)first;
    Currencies* aux2 = (Currencies *)second;
    return strcmp(aux1->date->date,aux2->date->date); 
}

int get_rates(Date start_at, Date end_at,unsigned n_currencies,char *base, char *currencies[], Rates *result){
  
    char *request;
    json_t *jroot,*jrates,*jcurrentcurrency;
    json_error_t error;
    int resultValue=-1;
    char url[256];
    printf("Building Url...\n\n");
    //Build the url for the http request
    size_t url_size = snprintf(url,sizeof(url),"%s%s&start_at=%s&end_at=%s&symbols=",
        starturl,base,start_at.date,end_at.date);
    for(size_t i = 0;i<n_currencies;++i){
        url_size = url_size+snprintf(url+url_size,sizeof(url),"%s%c",currencies[i],(i==(n_currencies-1))?' ':',');     
    }
        printf("Url is : %s \n\n",url);
    
    //http request
    size_t request_size=0;
    
    request = http_get_data(url,&request_size);
    
    if(request==NULL){
        fprintf(stderr,"Request returned NULL, please enter correct information");
        return resultValue;
    }
    
    jroot = json_loads(request,0,&error);
    free(request);

    if (jroot == NULL) {
    	/* an error  occured while parsing JSON text */
        fprintf(stderr, "***error: on line %d: %s\n", error.line, error.text);
        return resultValue;
    }
	
	/**
	 * Process the JSON response
	 */
	
    /* the root must always be an object */
    if (!json_is_object(jroot)) {
        fprintf(stderr, "***error: root is not an object\n");
        goto cleanup;
    }
	/* get the "currently" field from root object; this field must be an object */
	jrates = json_object_get(jroot, "rates");

	if (!json_is_object(jrates)) {
		fprintf(stderr, "***error: rates is not an object\n");
		goto cleanup;
	}
	result->base = base;
    result->size = 0;
    size_t numberOfCurrencies=0;
    const char *key;
    json_t *value;
    Currencies * curr =NULL;
    json_object_foreach(jrates,key,value){
            curr = (Currencies *)realloc(curr,(numberOfCurrencies+1)*sizeof(Currencies));
            curr[numberOfCurrencies].ncurr = n_currencies;
            Date * currDate = (Date *)malloc(sizeof(Date));
            currDate->date = strdup((char *)key);
            curr[numberOfCurrencies].date = currDate;
            double * auxValues = malloc (n_currencies*sizeof(double));
            for(size_t i = 0 ; i< n_currencies ; ++i){
                jcurrentcurrency = json_object_get(value,currencies[i]);
                auxValues[i] = (double)json_number_value(jcurrentcurrency);
            }
            curr[numberOfCurrencies++].value = auxValues;
    }
    result->currencies = curr;
    result->size = numberOfCurrencies;   
	/* return success */
    resultValue = 0;
    cleanup:
	/* free all memory allocated by jansson library to build the object tree */
    json_decref(jroot);	
    qsort(result->currencies,result->size,sizeof(Currencies),compar);
	return resultValue;
}


void free_rates(Rates *rates){
    for(size_t i =0;i<(rates->size);++i){
        Currencies auxcurr = rates->currencies[i];
        free(auxcurr.date->date);
        free(auxcurr.value); 
    }
    free(rates->currencies);
}
