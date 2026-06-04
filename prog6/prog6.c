#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Buffer: simple 2D character buffer */
typedef struct {
    int width;
    int height;
    char *data; /* row-major, size = width*height */
} Buffer;

/* Circle structure */
typedef struct {
    int cx;
    int cy;
    int r;
    char ch; /* drawing character */
} Circle;

/* Buffer utilities */
Buffer *buffer_create(int width, int height);
void buffer_destroy(Buffer *b);
void buffer_clear(Buffer *b, char fill);
void buffer_set(Buffer *b, int x, int y, char ch);
void buffer_print(const Buffer *b);

/* Circle drawing */
void draw_circle(Buffer *b, const Circle *c);

/* helpers */
static void plot8(Buffer *b, int cx, int cy, int x, int y, char ch);

Buffer *buffer_create(int width, int height)
{
    Buffer *b = malloc(sizeof(Buffer));
    if (!b) return NULL;
    b->width = width;
    b->height = height;
    b->data = malloc((size_t)width * height);
    if (!b->data) { free(b); return NULL; }
    buffer_clear(b, ' ');
    return b;
}

void buffer_destroy(Buffer *b)
{
    if (!b) return;
    free(b->data);
    free(b);
}

void buffer_clear(Buffer *b, char fill)
{
    if (!b || !b->data) return;
    memset(b->data, (unsigned char)fill, (size_t)b->width * b->height);
}

void buffer_set(Buffer *b, int x, int y, char ch)
{
    if (!b) return;
    if (x < 0 || x >= b->width || y < 0 || y >= b->height) return;
    b->data[y * b->width + x] = ch;
}

void buffer_print(const Buffer *b)
{
    if (!b) return;
    for (int y = 0; y < b->height; ++y) {
        fwrite(&b->data[y * b->width], 1, b->width, stdout);
        putchar('\n');
    }
}

/* Midpoint circle algorithm plotting the 8-way symmetric points */
void draw_circle(Buffer *b, const Circle *c)
{
    if (!b || !c) return;
    int x = 0;
    int y = c->r;
    int d = 1 - c->r;
    plot8(b, c->cx, c->cy, x, y, c->ch);
    while (y > x) {
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y -= 1;
        }
        x += 1;
        plot8(b, c->cx, c->cy, x, y, c->ch);
    }
}

static void plot8(Buffer *b, int cx, int cy, int x, int y, char ch)
{
    buffer_set(b, cx + x, cy + y, ch);
    buffer_set(b, cx - x, cy + y, ch);
    buffer_set(b, cx + x, cy - y, ch);
    buffer_set(b, cx - x, cy - y, ch);
    buffer_set(b, cx + y, cy + x, ch);
    buffer_set(b, cx - y, cy + x, ch);
    buffer_set(b, cx + y, cy - x, ch);
    buffer_set(b, cx - y, cy - x, ch);
}

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    int width = 60;
    int height = 30;
    Buffer *b = buffer_create(width, height);
    if (!b) return 2;

    /* Example circle centered roughly in the middle */
    Circle c;
    c.cx = width / 2;
    c.cy = height / 2;
    c.r = (height < width ? height : width) / 4; /* reasonable radius */
    c.ch = '*';

    draw_circle(b, &c);
    buffer_print(b);
    buffer_destroy(b);
    return 0;
}
