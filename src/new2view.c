/***** new2view.c ***********************************************************
 * Description: Visualize trees in Newick format
 * Author: Bernhard Haubold
 * Email: haubold@evolbio.mpg.de
 * License: GNU General Public License, https://www.gnu.org/licenses/gpl.html
 * Date: Sat Jun  1 12:00:34 2019
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "interface.h"
#include "eprintf.h"
#include "tree.h"

void scanFile(FILE *fp, char *fn, Args *args){
  Node *root;

  setBisonFile(fp);
  while((root = parseTree()) != NULL){
    root->x = root->y = 0; /* avoid valgrind warning if > 1 tree analyzed */
    if(deg(root) == 2 && !args->u){
      args->r = 1;
      layoutRootedTree(root);
    }else if(deg(root) == 3 && !args->r){
      args->u = 1;
      layoutUnrootedTree(root);
    }else if(args->r){
      layoutRootedTree(root);
    }else if(args->u){
      layoutUnrootedTree(root);
    }else{
      args->r = 1;
      layoutRootedTree(root);
    }
    printLatex(root, fn, args);
  }
}

int main(int argc, char *argv[]){
  FILE *fp;
  Args *args = getArgs(argc, argv);

  setprogname2(argv[0]);
  if(args->v)
    printSplash(args);
  if(args->h || args->err)
    printUsage();
  if(args->nf == 0) {
    fp = stdin;
    scanFile(fp, "stdin", args);
  } else {
    for(int i = 0; i < args->nf; i++) {
      fp = efopen(args->fi[i], "r");
      scanFile(fp, args->fi[i], args);
      fclose(fp);
    }
  }
  freeArgs(args);
  free(progname());
  return 0;
}

