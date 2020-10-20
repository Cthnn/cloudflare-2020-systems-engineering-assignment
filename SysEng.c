#include "SysEng.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <float.h>

int compare(const void*a, const void *b){
      double *a_dbl = (double *) a;
      double *b_dbl = (double *) b;
      if (*a_dbl > *b_dbl){
            return 1;
      }
      if (*a_dbl == *b_dbl){
            return 0;
      }
      if (*a_dbl < *b_dbl){
            return -1;
      }
}

int main(int argc, char **argv){
      char *url = NULL;
      int port = 80;
      int profile= 0;
      if(parseCL(argc,argv,&url,&port,&profile)){
            char *token = strtok(url,"/");
            char *host = NULL;
            char *path = NULL;
            while(token != NULL){
                  if(host == NULL){
                        if (strcmp(token,"https:") != 0 && strcmp(token,"http:")!= 0){
                        host = token;
                        token = strtok(NULL," ");
                        }else{
                        token = strtok(NULL,"/");
                        }
                  }else{
                        path = malloc(strlen(token)+1);
                        *path = '/';
                        strcat(path,token);
                        break;
                  }
            }
            if (host == NULL && path == NULL){
                  return -1;
            }else if (path == NULL){
                  path = malloc(1);
                  *path = '/';
            }
            if (profile > 0){
                  double maxTime = 0;
                  double minTime = DBL_MAX;
                  double total = 0;
                  double times[profile];
                  int errors = 0;
                  int success = 0;
                  int minSize = __INT_MAX__;
                  int maxSize = 0;
                  for(int i = 0; i < profile;i++){
                        int resp = 0;
                        struct timeval begin,now;
                        double time;
                        gettimeofday(&begin , NULL);
                        resp = makeReq(host,path,port,1);
                        gettimeofday(&now , NULL);
                        time = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
                        if (resp >= 0){
                              success += 1;
                              total += time;
                              times[i] = time;
                              if (time > maxTime){
                                    maxTime = time;
                              }
                              if (time < minTime){
                                    minTime = time;
                              }
                              if (resp < minSize){
                                    minSize = resp;
                              }
                              if(resp > maxSize){
                                    maxSize = resp;
                              }
                        }else{
                              errors= -1;
                              times[i] = 0.0;
                        }
                  }
                  double average = total/success;
                  qsort(times,profile,sizeof(double),compare);
                  double median = (profile%2 == 0) ? (times[profile/2]+times[(profile/2)+1])/2 :times[profile/2];
                  if(success == 0){
                        minTime = 0;
                        maxTime = 0;
                        minSize = 0;
                        maxSize = 0;
                        average = 0.0;
                        median = 0;
                        
                  }
                  printf("Profile of %s \n",host);
                  printf("\tNumber of Requests:\t\t\t%d\n",profile);
                  printf("\tFastest Time(in seconds):\t\t%.2f\n",minTime);
                  printf("\tSlowest Time(in seconds):\t\t%.2f\n",maxTime);
                  printf("\tAverage Time(in seconds):\t\t%.2f\n",average);
                  printf("\tMedian Time(in seconds):\t\t%.2f\n",median);
                  printf("\tPercentage Successful:\t\t\t%.2f\n",100.0*(success/profile));
                  if (errors == -1){
                        printf("\tError Codes:\t\t\t\t%d\n",errors);
                  }else{
                        printf("\tError Codes:\n");
                  }
                  printf("\tSmallest Response(in bytes):\t\t%d\n",minSize);
                  printf("\tLargest Response(in bytes):\t\t%d\n",maxSize);
                  free(path);
                  return 0;
            }else{
                  if(makeReq(host,path,port,0) >= 0){
                        free(path);
                        return 0;
                  }else{
                        free(path);
                        return -1;
                  }
            }
      } 
}