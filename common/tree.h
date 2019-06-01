/***** tree.h *************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 12 10:35:29 2012
 **************************************************/
#ifndef TREE
#define TREE
#include "../src/interface.h"

#define PI 3.1415926535897932384626433832795028841971
/* define tree node */
typedef struct node{
  struct node *left;
  struct node *right;
  struct node *parent;
  char *label;
  float dist;
  float bl;           /* branch length in  subtree */
  int id;
  int l;              /* number of leaves in subtree */
  float tau;
  float omega;
  float x;
  float y;
} Node;

typedef struct maxDist{
  float dist;
  float x1, x2, y1, y2;
} MaxDist;

Node *parseTree();
Node *newNode();
void printTree(Node *np);
Node *addNode(Node *parent, Node *child);
void printInorder(Node *np);
void printPreorder(Node *np);
void printPostorder(Node *np);
int deg(Node *np);

void layoutUnrootedTree(Node *np);

void printLatex(Node *np, char *fn, Args *args);
void printWrapper(FILE *fw, char *fn, char *wrappedFile);

void countLeaves(Node *np);

void layoutRootedTree(Node *np);

/* flex/bison declarations */
void setBisonFile(FILE *fp);
void yyerror(const char *str); 
Node *getTree();
int yyparse();

#endif
