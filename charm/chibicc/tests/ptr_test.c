int *gp = 0;
int z = 0;
int f() {
    int x = 5;
    int *lp = &x;
    int  y = *lp + 5;
    gp = &y;
    z = *gp;
}
