#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double TAUX_MIN = 0.1;
const double TAUX_MAX = 10;

const double CAPITAL_MIN = 1000;
const double CAPITAL_MAX = 1000000;

const long int DUREE_MIN = 1;
const long int DUREE_MAX = 600;

const char* program_name;

void print_usage(FILE* stream, int exit_code) {
  fprintf(stream, "Usage: %s [options]\n", program_name);
  fprintf(stream, "  -h --help            Affiche l'aide\n");
  fprintf(stream, "  -c --capital capital Capital emprunté entre %.0f et %.0f\n", CAPITAL_MIN, CAPITAL_MAX);
  fprintf(stream, "  -t --taux taux       Taux entre %.2f%% et %.2f%%\n", TAUX_MIN, TAUX_MAX);
  fprintf(stream, "  -d --duree mois      Nombre de mensualités entre %ld et %ld\n", DUREE_MIN, DUREE_MAX);
  exit(exit_code);
}

typedef struct Echeance_Struct Echeance;
struct Echeance_Struct {
  long int numero;
  double restantdu;
  double principal;
  double interets;
  double mensualite;
};

int main(int argc, char* argv[]) {

  int next_option;
  const char* const short_options = "hc:t:d:";
  const struct option long_options[] = {
    { "help",     1, NULL, 'h' },
    { "capital",  1, NULL, 'c' },
    { "taux",     1, NULL, 't' },
    { "duree",    1, NULL, 'd' },
    { NULL     ,  0, NULL, 0   },
  };

  float capital = 100000;
  float tauxAnnuel = 1;
  long int duree = 120;

  program_name = argv[0];
  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);
    switch(next_option) {
      case 'h':
        print_usage(stdout, 0);
      case 'c': 
        {
          char* end;
          capital = strtof(optarg, &end);
	  if(*end != '\0' || capital < CAPITAL_MIN || capital > CAPITAL_MAX) print_usage(stdout, 1);
        }
        break;
      case 't': 
        {
          char* end;
          tauxAnnuel = strtof(optarg, &end);
	  if(*end != '\0' || tauxAnnuel < TAUX_MIN || tauxAnnuel > TAUX_MAX) print_usage(stdout, 1);
        }
	break;
      case 'd': 
        {
          char* end;
          duree = strtol(optarg, &end, 10);
	  if(*end != '\0' || duree < DUREE_MIN || duree > DUREE_MAX) print_usage(stdout, 1);
        }
        break;
      case '?': /* Invalid option */
        print_usage(stdout, 1);
      case -1: /* End of the options */
        break;
      default: /* Something that is unusual */
        abort();
    }
  } while(next_option != -1);

  Echeance* echeances = (Echeance*)malloc(sizeof(Echeance) * duree);

  double tauxPeriode = tauxAnnuel / 12;

  for(long int i = 0; i < duree; i++) {
    echeances[i].numero = i + 1;
    if(i == 0) echeances[i].restantdu = capital;
    else echeances[i].restantdu = echeances[i - 1].restantdu - echeances[i - 1].principal;
    echeances[i].mensualite = (capital * tauxPeriode / 100) / (1 - pow(1 + tauxPeriode / 100, -duree));
    echeances[i].interets = echeances[i].restantdu * tauxPeriode / 100;
    echeances[i].principal = echeances[i].mensualite - echeances[i].interets;
  }
  
  printf("+---------------------------------------------------------+\n");
  printf("| Capital emprunté: %9.0f / Taux: %5.2f%% / Durée: %3ld |\n", capital, tauxAnnuel, duree);
  printf("+-----+------------+------------+------------+------------+\n");
  printf("| Num | Restant dû |   Intérêts | Principal  | Mensualité |\n");
  printf("+-----+------------+------------+------------+------------+\n");
  for(long int i = 0; i < duree; i++) {
    printf("| %3ld | %10.2f | %10.2f | %10.2f | %10.2f |\n",
    	echeances[i].numero,
	echeances[i].restantdu,
	echeances[i].interets,
	echeances[i].principal,
	echeances[i].mensualite);
  }
  printf("+-----+------------+------------+------------+------------+\n");

  free(echeances);
  return 0;
}
