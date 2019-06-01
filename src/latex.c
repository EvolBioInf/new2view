/***** latex.c ************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 12 17:42:12 2012
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "tree.h"
#include "interface.h"
#include "eprintf.h"

void writeScale(FILE *fp, Args *args, Node *np);
void writeNodes(Node *np, Args *args, FILE *fp);
void writeEdges(Node *np, FILE *fp, Args *args);
void setupFileNameTemplate(char *n);
void findMaxDim(Node *np);
float absolute(float a);
void scalePos(Node *np, float scale);
void printN(Node *np, Args *args, FILE *fp, float labelAngle, float posAngle, int internal);
void findMaxDist(Node *np, MaxDist *md);
float findScale(float evolDist, float paperDist);

float maxX, maxY, minX, minY;

void printLatex(Node *np, char *fn, Args *args){
  float scale, min, max;
  char *of, *wf, *buf;
  FILE *fp;

  of = (char *)emalloc(256*sizeof(char));
  wf = (char *)emalloc(256*sizeof(char));
  setupFileNameTemplate(of);
  setupFileNameTemplate(wf);
  buf = emalloc(512*sizeof(char));
  if(args->a)
    fp = stdout;
  else
    fp = fdopen(mkstemp(of),"w");
  maxX = FLT_MIN;
  minX = FLT_MAX;
  maxY = FLT_MIN;
  minY = FLT_MAX;
  findMaxDim(np);
  min = minX < minY ? minX : minY;
  max = maxX > maxY ? maxX : maxY;
  min = absolute(min);
  max = min > max ? min : max;
  countLeaves(np);
  if(max>FLT_MIN)
    scale = args->d/max;
  else
    scale = 4./args->d;
  minX *= scale;
  minY *= scale;
  maxX *= scale;
  maxY *= scale;

  fprintf(fp,"\\begin{pspicture}(%4.3f,%4.3f)(%4.3f,%4.3f)\n",minX,minY,maxX,maxY+0.5);
  if(args->l)
    fprintf(fp,"\\psset{nodesep=2pt}\n");
  scalePos(np,scale);
  writeNodes(np,args,fp);
  writeEdges(np,fp,args);
  fprintf(fp,"\\end{pspicture}\n");
  if(!args->a){
    fclose(fp);
    fp = fdopen(mkstemp(wf),"w");
    printWrapper(fp, fn, of);
    fclose(fp);
    sprintf(buf,"latex -output-directory=/tmp -interaction=batchmode %s >/dev/null; dvips  %s -o %s.ps -q; gv %s.ps &",wf,wf,wf,wf);
    if(system(buf)){
      printf("ERROR: %s\n",buf);
      printf("ERROR: please make sure that the programs latex and gv are installed.\n");
      exit(-1);
    }
  }
  free(of);
  free(wf);
  free(buf);
}

void setupFileNameTemplate(char *n){
  int i;

  n[0] = '/';
  n[1] = 't';
  n[2] = 'm';
  n[3] = 'p';
  n[4] = '/';
  for(i=5;i<11;i++)
    n[i] = 'X';
  n[i] = '\0';
}
/* writeScale: Write LaTeX code for scalebar. */
void writeScale(FILE *fp, Args *args, Node *np){
  float dx, dy, l;
  float paperDist, evolDist, scale;
  MaxDist *md;

  md = (MaxDist *)emalloc(sizeof(MaxDist));
  md->dist = -1;
  findMaxDist(np, md);

  evolDist = md->dist;
  dx = md->x2 - md->x1;
  dy = md->y2 - md->y1;

  if(args->u)
    paperDist = sqrt(dx*dx + dy*dy);
  else
    paperDist = dx > 0 ? dx : -dx;
  if(args->s < 0)
    scale = findScale(evolDist, paperDist);
  else
    scale = args->s;
  if(scale > 0){
    l = paperDist*scale/evolDist;
    fprintf(fp,"\\psline(%f,%f)(%f,%f)\n",maxX-l,maxY+0.2,maxX,maxY+0.2);
    fprintf(fp,"\\rput(%f,%f){%g}\n",maxX-l/2.,maxY+0.4,scale);
  }
  free(md);
}

/* findScale: Compute factor such that the scale bar is between 1 and 2 cm long. */
float findScale(float evolDist, float paperDist){
  float incr = 1e-10;
  float scale = 1e-10;
  float lowerBound = 0.5;
  float upperBound = 2.5;
  float l;
  int i;

  if(evolDist <= 0.)
    return -1;
  l = 0.;
  while(!(lowerBound<l && upperBound>l)){
    if(l > upperBound)
      return -1;
    for(i=0;i<9;i++){
      l = paperDist*scale/evolDist;
      if(lowerBound<l && upperBound>l)
	return scale;
      scale += incr;
    }
    incr *= 10;
  }
  return scale;
}

void findMaxDist(Node *np, MaxDist *md){
  if(np != NULL){
    findMaxDist(np->left, md);
    if(md->dist < np->dist){
      md->dist = np->dist;
      md->x1 = np->x;
      md->y1 = np->y;
      if(np->parent){
	md->x2 = np->parent->x;
	md->y2 = np->parent->y;
      }
    }
    findMaxDist(np->right,md);
  }
}

void scalePos(Node *np, float scale){
  if(np != NULL){
    scalePos(np->left,scale);
    np->x *= scale;
    np->y *= scale;
    scalePos(np->right,scale);
  }
}

void printWrapper(FILE *fp, char *fn, char *wrappedFile){

  fprintf(fp,"\\documentclass{article}\n");
  fprintf(fp,"\\usepackage{graphics,color,eurosym,latexsym}\n");
  fprintf(fp,"\\usepackage{times}\n");
  fprintf(fp,"\\usepackage{pstricks,pst-node}\n");
  fprintf(fp,"\\pagestyle{empty}\n");
  fprintf(fp,"\\oddsidemargin=0cm\n");
  fprintf(fp,"\\evensidemargin=0cm\n");
  fprintf(fp,"\\headheight=0cm\n");
  fprintf(fp,"\\headsep=0cm\n");
  fprintf(fp,"\\textwidth=16cm\n");
  fprintf(fp,"\\textheight=23cm\n");
  fprintf(fp,"\\begin{document}\n");
  fprintf(fp,"\n");
  fprintf(fp,"\\title{\\texttt{%s}}\n", fn);
  fprintf(fp, "\\author{}\n");
  fprintf(fp, "\\date{}\n");
  fprintf(fp, "\\maketitle\n");
  fprintf(fp, "\\n");
  fprintf(fp,"\\begin{center}\n");
  fprintf(fp,"\\input{%s}\n",wrappedFile);
  fprintf(fp,"\\end{center}\n");
  fprintf(fp,"\n");
  fprintf(fp,"\\end{document}\n");
}

void findMaxDim(Node *np){
  if(np != NULL){
    findMaxDim(np->left);
    if(np->x > maxX)
      maxX = np->x;
    if(np->x < minX)
      minX = np->x;
    if(np->y > maxY)
      maxY = np->y;
    if(np->y < minY)
      minY = np->y;
    findMaxDim(np->right);
  }
}

void writeNodes(Node *np, Args *args, FILE *fp){
  float posAngle, labelAngle;
  int internal;

  if(np != NULL){
    writeNodes(np->left,args,fp);

    if(np->left == NULL){          /* leaf */
      if(args->r){
	labelAngle = 0;
	posAngle = 0;
      }else{
	labelAngle = (np->tau + np->omega/2)*180/PI;
	posAngle = labelAngle;
	if(labelAngle > 90 && labelAngle < 270){
	  posAngle = labelAngle - 10;
	  labelAngle += 180;
	}
      }
    }else if(np->parent != NULL){  /* internal */
      labelAngle = 0;
      if(args->r)
	posAngle = 135;
      else
	posAngle = (np->tau + np->omega/2)*180/PI + 90;
    }else{                         /* root */   
      labelAngle = 0;
      if(args->r)
	posAngle = 180;
      else
	posAngle = (np->tau + np->omega/2)*180/PI + 45;
    }
    if(np->left)
      internal = 1;
    else
      internal = 0;
    printN(np,args,fp,labelAngle,posAngle,internal);

    writeNodes(np->right,args,fp);
  }
}

void printN(Node *np, Args *args, FILE *fp, float labelAngle, float posAngle, int internal){
  int i;

  if(args->l)
    fprintf(fp,"\\rput(%f,%f){\\rnode{n%d}{%d}}",np->x,np->y,np->id,np->id);
  else{
    fprintf(fp,"\\rput(%f,%f){\\rnode{n%d}{}}",np->x,np->y,np->id);
    if(strlen(np->label) > 0 && !args->n){
      fprintf(fp,"\\uput{4pt}[%f]{%f}(n%d){",posAngle,labelAngle,np->id);
      if(internal){
	if(np->label)
	  fprintf(fp,"\\small ");
      }
      for(i=0;i<strlen(np->label);i++){
	if(np->label[i] == '_'){
	  fprintf(fp, " ");
	}else
	  fprintf(fp,"%c",np->label[i]);
      }
      fprintf(fp,"}");
    }
  }
  fprintf(fp,"\n");
}

void writeEdges(Node *np, FILE *fp, Args *args){
  if(np != NULL){
    writeEdges(np->left,fp,args);
    if(np->parent != NULL){
      if(np->left != NULL){
	if(args->r){
	  if(np->parent->y < np->y)
	    fprintf(fp,"\\ncangle[angleB=180,angleA=90,armB=0]{n%d}{n%d}\n",np->parent->id,np->id);
	  else
	    fprintf(fp,"\\ncangle[angleB=180,angleA=-90,armB=0]{n%d}{n%d}\n",np->parent->id,np->id);
	}else
	  fprintf(fp,"\\ncline{n%d}{n%d}\n",np->id,np->parent->id);
      }else{
	if(args->r){
	  if(np->parent->y < np->y)
	    fprintf(fp,"\\ncangle[angleB=180,angleA=90,armB=0]{n%d}{n%d}\n",np->parent->id,np->id);
	  else
	    fprintf(fp,"\\ncangle[angleB=180,angleA=-90,armB=0]{n%d}{n%d}\n",np->parent->id,np->id);
	}else
	  fprintf(fp,"\\ncline{n%d}{n%d}\n",np->id,np->parent->id);
      }
    }
    writeEdges(np->right,fp,args);
  }
}

float absolute(float a){
  return a > 0 ? a : -a;
}
