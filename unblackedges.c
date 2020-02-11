/* Comp 40 
 * Homework 2
 * Emily Hu and Elizabeth Endo
 * 2/10/2020
 *
 * unblackedges.c
 * removes black edge pixels from a Portable Bitmap image
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>
#include <pnmrdr.h>
#include <seq.h>

#include "bit2.h"

void replace_black_edges(Bit2_T pixmap);
void traverse_black_edge(Bit2_T pixmap, int row, int col);
void neighbor_location(int *row, int *col, int dir);
bool is_black_neighbor(Bit2_T pixmap, int row, int col);
void apply_populate(int row, int col, Bit2_T pixmap, int bit_val, void *cl);
Bit2_T read_file(FILE *fp);
void print_image(Bit2_T pixmap);
void apply_print(int x, int y, Bit2_T pixmap, int bit_val, void *fp);
bool search_sequence(Seq_T row_seq, Seq_T col_seq, int row, int col);
void continue_path(Bit2_T pixmap, Seq_T row_queue, Seq_T col_queue);

const int BLACK = 1;
const int WHITE = 0;

const int UP    = 0;
const int RIGHT = 1;
const int DOWN  = 2;
const int LEFT  = 3;

const int HINT  = 20;

int main(int argc, char *argv[])
{
    assert(argc <= 2);

    Bit2_T pixmap;

    if (argc == 2) {
        FILE *fp = fopen(argv[1], "rb");
        pixmap = read_file(fp);
        assert(pixmap != NULL);
        fclose(fp);
    } else if (argc == 1) {
        pixmap = read_file(stdin);
    } 

    replace_black_edges(pixmap);
    print_image(pixmap);

    Bit2_free(&pixmap);
}

/* write file
 * parameters: pixel map
 * returns: void
 * writes pixel map data to output file
 */
void print_image(Bit2_T pixmap)
{
    printf("P1\n");
    printf("# unblacked edges\n");
    printf("%d %d\n", Bit2_width(pixmap), Bit2_height(pixmap));
    Bit2_map_row_major(pixmap, apply_print, stdout);
}

/* apply write
 * parameters: location, pixel map, bit value at location, closure: file ptr
 * returns: void
 * writes one bit value at a time to file
 */
void apply_print(int col, int row, Bit2_T pixmap, int bit_val, void *fp)
{
    (void) row;

    fprintf((FILE *)fp, "%d", bit_val);
    if (col == Bit2_width(pixmap) - 1) {
        fprintf((FILE *)fp, "\n");
    }
}

/* replace black edges
 * parameters: pixel map
 * returns: void
 * makes all black edges white
 */
void replace_black_edges(Bit2_T pixmap)
{
    assert(pixmap != NULL);

    int index;
    /* left and right edges */
    for (index = 0; index < Bit2_height(pixmap); index++) {
        traverse_black_edge(pixmap, index, 0);
        traverse_black_edge(pixmap, index, Bit2_width(pixmap) - 1);
    }
    /* top and bottom edges */
    for (index = 0; index < Bit2_width(pixmap); index++) {
       traverse_black_edge(pixmap, 0, index);
       traverse_black_edge(pixmap, Bit2_height(pixmap) - 1, index);
    }
}

/* traverse black edge
 * parameters: pixel map, location
 * returns: void
 * traverses a given black edge path
 */
void traverse_black_edge(Bit2_T pixmap, int row, int col)
{   
    /* if black, make it and connecting black edge pixels white */
    if (Bit2_get(pixmap, col, row) == BLACK) {
        /* queues for black edge row & col coordinates */
        Seq_T row_queue = Seq_new(HINT);
        Seq_T col_queue = Seq_new(HINT);

        int *first_row = malloc(sizeof(int));
        int *first_col = malloc(sizeof(int));

        *first_row = row;
        *first_col = col;

        Seq_addhi(row_queue, first_row);
        Seq_addhi(col_queue, first_col);

        /* while there are still connecting black edge pixels, 
           make them white */
        while (Seq_length(row_queue) > 0) {
            continue_path(pixmap, row_queue, col_queue);
        }
        Seq_free(&row_queue);        
        Seq_free(&col_queue);    
        return;    
    }
    return;
}

/* continue path
 * parameters: pixel map, row queue, col queue
 * returns: void
 * makes first position in queue white and adds black neighbors to queues
 */
void continue_path(Bit2_T pixmap, Seq_T row_queue, Seq_T col_queue) 
{
    /* make first black edge pixel in parallel queues white */
    int *cur_col = (int *)Seq_remlo(col_queue);
    int *cur_row = (int *)Seq_remlo(row_queue);
    Bit2_put(pixmap, *cur_col, *cur_row, WHITE);

    /* add neighboring black pixels to parallel queues */
    int dir;
    for (dir = 0; dir <= 3; dir++) {
        int *nbr_row = malloc(sizeof(int));
        int *nbr_col = malloc(sizeof(int));
        *nbr_row = *cur_row;
        *nbr_col = *cur_col;
        neighbor_location(nbr_row, nbr_col, dir);

        if (is_black_neighbor(pixmap, *nbr_row, *nbr_col) &&
           !search_sequence(row_queue, col_queue, *nbr_row, *nbr_col)) {
                Seq_addhi(row_queue, nbr_row);
                Seq_addhi(col_queue, nbr_col);
        } else {
            free(nbr_row);
            free(nbr_col);
        }
    }
    free(cur_col);
    free(cur_row);
}

/* search seq
 * parameters: row queue, col queue, search target coordinates
 * returns: T if specified coordinate pair exists in parallel sequences,
 *          F otherwise
 * iterates through sequences to determine whether target coordinates already
 * exist within the parallel sequences
 */
bool search_sequence(Seq_T row_seq, Seq_T col_seq, int row, int col)
{
    int i;
    int seq_len = Seq_length(row_seq);
    for (i = 0; i < seq_len; i++) {
        if (*(int *)Seq_get(row_seq, i) == row &&
            *(int *)Seq_get(col_seq, i) == col) {
            return true;
        }
    }
    return false;
}

/* neighbor location
 * parameters: position (by ref), direction number (order up/right/down/left),
 *             0 <= dir <= 3
 * returns: void
 * updates location to neighbor location
 */
void neighbor_location(int *row, int *col, int dir)
{
    if (dir == UP) {
        (*row)--;
    } if (dir == RIGHT) {
        (*col)++;
    } if (dir == DOWN) {
        (*row)++;
    } if (dir == LEFT) {
        (*col)--;
    }
}

/* is black neighbor
 * parameters: pixel map, position
 * returns: T if black neighbor, F if not
 * evaluates if given neighbor is black pixel
 */
bool is_black_neighbor(Bit2_T pixmap, int row, int col)
{
    /* if out of bounds, return F */
    if (row < 0 || row == Bit2_height(pixmap) ||
        col < 0 || col == Bit2_width(pixmap)) {
        return false;
    }
    if (Bit2_get(pixmap, col, row) == BLACK) {
        return true;
    } else {
        return false;
    }
}

/* read file
 * parameters: file pointer
 * returns: populated Bit2_T 
 * populates Bit2_T with bits from file
 */
Bit2_T read_file(FILE *fp)
{
    if (fp == NULL) {
        printf("error: could not open file\n");
        exit(EXIT_FAILURE);
    }

    Pnmrdr_T rdr;

    TRY
        rdr = Pnmrdr_new(fp);
    EXCEPT(Pnmrdr_Badformat)
        exit(EXIT_FAILURE);
    END_TRY;

    int height = (int)Pnmrdr_data(rdr).height;
    int width  = (int)Pnmrdr_data(rdr).width;

    assert(Pnmrdr_data(rdr).type == 1);
    /* new Bit2 array called pixmap */
    Bit2_T pixmap = Bit2_new(width, height);
    /* fill pixmap with pixels from image */
    Bit2_map_row_major(pixmap, apply_populate, &rdr);

    Pnmrdr_free(&rdr);

    return pixmap;
}

/* apply populate
 * parameters: position, pixel map, bit value, closure: Pnmrdr object
 * returns: void 
 * apply function to populate given bit cell
 */
void apply_populate(int row, int col, Bit2_T pixmap, int bit_val, void *cl)
{
    (void) bit_val;
    
    Bit2_put(pixmap, row, col, Pnmrdr_get(*(Pnmrdr_T *)cl));
}
