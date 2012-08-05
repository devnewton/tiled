#include "polygontools.h"

namespace im
{
    namespace bci
    {
        namespace polygon_tools
        {
            struct counter_clockwise_comparator
            {
                QPointF center_;
                counter_clockwise_comparator( const QPolygonF& polygon)
                    : center_(polygon.boundingRect().center())
                {
                }

                bool operator()(const QPointF& a, const QPointF& b)
                {
                    if (a.x() >= 0 && b.x() < 0)
                        return true;
                    if (a.x() == 0 && b.x() == 0)
                        return a.y() > b.y();

                    // compute the cross product of vectors (center -> a) x (center -> b)
                    qreal det = (a.x()-center_.x()) * (b.y()-center_.y()) - (b.x() - center_.x()) * (a.y() - center_.y());
                    if (det < 0)
                        return true;
                    if (det > 0)
                        return false;

                    // points a and b are on the same line from the center
                    // check which point is closer to the center
                    qreal d1 = (a.x()-center_.x()) * (a.x()-center_.x()) + (a.y()-center_.y()) * (a.y()-center_.y());
                    qreal d2 = (b.x()-center_.x()) * (b.x()-center_.x()) + (b.y()-center_.y()) * (b.y()-center_.y());
                    return d1 > d2;
                }
            };


            void sort_counter_clockwise(QPolygonF& polygon)
            {
                qSort(polygon.begin(), polygon.end(), counter_clockwise_comparator(polygon));

            }

            QPolygonF convex_hull(const QPolygonF& input)
            {
                //todo use boost geometry to compute convex hull
                QPolygonF result = input;
                sort_counter_clockwise(result);
                return result;
            }
        }
    }
}
