#include <math.h>

int calc_ang_inf(double p1x, double p1y, double p2x, double p2y){
    double a;
    double b;
    double m;

    a = p1x - p2x;
    b = p1y - p2y;

    if (a == 0 && b == 0){
        return 90;
    }

    m = b/a;

    int res;
    res = (int)(atan(m)*180/3.14);
    if (res < 0){
        return 180+res;
    }

    return res;
}

int calc_ang_sup(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, int r, int r2){
    double a1, a2;
    double b1, b2;
    double m1, m2;
    double dx, dy, d;

    a1 = p1x - p2x;
    b1 = p1y - p2y;

    a2 = p2x - p3x;
    b2 = p2y - p3y;

    if (a1 == 0 && b1 == 0){
        if (a2 == 0 && b2 == 0){
            return 90;
        }
        m2 = b2/a2;

        int res;
        res = (int)(atan(m2)*180/3.14);
        
        if (res < 0){
            return 180+res;
        }
        return res;
    }
    m1 = b1/a1;
    m2 = b2/a2;
    int res;

    res = (int)(atan((m1-m2)/(1+m2*m1))*180/3.14);

    dx = p3x - p1x;
    dy = p3y - p1y;
    d = hypot(dx,dy);
    if (d <= hypot(r,r2)){
        return -1;
    }
    return 90-res;
}
