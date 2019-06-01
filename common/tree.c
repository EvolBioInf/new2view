/***** tree.c *************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 12 10:38:36 2012
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "eprintf.h"
#include "y.tab.h"

int nodeId;

Node *parseTree(){
  nodeId = 0;
  if(yyparse())
    return getTree();
  else
    return NULL;
}

Node *addNode(Node *curN, Node *newN){
  Node *parent;

  if(curN == NULL){
    curN = newN;
  }else if(curN->left == NULL){
    curN->left = newN;
    newN->parent = curN;
    curN = newN;
  }else{
    parent = curN;
    curN = curN->left;
    while(curN->right != NULL)
      curN = curN->right;
    curN->right = newN;
    curN = curN->right;
    curN->parent = parent;
  }

  return curN;
}

Node *newNode(){
  Node *np;

  np = (Node *)emalloc(sizeof(Node));
  np->left = NULL;
  np->right = NULL;
  np->parent = NULL;
  np->label = NULL;

  np->omega = -1;
  np->tau = -1;
  
  np->dist = 0;
  np->id = ++nodeId;
  np->bl = 0;
  np->x = 0;
  return np;
}

void countLeaves(Node *np){
  Node *cp;

  if(np != NULL){
    countLeaves(np->left);
    countLeaves(np->right);
    if(np->left == NULL)
      np->l = 1;
    else{
      np->l = 0;
      cp = np->left;
      while(cp != NULL){
	np->l += cp->l;
	cp = cp->right;
      }
    }
  }
}


int deg(Node *np){
  int d;
  Node *n;

  if(np->parent)
    d = 1;
  else
    d = 0;
  n = np->left;
  while(n != NULL){
    d++;
    n = n->right;
  }
  return d;
}
