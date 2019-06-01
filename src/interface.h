/***** interface.h **********************************************************
 * Description: Visualize trees in Newick format
 * Author: Bernhard Haubold
 * Email: haubold@evolbio.mpg.de
 * License: GNU General Public License, https://www.gnu.org/licenses/gpl.html
 * Date: Sat Jun  1 12:00:34 2019
 ****************************************************************************/
#ifndef INTERFACE
#define INTERFACE

#define DEFAULT_D 10. /* default min. dim. */

/* define argument container */
typedef struct args{
  char    h; /* help message?         */
  char    v; /* version message?      */
  char  err; /* error                 */
  char **fi; /* input files           */
  int    nf; /* number of input files */

  float   d; /* dimension             */
  float   s; /* scale                 */
  char    u; /* unrooted              */
  char    r; /* rooted                */
  char    n; /* no labels             */
  char    l; /* id labels             */
  char    a; /* latex output          */
} Args;

Args *getArgs(int argc, char *argv[]);
Args *newArgs();
void freeArgs(Args *args);
void printUsage();
void printSplash(Args *args);

#endif
