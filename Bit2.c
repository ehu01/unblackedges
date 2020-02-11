/* Comp 40 
 * Homework 2
 * Emily Hu and Elizabeth Endo
 * 2/10/2020
 *
 * bit2 implementation
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <uarray.h>
#include <assert.h>
#include <bit.h>

#include "Bit2.h"

struct Bit2_T {
    int width;
    int height;
    UArray_T pixmap;
};

Bit2_T Bit2_new(int width, int height) 
{
    assert(width > 0 && height > 0);

    UArray_T h_array = UArray_new(width, sizeof(Bit_T));

    Bit2_T bit2 = malloc(sizeof(struct Bit2_T));

    assert(bit2 != NULL && h_array != NULL);

    bit2->width = width;
    bit2->height = height;
    bit2->pixmap = h_array;

    int i;
    for (i = 0; i < width; i++) {
        *(Bit_T *)UArray_at(h_array, i) = Bit_new(height);
    }
 
    return bit2;
}

void Bit2_free(Bit2_T *pixmap)
{
    assert(pixmap != NULL);

    int i;
    for (i = 0; i < Bit2_width(*pixmap); i++) {
        assert(UArray_at((*pixmap)->pixmap, i) != NULL);
        Bit_free((Bit_T *)UArray_at((*pixmap)->pixmap, i));
    }

    UArray_free(&(*pixmap)->pixmap);
    free(*pixmap);
}

int Bit2_get(Bit2_T pixmap, int col, int row)
{
    assert(pixmap != NULL);
    assert(col >= 0 && col < pixmap->width);
    assert(row >= 0 && row < pixmap->height);

    return Bit_get(*(Bit_T *)UArray_at(pixmap->pixmap, col), row);
}

int Bit2_put(Bit2_T pixmap, int col, int row, int value)
{
    assert(pixmap != NULL);
    assert(col >= 0 && col < pixmap->width);
    assert(row >= 0 && row < pixmap->height);
    assert(value == 0 || value == 1);

    return Bit_put(*(Bit_T *)UArray_at(pixmap->pixmap, col), row, value);
}

int Bit2_width(Bit2_T pixmap)
{
    assert(pixmap != NULL);

    return pixmap->width;
}

int Bit2_height(Bit2_T pixmap)
{
    assert(pixmap != NULL);

    return pixmap->height;
}

void Bit2_map_col_major(Bit2_T pixmap, 
        void apply(int i, int j, Bit2_T pixmap, int bit_val, void *cl), 
        void *cl)
{
    assert(pixmap != NULL);

    int col, row;
    for (col = 0; col < pixmap->width; col++) {
        for (row = 0; row < pixmap->height; row++) {
            apply(col, row, pixmap, Bit2_get(pixmap, col, row), cl);
        }
    }
}

void Bit2_map_row_major(Bit2_T pixmap, 
        void apply(int col, int row, Bit2_T pixmap, int bit_val, void *cl),
        void *cl)
{
    assert(pixmap != NULL);

    int col, row;
    for (row = 0; row < pixmap->height; row++) {
        for (col = 0; col < pixmap->width; col++) {
            apply(col, row, pixmap, Bit2_get(pixmap, col, row), cl);
        }
    }
}
