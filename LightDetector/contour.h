#ifndef CONTOUR_H
#define CONTOUR_H

#include <QPoint>
#include <QVector>


class Contour
{
public:
    Contour(QVector<QPoint> cP);
    ~Contour();

private:
   QVector<QPoint> conPoints;

};

#endif // CONTOUR_H
