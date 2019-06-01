/***** interface.c **********************************************************
 * Description: Visualize trees in Newick format
 * Author: Bernhard Haubold
 * Email: haubold@evolbio.mpg.de
 * License: GNU General Public License, https://www.gnu.org/licenses/gpl.html
 * Date: Sat Jun  1 12:00:34 2019
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "interface.h"
#include "eprintf.h"

Args *newArgs() {
  Args *args = (Args *)emalloc(sizeof(Args));
  args->h   = 0;
  args->v   = 0;
  args->err = 0;

  args->d   = DEFAULT_D;
  args->s   = 0.;
  args->u   = 0;
  args->r   = 0;
  args->n   = 0;
  args->l   = 0;
  args->a   = 0;
  
  return args;
}

void freeArgs(Args *args) {
  free(args);
}

Args *getArgs(int argc, char *argv[]) {
  int c;
  char *optString = "hvurnlad:s:";
  Args *args = newArgs();

  while ((c = getopt(argc, argv, optString)) != -1) {
    switch(c) {
    case 'd': /* dimension */
      args->d = atof(optarg);
      break;
    case 's': /* scale */
      args->s = atof(optarg);
      break;
    case 'u': /* unrooted */
      args->u = 1;
      break;
    case 'r': /* rooted */
      args->r = 1;
      break;
    case 'n': /* no labels */
      args->n = 1;
      break;
    case 'l': /* id labels */
      args->l = 1;
      break;
    case 'a': /* latex */
      args->a = 1;
      break;
    case 'h': /* help       */
      args->h = 1;
      break;
    case 'v': /* version    */
      args->v = 1;
      break;
    case '?':
      args->err = 1;
      if(optopt == 'i')
	fprintf(stderr, "Error: Option `%c` requires an argument.\n", optopt);
      else if(isprint(optopt))
	fprintf(stderr, "Error: Unknown option `%c`.\n", optopt);
      else
	fprintf(stderr, "Error: Unknown option character `\\x%x`.\n", optopt);
    default:
      args->err = 1;
      return args;
    }
  }
  args->fi = argv + optind;
  args->nf = argc - optind;

  return args;
}

void printUsage() {
  printf("Usage: %s [options] [inputFiles]\n", progname());
  printf("<DESCRIPTION>\n");
  printf("Example: %s -i 2\n", progname());
  printf("Options:\n");
  printf("\t[-d <NUM> smallest dimension (in cm); default: %.1f]\n", DEFAULT_D);
  printf("\t[-s <NUM> scale; default: find automatically]\n");
  printf("\t[-S no scale; default: print scale]\n");
  printf("\t[-u draw unrooted tree; default: let input decide]\n");
  printf("\t[-r draw rooted tree; default: let input decide]\n");
  printf("\t[-n no labels; default: print labels]\n");
  printf("\t[-l id labels; default: print text labels]\n");
  printf("\t[-a print LaTeX output; default: generate image]\n");
  printf("\t[-h print this help message and exit]\n");
  printf("\t[-v print program information and exit]\n");
  exit(0);
}

void printSplash(Args *args) {
  printf("%s ", progname());
  int l = strlen(VERSION);
  for(int i = 0; i < l - 1; i++)
    printf("%c", VERSION[i]);
  printf(", %s\n", DATE);
  printf("Author: Bernhard Haubold\n");
  printf("License: GNU General Public License, https://www.gnu.org/licenses/gpl.html\n");
  printf("Bugs: haubold@evolbio.mpg.de\n");
  exit(0);
}
