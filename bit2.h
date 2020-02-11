/* Comp 40 
 * Homework 2
 * Emily Hu and Elizabeth Endo
 * 2/10/2020
 *
 * bit2 interface
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Bit2_T *Bit2_T;

/* Allocates, initializes, and returns a new 2D pixel map with dimensions 
 * (width, height) each element being a bit. All parameters should be > 0.
 */
Bit2_T Bit2_new(int width, int height);

/* Deallocates and clears *pixmap. pixmap should not be NULL.
 */
void Bit2_free(Bit2_T *pixmap);

/* Puts value in pixmap at (col, row) and returns value previously stored there.
 * x, y should be within the bounds of the pixel map. pixmap should not be NULL.
 * value should be 0 or 1.
 */
int Bit2_put(Bit2_T pixmap, int col, int row, int value);

/* Returns value stored at coordinates (col, row) in the Bit2 pixel map.
 * x, y should be within the bounds of the pixmap. pixmap should not be NULL.
 */
int Bit2_get(Bit2_T pixmap, int col, int row);

/* Returns the number of columns in the Bit2 pixel map. pixmap should be a valid
 * Bit2 pixel map and return value should be greater than 0.
 */
int Bit2_width(Bit2_T pixmap);

/* Returns the number of rows in the Bit2 pixmap. pixmap should be a valid
 * Bit2 pixel map and return value should be greater than 0.
 */
int Bit2_height(Bit2_T pixmap);

/* Calls apply for each element in the Bit2 pixmap, indexing through the rows
 * of one column at a time. pixmap should be a valid Bit2 pixel map.
 */
void Bit2_map_col_major(Bit2_T pixmap, 
        void apply(int col, int row, Bit2_T pixmap, int bit_val, void *cl),
        void *cl);

/* Calls apply for each element in the Bit2 pixmap, indexing through the
 * columns of one row at a time. pixmap should be a valid Bit2 pixel map.
 */
void Bit2_map_row_major(Bit2_T pixmap, 
        void apply(int col, int row, Bit2_T pixmap, int bit_val, void *cl),
        void *cl);
