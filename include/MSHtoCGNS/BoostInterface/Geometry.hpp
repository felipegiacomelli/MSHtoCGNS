#ifndef __BOOST_INTERFACE_GEOMETRY_HPP__
#define __BOOST_INTERFACE_GEOMETRY_HPP__

#include <boost/geometry.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

using Point = boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian>;
using Box = boost::geometry::model::box<Point>;
using MultiPoint = boost::geometry::model::multi_point<Point>;
using Segment = boost::geometry::model::segment<Point>;

#endif
