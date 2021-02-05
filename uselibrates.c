#include "./http.h"
#include "./rates.h"
#define filename "output.csv" 

int main(int argc,char *argv[]){
    if(argc>4){
    size_t ncurr = argc-4;
    Rates r = {.base="xpto",.currencies=NULL,.size=0};
    Date dinicio = {.date=argv[1]};
    Date dfim = {.date=argv[2]};
    char * currencies[ncurr];
    for(int i=argc-ncurr;i<argc;++i)
        currencies[i-(argc-ncurr)]=argv[i];
        
    char *base = argv[3];
    FILE * file = fopen(filename,"w");
    if (file == NULL) {
        fprintf(stderr, "*** error openning output file\n");
        return -1;
    }
    if(((get_rates(dinicio,dfim,ncurr,base,currencies,&r)))>=0){
      fprintf(file,"%s,",r.base);
        for(size_t i =0; i<ncurr;++i)
             fprintf(file,"%s%c",currencies[i],(i==(ncurr-1)?' ':','));
             fprintf(file,"\n");
        for(size_t i =0; i<r.size;++i){
                Currencies aux = r.currencies[i];
                fprintf(file,"%s, ",aux.date->date);
                for(size_t j=0;j<aux.ncurr;++j)
                    fprintf(file,"%f %c ",aux.value[j], (j==(aux.ncurr-1)?' ':','));
                 fprintf(file,"\n");   
        }
    }
    fclose(file);
    free_rates(&r);
    }
    return 0;
}