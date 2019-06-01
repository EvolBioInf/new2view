/***** unrooted.c *********************************
 * Description: Compute unrooted (radial) tree.
 * Reference: Bachmaier, C. Brandes, U. and
 *   Schlieper, B. (2005). Drawing Phylogenetic
 *   Trees. X. Deng and D. u (Eds.): ISAAC 2005, 
 *   LNCS 3827, pp. 2158/1-2. Springer, Heidelberg.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 12 17:22:29 2012
 **************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "eprintf.h"

#include "tree.h"

void unrootedLayout(Node *v, int numLeaves){
  Node *u, *w;
  float eta;

  if(v != NULL){
    if(v->parent != NULL){
      u = v->parent;
      v->x = u->x + v->dist * (cos(v->tau*1 + v->omega/2.));
      v->y = u->y + v->dist * (sin(v->tau*1 + v->omega/2.));
    }
    eta = v->tau;
    w = v->left;
    while(w != NULL){
      w->omega = (float)(w->l)/(float)(numLeaves) * 2. * PI;
      w->tau = eta;
      eta += w->omega;
      w = w->right;
    }
    unrootedLayout(v->left,numLeaves);
    unrootedLayout(v->right,numLeaves);
  }
}

void layoutUnrootedTree(Node *np){
  countLeaves(np);
  unrootedLayout(np,np->l);
}
