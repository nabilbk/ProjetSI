#include "circle.h"

#include <cmath>
#include <limits>

Circle::Circle(double x, double y, double r)
    : x(x), y(y), r(r), i(0)
{
}

Circle::Circle(list<pair<int, int> > edge)
    : x(0), y(0), r(0), i(0), edge(edge)
{
    // Itérateurs que l'on va réutiliser
    list<pair<int, int> >::iterator beg, end = edge.end();

    // Calcul de la position du centre du cercle
    // C'est la moyenne de la position des points de contour
    for (beg = edge.begin(); beg != end; ++beg)
    {
        x += beg->first;
        y += beg->second;
    }

    x /= double(edge.size());
    y /= double(edge.size());

    // Calcul du rayon du cercle
    // C'est la moyenne des distances des points de contour au centre du cercle
    for (beg = edge.begin(); beg != end; ++beg)
    {
        double d0, d1;

        d0  = (beg->first  - x);
        d0 *= d0;

        d1  = (beg->second - y);
        d1 *= d1;

        r  += sqrt(d0 + d1);
    }

    r /= double(edge.size());

    // Calcul de l'irrégularité du cercle
    // C'est la moyenne des distances des points de contour au rayon du cercle
    for (beg = edge.begin(); beg != end; ++beg)
    {
        double d0, d1;

        d0  = (beg->first  - x);
        d0 *= d0;

        d1  = (beg->second - y);
        d1 *= d1;

        i += abs(sqrt(d0 + d1) - r);
    }

    i /= double(edge.size());
}

Circle::~Circle()
{
}

bool operator==(Circle lhs, Circle rhs)
{
    double epsilon = 1.0e-7;

    return (abs(lhs.x - rhs.x) < epsilon) &&
           (abs(lhs.y - rhs.y) < epsilon) &&
           (abs(lhs.r - rhs.r) < epsilon) &&
           (abs(lhs.i - rhs.i) < epsilon);
}
