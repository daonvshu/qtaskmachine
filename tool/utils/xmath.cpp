#include "xmath.h"

namespace xmath {

    qreal p2AbArea(QPointF p1, QPointF p2, QPointF p) {
        return (p1.x() - p.x()) * (p2.y() - p.y()) - (p1.y() - p.y()) * (p2.x() - p.x());
    }

    QPointF getParallelPoint(const QLineF& line, const QPointF& pointAtLine, qreal r, bool left) {
        qreal d12 = line.length();

        auto p1 = QPointF(
            pointAtLine.x() - line.dy() * r / d12,
            pointAtLine.y() + line.dx() * r / d12
        );

        auto p2 = QPointF(
            pointAtLine.x() + line.dy() * r / d12,
            pointAtLine.y() - line.dx() * r / d12
        );

        if (left) {
            if (p2AbArea(line.p1(), line.p2(), p1) >= 0) {
                return p1;
            }
            return p2;
        }
        if (p2AbArea(line.p1(), line.p2(), p1) <= 0) {
            return p1;
        }
        return p2;
    }

    QPointF getFootPoint(const QPointF& p, const QPointF& p1, const QPointF& p2) {
        qreal dx = p1.x() - p2.x();
        qreal dy = p1.y() - p2.y();

        qreal dxy = dx * dy;
        qreal dx2 = dx * dx;
        qreal dy2 = dy * dy;

        return {
            (dx2 * p.x() + dy2 * p1.x() + dxy * p.y() - dxy * p1.y()) / (dx2 + dy2),
            (dy2 * p.y() + dx2 * p1.y() + dxy * p.x() - dxy * p1.x()) / (dx2 + dy2)
        };
    }

    qreal dotProduct(const QLineF& beginLine, const QLineF& endLine) {
        return beginLine.dx() * endLine.dx() + beginLine.dy() * endLine.dy();
    }

    QPointF getMinDistancePoint(const QLineF& line, const QPointF& point, qreal& distance, bool limitInLine) {
        QLineF ap(line.p1(), point);

        qreal dotApAb = dotProduct(ap, line);
        if (dotApAb <= 0 && limitInLine) {
            distance = ap.length();
            return line.p1();
        }
        qreal dotAbAb = dotProduct(line, line);
        if (dotApAb >= dotAbAb && limitInLine) {
            distance = QLineF(line.p2(), point).length();
            return line.p2();
        }
        qreal r = dotApAb / dotAbAb;
        QPointF tag(
            line.x1() + line.dx() * r,
            line.y1() + line.dy() * r
        );
        distance = QLineF(tag, point).length();
        return tag;
    }

    QPointF getPointByRatio(qreal ratio, QPointF p1, QPointF p2, bool useP1) {
        if (useP1) {
            return {
                    p1.x() + (p2.x() - p1.x()) * ratio,
                    p1.y() + (p2.y() - p1.y()) * ratio
            };
        } else {
            return {
                    p2.x() - (p2.x() - p1.x()) * ratio,
                    p2.y() - (p2.y() - p1.y()) * ratio
            };
        }
    }
}

namespace BezierUtil {

    qreal projectionRatio(qreal t, int n) {
        if (t == 0 || t == 1) {
            return t;
        }
        qreal top = qPow(1 - t, n);
        qreal bottom = qPow(t, n) + top;
        return top / bottom;
    }

    qreal abcRatio(qreal t, int n) {
        if (t == 0 || t == 1) {
            return t;
        }
        qreal bottom = qPow(t, n) + qPow(1 - t, n);
        qreal top = bottom - 1;
        return qAbs(top / bottom);
    }

    qreal getT(const QPointF &begin, const QPointF &end, const QPointF &control) {
        qreal d1 = QLineF(begin, control).length();
        qreal d2 = QLineF(end, control).length();
        qreal t = d1 / (d1 + d2);
        return t;
    }

    QPointF getRealControlPoint(const QPointF &begin, const QPointF &end, const QPointF &control) {
        qreal t = getT(begin, end, control);
        auto u = projectionRatio(t, 2);
        auto um = 1 - u;
        QPointF C = {
                u * begin.x() + um * end.x(),
                u * begin.y() + um * end.y()
        };
        auto s = abcRatio(t, 2);
        QPointF A = {
                control.x() + (control.x() - C.x()) / s,
                control.y() + (control.y() - C.y()) / s
        };
        return A;
    }
}
