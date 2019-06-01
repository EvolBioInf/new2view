/***** rooted.c ***********************************
 * Description: Compute rooted treee layout.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 12 17:22:29 2012
 **************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include "eprintf.h"

#include "tree.h"

void setXcoords(Node *np);
void scaleXcoords(Node *np, float xFact);
void setYcoords(Node *np, float yIncr);

float maxX;
float absY;

void layoutRootedTree(Node *np){
  float yIncr, xFact;

  maxX = 0;
  absY = 0;
  setXcoords(np);
  if(maxX < FLT_MIN)
    xFact = FLT_MAX;
  else
    xFact = 15./maxX;
  scaleXcoords(np,xFact);
  countLeaves(np);
  yIncr = 20./(float)np->l;
  setYcoords(np,yIncr);
}

void setYcoords(Node *np, float yIncr){
  float min, max;
  Node *cp;

  if(np != NULL){
    setYcoords(np->left, yIncr);
    if(np->left == NULL){     /* leaf */
      np->y = absY;
      absY += yIncr;
    }else{                    /* place node in middle of children */
      min = FLT_MAX;
      max = FLT_MIN;
      cp = np->left;
      while(cp){
	if(cp->y < min)
	  min = cp->y;
	if(cp->y > max)
	  max = cp->y;
	cp = cp->right;
      }
      np->y = (min+max)/2.;
    }
    setYcoords(np->right, yIncr);
  }
}

void scaleXcoords(Node *np, float xFact){
  if(np != NULL){
    scaleXcoords(np->left, xFact);
    scaleXcoords(np->right, xFact);
    np->x *= xFact;
  }
}

void setXcoords(Node *np){
  if(np != NULL){
    if(np->parent)
      np->x = np->dist + np->parent->x;
    if(np->x > maxX)
      maxX = np->x;
    setXcoords(np->left);
    setXcoords(np->right);
  }
}

