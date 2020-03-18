#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_D 100000
#define MAX_M 20000
#define MAX_S 100

typedef struct
{
    int posizionato; //lo metto a 1 quando lo siedo
    char company[6];
    int bonus_potential;
    int tot_skills;
    char skills[100][MAX_S];
    int x, y;
} developer;

typedef struct
{
    int posizionato; //lo metto a 1 quando lo siedo
    char company[6];
    int bonus_potential;
    int x,y;
} manager;

typedef struct{
  developer d;
  manager m;
  int chi; //0 dev, 1 man
}worker;

int width, height;
char office[1000][1000];
int office_set[1000][1000]; //va messo tutto a 0

int dev_number, man_number;

developer devs[MAX_D];
manager mans[MAX_M];

//trova il miglior lavoratore da mettere in una posizone dati i suoi vicini

int getPointsBySkills(developer d1, developer d2){
  int nDiverse = 0, nUguali = 0;

  for(int i=0; i<d1.tot_skills; i++){
    for(int j=0; j<d2.tot_skills; j++){
      if(strcmp(d1.skills[i], d2.skills[j])==0){
        nUguali++;
      }
    }
  }

  nDiverse = d1.tot_skills + d2.tot_skills - (2*nUguali);
  return nDiverse*nUguali;
}

//reswtituisce i punti tra due workers, esaustiva
int getPoints(worker w1, worker w2){

  if(w1.chi == 0){
    if(w2.chi == 0){
      if(strcmp(w1.d.company, w2.d.company) == 0){
        return w1.d.bonus_potential * w2.d.bonus_potential + getPointsBySkills(w1.d, w2.d);
      }
      return getPointsBySkills(w1.d, w2.d);
    }else{
      if(strcmp(w1.d.company, w2.m.company)==0){
        return w1.d.bonus_potential * w2.m.bonus_potential;
      }
    }
  }else{
    if(w2.chi == 0){
      if(strcmp(w1.m.company, w2.d.company) == 0){
        return w1.m.bonus_potential * w2.d.bonus_potential;
      }
    }else{
      if(strcmp(w1.m.company, w2.m.company)==0){
        return w1.m.bonus_potential * w2.m.bonus_potential;
      }
    }
  }
  return 0;
}

int best(worker workers[], int n, int chi){

  worker solution;
  worker temp;
  int bestPoints = 0;
  int points = 0;
  int bestPosition;
  int i, j;
  worker w;

    if(chi == 0){
      for(i=0; i<dev_number; i++){
        if(!devs[i].posizionato){

          points = 0;
          for(j=0; j<n; j++){
            w.d = devs[i];
            w.chi = 0;
            points += getPoints(workers[j], w);

          }

          if(points >= bestPoints){
            bestPoints = points;
            bestPosition = i;
          }
        }
      }

      devs[bestPosition].posizionato = 1;
      solution.d = devs[bestPosition];
      solution.chi = 0;
    }else{

      for(i=0; i<man_number; i++){
        if(!mans[i].posizionato){

          points = 0;
          for(j=0; j<n; j++){
            w.m = mans[i];
            w.chi = 1;
            points += getPoints(workers[j], w);
          }

          if(points >= bestPoints){
            bestPoints = points;
            bestPosition = i;
          }
        }
      }

      mans[bestPosition].posizionato = 1;
      solution.m = mans[bestPosition];
      solution.chi = 1;
    }



  return bestPosition;
}

void populateNeighbours(int i, int j, worker *w){

  //worker w[4];
  worker temp;
  int numero = 0;

  developer vuotoD;
  manager vuotoM;

  vuotoD.company[0] = 'V';
  vuotoD.bonus_potential = 0;
  vuotoD.tot_skills = 0;

  vuotoM.company[0] = 'V';
  vuotoM.bonus_potential = 0;

  if(i!=0){
    if(office_set[i-1][j] != -1){
      if(office[i-1][j] != '#' && office[i-1][j] == '_'){
        temp.d = devs[office_set[i-1][j]];
        temp.chi = 0;
        w[0] = temp;
      }else if(office[i-1][j] != '#' && office[i-1][j] == 'M'){
        temp.m = mans[office_set[i-1][j]];
        temp.chi = 1;
        w[0] = temp;
      }
    }else{
      temp.d = vuotoD;
      temp.m = vuotoM;
      temp.chi = 1;
      w[0] = temp;
    }


  }else{
    w[0] = temp;
  }


  if(j!=0){
    if(office_set[i][j-1] != -1){
      if(office[i][j-1] != '#' && office[i][j-1] == '_'){
        temp.d = devs[office_set[i][j-1]];
        temp.chi = 0;
        w[3] = temp;
      }else if(office[i][j-1] != '#' && office[i][j-1] == 'M'){
        temp.m = mans[office_set[i][j-1]];
        temp.chi = 1;
        w[3] = temp;
      }
    }else{
      temp.d = vuotoD;
      temp.m = vuotoM;
      temp.chi = 1;
      w[3] = temp;
    }

  }else{
    w[3] = temp;
  }

  if(i!=height-1){
    if(office_set[i+1][j] != -1){
      if(office[i+1][j] != '#' && office[i+1][j] == '_'){
        temp.d = devs[office_set[i+1][j]];
        temp.chi = 0;
        w[2] = temp;
      }else if(office[i+1][j] != '#' && office[i+1][j] == 'M'){
        temp.m = mans[office_set[i+1][j]];
        temp.chi = 1;
        w[2] = temp;
      }
    }else{
      temp.d = vuotoD;
      temp.m = vuotoM;
      temp.chi = 1;
      w[2] = temp;
    }

  }else{
    w[2] = temp;
  }

  if(j!=width -1){
    if(office_set[i][j+1] != -1){
      if(office[i][j+1] != '#' && office[i][j+1] == '_'){
        temp.d = devs[office_set[i][j+1]];
        temp.chi = 0;
        w[1] = temp;
      }else if(office[i][j+1] != '#' && office[i][j+1] == 'M'){
        temp.m = mans[office_set[i][j+1]];
        temp.chi = 1;
        w[1] = temp;
      }
    }else{
      temp.d = vuotoD;
      temp.m = vuotoM;
      temp.chi = 1;
      w[1] = temp;
    }

    }else{
      w[1] = temp;
    }
}

void solver(){
  int i, j;
  worker workers[4];
  worker w;
  int bestPosition;

  for(i = 0; i<height; i++){
    for(j = 0; j<width; j++){
      //printf("%d %d\n", i, j);
      if(office[i][j] != '#'){

        if(office[i][j] == '_'){
          populateNeighbours(i,j, workers);

          //printf("%ld\n", sizeof(workers)/sizeof(workers[0]));
          bestPosition = best(workers, 4, 0);
          office_set[i][j] = bestPosition;
          devs[bestPosition].x = j;
          devs[bestPosition].y = i;
          //printf("%d\n", bestPosition);
        }else{
          populateNeighbours(i,j, workers);
          //printf("%ld", sizeof(workers)/sizeof(workers[0]));
          bestPosition = best(workers, 4, 1);
          office_set[i][j] = bestPosition;
          mans[bestPosition].x = j;
          mans[bestPosition].y = i;
          //printf("%d\n", bestPosition);
        }
      }
  }
}
}

void print_office()
{
    printf("\n");
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            printf("%c", office[row][col]);
        }
        printf("\n");
    }
}

//ridà punti sulla diff delle skills

//calcola il punteggio sul campo e lo mette in points
int evaluate(){

  int i, j;
  int points = 0;

  for(j=0; j<width-1; j++){
    i = height-1;
    if(office_set[i][j] != '#'){

      if(office[i][j] == '_'){

        if(office[i][j+1] == '_'){
          developer d1, d2;
          d1 = devs[office_set[i][j]];
          d2 = devs[office_set[i][j+1]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
          points +=getPointsBySkills(d1, d2);
        }else if(office[i][j+1] == 'M'){
          developer d1;
          manager d2;
          d1 = devs[office_set[i][j]];
          d2 = mans[office_set[i][j+1]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
        }
      }else{
        if(office[i][j+1] == '_'){
          developer d1;
          manager d2;
          d2 = mans[office_set[i][j]];
          d1 = devs[office_set[i][j+1]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
        }else if(office[i][j+1] == 'M'){
          manager d1;
          manager d2;
          d1 = mans[office_set[i][j]];
          d2 = mans[office_set[i][j+1]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
        }
      }
    }
  }

  for(i=0; i<height-1;i++){
    j = width - 1;

    if(office_set[i][j] != '#'){

      if(office[i][j] == '_'){

        if(office[i+1][j] == '_'){
          developer d1, d2;
          d1 = devs[office_set[i][j]];
          d2 = devs[office_set[i+1][j]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
          points +=getPointsBySkills(d1, d2);
        }else if(office[i+1][j] == 'M'){
          developer d1;
          manager d2;
          d1 = devs[office_set[i][j]];
          d2 = mans[office_set[i+1][j]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
        }

      }else{
        if(office[i+1][j] == '_'){
          developer d1;
          manager d2;
          d2 = mans[office_set[i][j]];
          d1 = devs[office_set[i+1][j]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
        }else if(office[i+1][j] == 'M'){
          manager d1;
          manager d2;
          d1 = mans[office_set[i][j]];
          d2 = mans[office_set[i+1][j]];
          if(strcmp(d1.company , d2.company)==0){
            int bonus = d1.bonus_potential * d2.bonus_potential;
            points += bonus;
          }
        }

      }
    }


  }

  for(i=0; i<height-1; i++){
    for(j=0; j<width-1; j++){

      if(office_set[i][j] != '#'){

        if(office[i][j] == '_'){

          if(office[i+1][j] == '_'){
            developer d1, d2;
            d1 = devs[office_set[i][j]];
            d2 = devs[office_set[i+1][j]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
            points +=getPointsBySkills(d1, d2);
          }else if(office[i+1][j] == 'M'){
            developer d1;
            manager d2;
            d1 = devs[office_set[i][j]];
            d2 = mans[office_set[i+1][j]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
          }
          if(office[i][j+1] == '_'){
            developer d1, d2;
            d1 = devs[office_set[i][j]];
            d2 = devs[office_set[i][j+1]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
            points +=getPointsBySkills(d1, d2);
          }else if(office[i][j+1] == 'M'){
            developer d1;
            manager d2;
            d1 = devs[office_set[i][j]];
            d2 = mans[office_set[i][j+1]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
          }
        }else{
          if(office[i+1][j] == '_'){
            developer d1;
            manager d2;
            d2 = mans[office_set[i][j]];
            d1 = devs[office_set[i+1][j]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
          }else if(office[i+1][j] == 'M'){
            manager d1;
            manager d2;
            d1 = mans[office_set[i][j]];
            d2 = mans[office_set[i+1][j]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
          }

          if(office[i][j+1] == '_'){
            developer d1;
            manager d2;
            d2 = mans[office_set[i][j]];
            d1 = devs[office_set[i][j+1]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
          }else if(office[i][j+1] == 'M'){
            manager d1;
            manager d2;
            d1 = mans[office_set[i][j]];
            d2 = mans[office_set[i][j+1]];
            if(strcmp(d1.company , d2.company)==0){
              int bonus = d1.bonus_potential * d2.bonus_potential;
              points += bonus;
            }
          }
        }
      }


    }
  }
  return points;
}

void writeOutput(){
  FILE *f;

  f = fopen("output.txt", "w");
  for(int i=0; i<dev_number; i++){
    if(devs[i].x != -1){
      fprintf(f, "%d %d\n", devs[i].x, devs[i].y);
    }else{
      fprintf(f, "X\n");
    }
  }
  for(int i=0; i<man_number; i++){
    if(mans[i].x != -1){
      fprintf(f, "%d %d\n", mans[i].x, mans[i].y);
    }else{
      fprintf(f, "X\n");
    }
  }
  fclose(f);

}

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    int i, j;
    //apre il file giusto
    switch (argv[1][0])
    {
    case '1':
        file = fopen("tests/a_solar.txt", "r");
        break;
    case '2':
        file = fopen("tests/b_dream.txt", "r");
        break;
    case '3':
        file = fopen("tests/c_soup.txt", "r");
        break;
    case '4':
        file = fopen("tests/d_maelstrom.txt", "r");
        break;
    case '5':
        file = fopen("tests/e_igloos.txt", "r");
        break;
    case '6':
        file = fopen("tests/f_glitch.txt", "r");
        break;
    }

    fscanf(file, "%d %d", &width, &height);
    //printf("%d %d", height, width);

    //leggo ufficio
    for (int row = 0; row < height; ++row)
    {

        fscanf(file, "%*c");
        for (int col = 0; col < width; ++col)
        {
            char tile;
            office_set[row][col] = -1;
            fscanf(file, "%c", &tile);
            office[row][col] = tile;
        }
    }

    //print_office();


    fscanf(file, "%d%*c", &dev_number);
    //printf("%d\n", dev_number);

    //legge developer e li stampa
    for (i = 0; i < dev_number; ++i)
    {
        developer dev;
        dev.posizionato = 0;
        dev.x = -1;
        dev.y = -1;
        fscanf(file, "%s %d %d ", dev.company, &dev.bonus_potential, &dev.tot_skills);
        //printf("%s %d %d ",dev.company, dev.bonus_potential, dev.tot_skills);
        for (j = 0; j < dev.tot_skills; ++j)
        {
            // printf("%d", j);
            fscanf(file, "%s%*c", dev.skills[j]);
            //printf("%s ", dev.skills[j]);
        }
        printf("\n");
        devs[i] = dev;
    }

    //legge manager e li stampa
    fscanf(file, "%d%*c", &man_number);
    //printf("%d\n", man_number);
    for (i = 0; i < man_number; ++i)
    {
        manager man;
        man.posizionato = 0;
        man.x = -1;
        man.y = -1;
        fscanf(file, "%s %d%*c", man.company, &man.bonus_potential);

        mans[i] = man;

        //printf("%s %d\n", mans[i].company, mans[i].bonus_potential);
    }
    printf("Entro nel solver\n");
    solver();
    printf("Esco nel solver\n");
    for(i = 0; i<height; i++){
      for(j = 0; j<width; j++){
        printf("%d  ", office_set[i][j]);
      }
      printf("\n");
    }
    printf("%d\n", evaluate());
    writeOutput();
    return 1;
}
