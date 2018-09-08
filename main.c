#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/*Borrar*/
#include <time.h>
/*Borrar*/

int num_data_points;
int num_clusters;
int num_dimensions;
double **degree_of_membs;
double epsilon;
double fuzziness;
double **data_points;
double **cluster_centres;

struct timeval t0;
struct timeval t1;
double elapsed;
double error;

int mejoraIteracion=0;
int mejoraFlag=0;
int iteracion;

int banderaImpar=1;

FILE *fp_resultados;

int uno=1;

double umbral=0.000000003;
double umbral2=0.00001;


double **low_high;

void print_membership_matrix(char *fname);
int gnuplot_membership_matrix();

double timedifference_msec(struct timeval t0, struct timeval t1);
void asignar();
double sumatoria_error(double **data_points,int num_data_points,double **cluster_centres,int num_clusters, int num_dimensions );

int init_centroides(char *fname);

double get_new_value(int i, int j);


//void crearObjetosEstables(double **data_points_anterior, double **degree_of_membs_anterior);
void crearObjetoEstable(int i,int j);
int
init(char *fname,char *fcentroides) {
    
    /*Borrar*/
    //    srand(time(NULL));
    /*Borrar*/
    
    int i, j;
    FILE *f;
    if ((f = fopen(fname, "r")) == NULL) {
        printf("Failed to open input file.");
        return -1;
    }
    
    gettimeofday(&t0, 0);
    // Inicializar matrices dinamicas
    
    data_points=(double **) malloc(num_data_points*sizeof(double *));
    
    for(i=0;i<num_data_points;i++){
        data_points[i]=(double *)malloc((num_dimensions+2)*sizeof(double));
    }
    
    degree_of_membs=(double **) malloc(num_data_points*sizeof(double *));
    
    for(i=0;i<num_data_points;i++){
        degree_of_membs[i]=(double *)malloc(num_clusters*sizeof(double));
    }
    
    
    for (i = 0; i < num_data_points; i++) {
        for (j = 0; j < num_dimensions; j++) {
            fscanf(f, "%lf", &data_points[i][j]);
            
            //Low High
            /*
             if (data_points[i][j] < low_high[j][0])
             low_high[j][0] = data_points[i][j];
             if (data_points[i][j] > low_high[j][1])
             low_high[j][1] = data_points[i][j];
             */
            //Low High
            
        }
        data_points[i][num_dimensions]=pow(10, 10);
        data_points[i][num_dimensions+1]=0;
    }
    fclose(f);
    
    //LowHigh
    /*   printf("LowHigh");
     for (int i = 0; i < num_dimensions; i++) {
     for (int j=0; j<2; j++) {
     printf("%lf ",low_high[i][j]);
     }
     printf("\n");
     }
     //LowHigh
     */
    // exit(0);
    
    /*
     int r;
     double rval=0.0;
     double s;
     
     
     
     double media=(1.0/num_clusters);
     double margin=media*0.15;
     
     double max=media+margin;
     double min=media-margin;
     
     */
    /*
     printf("Media %lf\n",media);
     printf("Margin %lf\n",margin);
     printf("Max %lf\n",max);
     printf("Min %lf\n",min);
     */
    /*
     for (i = 0; i < num_data_points; i++) {
     s = 0.0;
     r = 100;
     
     int ultima=rand() % num_clusters;
     for (j = 0; j < num_clusters;) {
     rval = rand() % (r+1);
     double value=(rval/100.0)/num_clusters;
     // printf("Value %lf\n",value);
     
     // printf("%lf\n",((double)rval/(double)num_clusters)/100.0);
     
     if (j!=ultima) {
     if (value<=max&&value>=min) {
     //r -= rval;
     //  printf("Entro\n");
     degree_of_membs[i][j] = value;
     // printf("Valor %lf\n",degree_of_membs[i][j]);
     s += degree_of_membs[i][j];
     j++;
     }
     }
     else{
     j++;
     }
     }
     degree_of_membs[i][ultima] = 1.0 - s;
     //    printf("Ultimo %lf\n",degree_of_membs[i][ultima]);
     }
     
     
     
     for (i = 0; i < num_data_points; i++) {
     s = 0.0;
     r = 100;
     for (j = 1; j < num_clusters; j++) {
     rval = rand() % (r + 1);
     r -= rval;
     degree_of_membs[i][j] = rval / 100.0;
     s += degree_of_membs[i][j];
     }
     degree_of_membs[i][0] = 1.0 - s;
     }
     */
    
    
    
    cluster_centres=(double **) malloc(num_clusters*sizeof(double *));
    for(i=0;i<num_clusters;i++){
        cluster_centres[i]=(double *)malloc(num_dimensions*sizeof(double));
    }
    init_centroides(fcentroides);
    
    
    //Inicializar matriz pertenencia
    double new_uij;
    
    for (j = 0; j < num_clusters; j++) {
        for (i = 0; i < num_data_points; i++) {
            new_uij = get_new_value(i, j);
            degree_of_membs[i][j] = new_uij;
        }
    }
    
    
    /*
     
     //Se abre el fichero
     for (i=0; i<num_data_points; i++) {
     for (j=0; j<num_clusters; j++) {
     fprintf(fp_matriz_pertenencia,"%.20g\t", degree_of_membs[i][j]);
     }
     fprintf(fp_matriz_pertenencia, "\n");
     }
     fclose(fp_matriz_pertenencia);
     exit(0);
     */
    return 0;
}

int
calculate_centre_vectors() {
    int i, j, k;
    double numerator, denominator;
    double **t;
    
    t=(double **) malloc(num_data_points*sizeof(double *));
    for(i=0;i<num_data_points;i++){
        t[i]=(double *)malloc(num_clusters*sizeof(double));
    }
    
    for (i = 0; i < num_data_points; i++) {
        for (j = 0; j < num_clusters; j++) {
            t[i][j] = pow(degree_of_membs[i][j], fuzziness);
        }
    }
    
    
    for (j = 0; j < num_clusters; j++) {
        for (k = 0; k < num_dimensions; k++) {
            numerator = 0.0;
            denominator = 0.0;
            for (i = 0; i < num_data_points; i++) {
                numerator += t[i][j] * data_points[i][k];
                denominator += t[i][j];
            }
            
            cluster_centres[j][k] = numerator / denominator;
        }
    }
    
    /*
     for (i=0; i<num_data_points; i++) {
     free(t[i]);
     }
     
     free(t);
     t=NULL;
     */
    return 0;
}

double
get_norm(int i, int j) {
    int k;
    double sum = 0.0;
    for (k = 0; k < num_dimensions; k++) {
        sum += pow(data_points[i][k] - cluster_centres[j][k], 2);
    }
    return sqrt(sum);
}

double get_new_value(int i, int j) {
    int k;
    double t, p, sum;
    sum = 0.0;
    p = 2 / (fuzziness - 1);
    for (k = 0; k < num_clusters; k++) {
        t = get_norm(i, j) / get_norm(i, k);
        t = pow(t, p);
        sum += t;
    }
    return 1.0 / sum;
}

double
update_degree_of_membership() {
    int i, j;
    double new_uij;
    double max_diff = 0.0, diff=0.0;
    
    printf("ITERACION %d\n",iteracion);
    
    
    /*
     int grupo=data_points[i][num_dimensions];
     // printf("Grupo: %d\n",grupo);
     double nuevo = get_new_value(i, grupo);
     // printf("Nuevo: %lf\n",nuevo);
     double anterior = degree_of_membs[i][grupo];
     // printf("Anterior: %lf\n",anterior);
     */
    
    for (i = 0; i < num_data_points&&data_points[i][num_dimensions+1]!=uno; i++) {
        for (j = 0; j < num_clusters; j++) {
            new_uij = get_new_value(i, j);
            diff = (double)new_uij - (double)degree_of_membs[i][j];
            //                   printf("Diff: %lf\n",diff);
            if (diff > max_diff)
                max_diff = diff;
            
            double anterior=degree_of_membs[i][j];
            //         printf("Anterior %lf\n",anterior);
            //       printf("Nuevo %lf\n",new_uij);
            double porcentaje=anterior*umbral;
            //       printf("Porcentaje %lf\n",porcentaje);
            double diferencia=new_uij-anterior;
            diferencia=pow(diferencia, 2);
            diferencia=sqrt(diferencia);
            //     printf("Diferencia %lf\n",diferencia);
            
            if(mejoraFlag==uno&&iteracion>mejoraIteracion) {
                
                if (diferencia>porcentaje) {
                    //                        printf("ES MAYOR DIFERENCIA\n");
                    //                      printf("J %d\n",j);
                    //                    printf("Cluster %d\n",(int)data_points[i][num_dimensions]);
                    degree_of_membs[i][j] = new_uij;
                }
                else{
                    //                 printf("J %d\n",j);
                    //               printf("Cluster %d\n",(int)data_points[i][num_dimensions]);
                    if (j==(int)data_points[i][num_dimensions]) {
                        //                  printf("J ES IGUAL\n");
                        degree_of_membs[i][j] = new_uij;
                        crearObjetoEstable(i, j);
                    }
                    else{
                        //               printf("J NOOO ES IGUAL\n");
                        degree_of_membs[i][j] = new_uij;
                    }
                }
                
                //Crear objetos
            }
            else{
                degree_of_membs[i][j] = new_uij;
                //    printf("Entra 3\n");
            }
        }
    }
    
    return max_diff;
}

int
fcm(char *fname, char *fcentroides) {
    double max_diff;
    init(fname, fcentroides);
    iteracion=0;
    
    //   double **data_points_anterior;
    //   double **degree_of_membs_anterior;
    
    
    do {
        /*
         if (mejoraFlag==1&&iteracion>=mejoraIteracion) {
         int i;
         data_points_anterior=(double **) malloc(num_data_points*sizeof(double *));
         for(i=0;i<num_data_points;i++){
         data_points_anterior[i]=(double *)malloc((num_dimensions+2)*sizeof(double));
         memcpy(data_points_anterior[i], data_points[i], (num_dimensions+2)*sizeof(double));
         //  data_points_anterior[i]=data_points[i];
         }
         
         degree_of_membs_anterior=(double **) malloc(num_data_points*sizeof(double *));
         for(i=0;i<num_data_points;i++){
         degree_of_membs_anterior[i]=(double *)malloc(num_clusters*sizeof(double));
         memcpy(degree_of_membs_anterior[i], degree_of_membs[i], num_clusters*sizeof(double));
         //degree_of_membs_anterior[i]=degree_of_membs[i];
         }
         }
         */
        
        
        /*
         printf("Centroides\n");
         for (int i = 0; i < num_clusters; i++) {
         for (int j=0; j<num_dimensions; j++) {
         printf("%lf ",cluster_centres[i][j]);
         }
         printf("\n");
         }
         printf("Centroides\n");
         */
        
        asignar();
        calculate_centre_vectors();
        max_diff = update_degree_of_membership();
        
        /*
         printf("MAX: %lf\n",max_diff);
         
         */
        
        /*
         if (mejoraFlag==1&&iteracion>=mejoraIteracion) {
         crearObjetosEstables(data_points_anterior, degree_of_membs_anterior);
         int n;
         for(n = 0; n < num_data_points; n++)
         free(data_points_anterior[n]);
         free(data_points_anterior);
         
         for(n = 0; n < num_data_points; n++)
         free(degree_of_membs_anterior[n]);
         free(degree_of_membs_anterior);
         
         data_points_anterior=NULL;
         degree_of_membs_anterior=NULL;
         }
         */
        iteracion++;
        
        /*
         print_membership_matrix("membership.matrix");
         gnuplot_membership_matrix(iteracion);
         system("gnuplot gnuplot.script");
         */
        //    printf("Max dif: %lf\n",max_diff);
        //  printf("Epsilon: %lf\n",epsilon);
    } while (fabs(max_diff) > epsilon);
    printf("Iteracion: %d\n",iteracion);
    
    gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);
    
    
    error=sumatoria_error(data_points,num_data_points,cluster_centres,num_clusters,num_dimensions);
    
    printf("Tiempo: %lf ms\n",elapsed);
    printf("Error: %lf \n",error);
    
    /*
     printf("Datos2\n");
     for (int i = 0; i < num_data_points; i++) {
     for (int j=0; j<num_dimensions+2; j++) {
     printf("[%d][%d]: %lf",i,j,data_points[i][j]);
     }
     printf("\n");
     }
     printf("Datos fin2\n");
     */
    
    return 0;
}
/*
 void crearObjetosEstables(double **data_points_anterior, double **degree_of_membs_anterior){
 int i;
 
 double e=1.0/(num_clusters*10);
 
 for (i = 0; i < num_data_points&&data_points[i][num_dimensions+1]!=uno; i++) {
 
 double anterior=degree_of_membs_anterior[i][(int)data_points[i][num_dimensions]];
 double nuevo=degree_of_membs[i][(int)data_points[i][num_dimensions]];
 
 //        printf("Nuevo %lf\n",nuevo);
 //        printf("Anterior %lf\n",anterior);
 
 double diff=nuevo-anterior;
 diff=pow(diff, 2);
 diff=sqrt(diff);
 
 double min=nuevo-epsilon;
 double max=nuevo-epsilon;
 
 
 if (diff<e) {
 //          printf("Diff %lf\n",diff);
 data_points[i][num_dimensions+1]=1;
 }
 }
 
 }
 */
void crearObjetoEstable(int i,int j){
    data_points[i][num_dimensions+1]=1;
}

void asignar(){
    int i, j, cluster;
    double highest;
    for (i = 0; i < num_data_points&&data_points[i][num_dimensions+1]!=uno; i++) {
        cluster = 0;
        highest = 0.0;
        for (j = 0; j < num_clusters; j++) {
            if (degree_of_membs[i][j] > highest) {
                highest = degree_of_membs[i][j];
                cluster = j;
            }
        }
        data_points[i][num_dimensions]=cluster;
    }
}

double timedifference_msec(struct timeval t0, struct timeval t1) //Recibe el tiempo inicial y final del computo
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

double sumatoria_error(double **data_points,int num_data_points,double **cluster_centres,int num_clusters, int num_dimensions ){
    int i,j,k;
    double error=0;
    double distancia;
    
    for (k=0; k<num_clusters; k++) {
        for (i=0; i<num_data_points; i++) {
            distancia=0;
            for (j=0; j<num_dimensions; j++) {
                if (data_points[i][num_dimensions]==k) {
                    distancia+=(degree_of_membs[i][k])*(pow(data_points[i][j]-cluster_centres[k][j], 2));
                }
            }
            error+=sqrt(distancia);
        }
    }
    return error;
}

int
init_centroides(char *fname) {
    
    FILE *f;
    if ((f = fopen(fname, "r")) == NULL) {
        printf("Failed to open input file.");
        return -1;
    }
    int i,j;
    for (i = 0; i < num_clusters; i++) {
        for (j = 0; j < num_dimensions; j++) {
            fscanf(f, "%lf", &cluster_centres[i][j]);
        }
    }
    
    /*
     printf("Centroides\n");
     for (int i = 0; i < num_clusters; i++) {
     for (int j=0; j<num_dimensions; j++) {
     printf("%lf ",cluster_centres[i][j]);
     }
     printf("\n");
     }
     printf("Centroides\n");
     */
    fclose(f);
    return 0;
}

int
main(int argc, char **argv) {
    printf
    ("------------------------------------------------------------------------\n");
    if (argc != 12) {
        printf("USAGE: fcm <input file>\n");
        exit(1);
    }
    
    num_data_points=atoi(argv[2]);
    num_dimensions=atoi(argv[3]);
    
    
    
    /* low_high=(double **) malloc(num_dimensions*sizeof(double *));
     int i;
     for(i=0;i<num_data_points;i++){
     low_high[i]=(double *)malloc(2*sizeof(double));
     }
     */
    
    num_clusters=atoi(argv[5]);
    fuzziness=atof(argv[6]);
    epsilon=atof(argv[7]);
    mejoraFlag=atoi(argv[8]);
    mejoraIteracion=atoi(argv[9]);
    fp_resultados = fopen(argv[10],"a");
    umbral = atof(argv[11]);
    
    fcm(argv[1], argv[4]);
    
    printf("Number of data points: %d\n", num_data_points);
    printf("Number of clusters: %d\n", num_clusters);
    printf("Number of data-point dimensions: %d\n", num_dimensions);
    printf("Accuracy margin: %lf\n", epsilon);
    
    fprintf(fp_resultados,"%.16g\t %lf\t %d\n", elapsed, error, iteracion); //Imprime en el archivo
    fclose(fp_resultados);  //Se cierra el fichero
    
    //   print_membership_matrix("membership.matrix");
    //   gnuplot_membership_matrix();
    
    /* Borrar
     printf("Datos\n");
     for (int i = 0; i < num_data_points; i++) {
     for (int j=0; j<num_dimensions+2; j++) {
     printf("[%d][%d]: %lf",i,j,data_points[i][j]);
     }
     printf("\n");
     }
     printf("Datos fin\n");
     // Borrar */
    
    // Libera memoria de centroides.
    
    /*
     int n;
     for(n = 0; n < num_clusters; n++)
     free(cluster_centres[n]);
     free(cluster_centres);
     
     for(n = 0; n < num_data_points; n++)
     free(data_points[n]);
     free(data_points);
     
     for(n = 0; n < num_data_points; n++)
     free(degree_of_membs[n]);
     free(degree_of_membs);
     
     degree_of_membs=NULL;
     cluster_centres=NULL;
     data_points=NULL;
     */
    return 0;
}



int
gnuplot_membership_matrix(int iter) {
    int i, j, cluster;
    char fname[100];
    double highest;
    FILE * f[num_clusters];
    if (num_dimensions != 2) {
        /*
         printf("Plotting the cluster only works when the\n");
         printf("number of dimensions is two. This will create\n");
         printf("a two-dimensional plot of the cluster points.\n");
         */
        
        return 0;
    }
    for (j = 0; j < num_clusters; j++) {
        sprintf(fname, "cluster.%d", j);
        if ((f[j] = fopen(fname, "w")) == NULL) {
            printf("Could not create %s\n", fname);
            for (i = 0; i < j; i++) {
                fclose(f[i]);
                sprintf(fname, "cluster.%d", i);
                remove(fname);
            }
            return -1;
        }
        fprintf(f[j], "#Data points for cluster: %d\n", j);
    }
    for (i = 0; i < num_data_points; i++) {
        cluster = 0;
        highest = 0.0;
        for (j = 0; j < num_clusters; j++) {
            if (degree_of_membs[i][j] > highest) {
                highest = degree_of_membs[i][j];
                cluster = j;
            }
        }
        fprintf(f[cluster], "%lf %lf\n", data_points[i][0], data_points[i][1]);
    }
    for (j = 0; j < num_clusters; j++) {
        fclose(f[j]);
    }
    if ((f[0] = fopen("gnuplot.script", "w")) == NULL) {
        printf("Could not create gnuplot.script.\n");
        for (i = 0; i < j; i++) {
            fclose(f[i]);
            sprintf(fname, "cluster.%d", i);
            remove(fname);
        }
        return -1;
    }
    fprintf(f[0], "set terminal png medium\n");
    fprintf(f[0], "set output \"cluster_plot%d.png\"\n",iter);
    fprintf(f[0], "set title \"FCM clustering\"\n");
    fprintf(f[0], "set xlabel \"x-coordinate\"\n");
    fprintf(f[0], "set ylabel \"y-coordinate\"\n");
    fprintf(f[0], "set xrange [%lf : %lf]\n", low_high[0][0], low_high[0][1]);
    fprintf(f[0], "set yrange [%lf : %lf]\n", low_high[1][0], low_high[1][1]);
    fprintf(f[0],
            "plot 'cluster.0' using 1:2 with points pt 7 ps 1 lc 1 notitle");
    for (j = 1; j < num_clusters; j++) {
        sprintf(fname, "cluster.%d", j);
        fprintf(f[0],
                ",\\\n'%s' using 1:2 with points  pt 7 ps 1 lc %d notitle",
                fname, j + 1);
    }
    fprintf(f[0], "\n");
    fclose(f[0]);
    return 0;
}

void
print_membership_matrix(char *fname) {
    int i, j;
    FILE *f;
    if (fname == NULL)
        f = stdout;
    else if ((f = fopen(fname, "w")) == NULL) {
        printf("Cannot create output file.\n");
        exit(1);
    }
    fprintf(f, "Membership matrix:\n");
    for (i = 0; i < num_data_points; i++) {
        fprintf(f, "Data[%d]: ", i);
        for (j = 0; j < num_clusters; j++) {
            fprintf(f, "%lf ", degree_of_membs[i][j]);
        }
        fprintf(f, "\n");
    }
    if (fname == NULL)
        fclose(f);
}

