//
//  main.c
//  Numbrix

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv)
{
  FILE * fp = fopen("formular", "w") ;
  FILE * fpc = fopen(argv[1],"r");
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
    //printf("%d %d\n", row, col); // Debugging - 1
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
    
    FILE * fpd = fopen(argv[1],"r");
 /*   char buffer2 [90000];
    for (int i =0; i<90000;i ++){
        buffer2[i] = 0;
    }*/
    int k=1,l=1;
    int already = 0;
    
    char c = 0;
    while(1){
        c = fgetc(fpd);
        if(feof(fpd)) break;
        if(c==' ') {l++; already = 0; continue;}
        if(c=='\n'){k++; l=1; continue;}
        if(c!='\0'){
                if(c=='?'){
                    //printf("-1 ");
                    a[k][l]=0;
                    //l++;
                }else{
                    //printf("%d ",atoi(&c));
                    a[k][l]*=10;
                    a[k][l]+=atoi(&c);
                    already = 1;
                }
            
        }
    }

    /*
    while(1){
        already = 0;
            fgets(buffer2, sizeof(buffer2), fpd);
            if(feof(fpd)) break;
            
            for (int i =0;i < sizeof(buffer2); i++){
                if ( l == col+1) break;
                
                if (buffer[i] == '\n'){
                    break;
                }
                
                if((buffer2[i] == ' ')){
                    if (already == 1){
                        l++;
                        already = 0;
                        
                    }
                    continue;
                    
                }
                if(buffer2[i] == '?'){
                    // printf("? "); // Debugging - 3
                    a[k][l] = 0;
                    l++;
                }
                if(buffer2[i]>=48 && buffer2[i]<= 57){
                    if (already == 1) {a[k][l] *=10;}
                    a[k][l]+= (buffer2[i]-48);
                    if(already == 1){continue;}
                    a[k][l] = atoi(&(buffer2[i]));
                    
                    already = 1;
                    
                }
                
            }
     */
            //printf("\n");
        /*    k++;
            l = 1;
        }*/
      fclose(fpd);
    /*
    printf("\n \n");
    for(int i=0;i<row+2;i++){
    for(int j=0;j<col+2;j++)
        printf("%d ",a[i][j]);
    printf("\n");
    }
    printf("\n \n");
    */
    for (int i = 0 ; i < row+2 ; i++)
        for (int j = 0 ; j < col+2 ; j++)
                  fprintf(fp, "(declare-const a%d_%d Int)\n", i, j) ;
    
    for (int i =0; i<row+2; i++){
        for (int j =0; j<col+2; j++){
            if (i == 0){
                fprintf(fp, "(assert (= a%d_%d 0))\n", i, j) ;
                continue;
            }
            if (j==0) {
                fprintf(fp, "(assert (= a%d_%d 0))\n", i, j) ;
                continue;
            }
            if (i == row+1){
                fprintf(fp, "(assert (= a%d_%d 0))\n", i, j) ;
                continue;
            }
            if (j==col+1) {
                fprintf(fp, "(assert (= a%d_%d 0))\n", i, j) ;
                continue;
            }
            
            if (a[i][j]!=0){
                fprintf(fp, "(assert (= a%d_%d %d))\n", i, j, a[i][j]) ;
            }
            else{
            fprintf(fp,"(assert(and (<= a%d_%d %d) (<= 1 a%d_%d)))\n", i, j,(row*col), i, j);
            }
        }
    }
    
    for (int i =1; i<row+1; i++){
        for (int j=1; j<col+1; j++){
            fprintf(fp,"(assert (or (= a%d_%d (- a%d_%d 1)) (= a%d_%d (- a%d_%d 1)) (= a%d_%d (- a%d_%d 1)) (= a%d_%d (- a%d_%d 1)) (= a%d_%d 1)))\n", i,j-1, i,j, i, j+1, i,j, i+1, j,i,j, i-1, j, i,j,i,j);
            fprintf(fp,"(assert (or (= a%d_%d (+ a%d_%d 1)) (= a%d_%d (+ a%d_%d 1)) (= a%d_%d (+ a%d_%d 1)) (= a%d_%d (+ a%d_%d 1)) (= a%d_%d %d)))\n", i,j-1, i,j, i, j+1, i,j, i+1, j,i,j, i-1, j, i,j, i,j, row*col);
        }
    }
    
    for (int i =1; i<=row; i++){
        for (int j=1; j<=col; j++){
            for (int k =1; k<=row; k++){
                //fprintf(fp, "(assert (not (= a%d_%d a%d_%d)))\n", i, j,k,j) ;
                for (l = 1; l<=col; l++){
                    if((k!=i) || (l!= j)) fprintf(fp, "(assert (not (= a%d_%d a%d_%d)))\n", i, j,k,l) ;
            }
            /*for (int k =i+1; k<=row; k++){
                    //fprintf(fp, "(assert (not (= a%d_%d a%d_%d)))\n", i, j,k,j) ;
                for (l = j+1; l<=col; l++){
                    fprintf(fp, "(assert (not (= a%d_%d a%d_%d)))\n", i, j,k,l) ;
                }*/
            }
            /*for (l = j+1; l<=col; l++){
               fprintf(fp, "(assert (not (= a%d_%d a%d_%d)))\n", i, j,i,l) ;
            }*/
        }
    }
    fprintf(fp,"(check-sat)\n(get-model)\n");
     fclose(fp) ;

 FILE * fin = popen("z3 formular", "r") ; //FIXME
    int board[row+2][col+2] ;
    int i, j, z;

    char b[128] ;
    char s[128] ;
    char position, close ;
    int where=0, value = 0;
    
    fscanf(fin, "%s %s", b, s) ;
    if(strcmp(b,"unsat")==0){
        printf("No solution!\n");
        exit(-1);
    }
    
    for (z = 0 ; z < (row+2)*(col+2) ; z++) {
        fscanf(fin,"%s %c%d%c%d %s %s", b, &position, &i, &position, &j, b, b);
        fscanf(fin, " %d%c", &value, &close );
        //printf("%c%d %d%c\n", position, where, value, close);

        board[i][j] = value;
        //printf("%d %d %d %d\n", where, i, j, value);
    }
    
    for (i = 1 ; i <= row ; i++) {
            for (j = 1 ; j <= col ; j++) {
                    printf("%d ", board[i][j]) ;
            }
            printf("\n") ;
    }
    pclose(fin) ;
}


