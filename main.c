//
//  main.c
//  Numbrix
//
//  Created by YoungChae Kong on 30/09/2019.
//  Copyright © 2019 Team 2. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
  FILE * fp = fopen("formular", "w") ;
  FILE * fpc = fopen("test","r");
  int **a=NULL;
    char buffer[100];

     
    int row=0,col=0;
    
    while(1){
      fgets(buffer, sizeof(buffer), fpc);
      if(feof(fpc))break;
        row++;
        for (int i =0; (row == 0) &&i < sizeof(buffer); i++){
            if(!((buffer[i] == ' ')||(buffer[i]== '\n'))) col++;
        }
          col ++;
     }
    // printf("%d %d\n", row, col); // Debugging - 1
    fclose(fpc);

  //row 와 column은 알게됨.
    a=(int**)malloc((row+2) * sizeof (int*));
    for (int j = 0; j < row+2; j++)
    a[j]=(int*)malloc((col+2) * sizeof(int));
    //할당
    
    for (int i =0; i<row+2; i++){
        for (int j=0; j<col+2; j++){
            a[i][j] = 0;
            //printf("%d", a[i][j]);
        }
        //printf("\n");
    } // Debugging - 2
    
    FILE * fpd = fopen("test","r");
    char buffer2 [100];
    int k=1,l=1;
    int already = 0;
        while(1){
            
            fgets(buffer2, sizeof(buffer2), fpd);
            if(feof(fpd)) break;
            
            for (int i =0;i < sizeof(buffer2); i++){
                if(buffer2[i] == ' '){
                    already = 0;
                    continue;
                    
                }
                if(buffer2[i] == '?'){
                    // printf("? "); // Debugging - 3
                    a[k][l] = 0;
                    l++;
                }
                if(buffer2[i]>=48 && buffer2[i]<= 57){
                    if(already == 1){already =0; continue;}
                    a[k][l] = atoi(&(buffer2[i]));
                    already = 1;
                    l++;
                }
                
            }
            //printf("\n");
            k++;
            l = 1;
        }
      fclose(fpd);
    /*
    printf("\n \n");
    for(int i=0;i<row+2;i++){
    for(int j=0;j<col+2;j++)
        printf("%d ",a[i][j]);
    printf("\n");
    }
    */
    for (int i = 0 ; i < row+2 ; i++)
        for (int j = 0 ; j < col+2 ; j++)
                  fprintf(fp, "(declare-const a%d%d Int)\n", i, j) ;
    
    for (int i =0; i<row+2; i++){
        for (int j =0; j<col+2; j++){
            if (i == 0){
                fprintf(fp, "(assert (= a%d%d 0))\n", i, j) ;
                continue;
            }
            if (j==0) {
                fprintf(fp, "(assert (= a%d%d 0))\n", i, j) ;
                continue;
            }
            if (i == row+1){
                fprintf(fp, "(assert (= a%d%d 0))\n", i, j) ;
                continue;
            }
            if (j==col+1) {
                fprintf(fp, "(assert (= a%d%d 0))\n", i, j) ;
                continue;
            }
            
            if (a[i][j]!=0){
                fprintf(fp, "(assert (= a%d%d %d))\n", i, j, a[i][j]) ;
            }
            else{
            fprintf(fp,"(assert(and (<= a%d%d %d) (<= 1 a%d%d)))\n", i, j,(row*col), i, j);
            }
        }
    }
    
    for (int i =1; i<row+1; i++){
        for (int j=1; j<col+1; j++){
            fprintf(fp,"(assert (or (= a%d%d (- a%d%d 1)) (= a%d%d (- a%d%d 1)) (= a%d%d (- a%d%d 1)) (= a%d%d (- a%d%d 1)) (= a%d%d 1)))\n", i,j-1, i,j, i, j+1, i,j, i+1, j,i,j, i-1, j, i,j,i,j);
            fprintf(fp,"(assert (or (= a%d%d (+ a%d%d 1)) (= a%d%d (+ a%d%d 1)) (= a%d%d (+ a%d%d 1)) (= a%d%d (+ a%d%d 1)) (= a%d%d %d)))\n", i,j-1, i,j, i, j+1, i,j, i+1, j,i,j, i-1, j, i,j, i,j, row*col);
        }
    }
    
    for (int i =1; i<=row; i++){
        for (int j=1; j<=col; j++){
            for (int k =i+1; k<=row; k++){
                    fprintf(fp, "(assert (not (= a%d%d a%d%d)))\n", i, j,k,j) ;
                for (l = j+1; l<=col; l++){
                    fprintf(fp, "(assert (not (= a%d%d a%d%d)))\n", i, j,k,l) ;
                }
            }
            for (l = j+1; l<=col; l++){
               fprintf(fp, "(assert (not (= a%d%d a%d%d)))\n", i, j,i,l) ;
            }
        }
    }
    fprintf(fp,"(check-sat)\n(get-model)\n");
     fclose(fp) ;

 FILE * fin = popen("z3 formula", "r") ; //FIXME
    int board[row+2][col+2] ;
    int i, j, z;

    char b[128] ;
    char s[128] ;
    char position ;
    int where=0, value = 0;
    
    fscanf(fin, "%s %s", b, s) ;
    if(strcmp(b,"unsat")==0){
        printf("No solution!\n");
        exit(-1);
    }
    
    for (z = 0 ; z < (row+2)*(col+2) ; z++) {
        fscanf(fin,"%s %c%d %s %d%c", b, &position, &where, b, &value, &position);
        
        i = where/10;
        j =  where%10;

        board[i][j] = value;
        printf("%d %d %d %d", where, i, j, value);
    }
    
    for (i = 1 ; i <= row ; i++) {
            for (j = 1 ; j <= col ; j++) {
                    printf("%d ", board[i][j]) ;
            }
            printf("\n") ;
    }
    pclose(fin) ;
}


