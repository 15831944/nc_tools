/* 
 * Copyright (C) 2013  Nicholas Gill
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * rs274_backplot.cpp
 *
 *  Created on: 2014-12-31
 *      Author: nicholas
 */

#include "rs274_backplot.h"
#include <cmath>
#include <cstring>
#include "Path.h"
#include <osg/Geometry>

void pushBackplot(osg::Geode* geode, const std::vector<cxxcam::path::step>& steps, bool cut) {
    using cxxcam::units::length_mm;
    auto geom = new osg::Geometry();

    auto vertices = new osg::Vec3Array;
    vertices->reserve(steps.size());
    for(auto& step : steps)
    {
        auto& p = step.position;
        vertices->push_back({static_cast<float>(length_mm{p.x}.value()), static_cast<float>(length_mm{p.y}.value()), static_cast<float>(length_mm{p.z}.value())});
    }
    geom->setVertexArray(vertices);

    auto colors = new osg::Vec4Array;
    if(cut)
        colors->push_back({0.0f,1.0f,0.0f,1.0f});
    else
        colors->push_back({1.0f,0.0f,0.0f,1.0f});
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);

    auto normals = new osg::Vec3Array;
    normals->push_back({0.0f,0.0f,1.0f});
    geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,vertices->size()));

    geode->addDrawable(geom);
}

void rs274_backplot::_rapid(const Position& pos)
{
	auto steps = cxxcam::path::expand_linear(convert(program_pos), convert(pos), {}, -1).path;
    pushBackplot(geode, steps, false);
}

void rs274_backplot::_arc(const Position& end, const Position& center, const cxxcam::math::vector_3& plane, int rotation)
{
    using namespace cxxcam::path;
	auto steps = expand_arc(convert(program_pos), convert(end), convert(center), (rotation < 0 ? ArcDirection::Clockwise : ArcDirection::CounterClockwise), plane, std::abs(rotation), {}).path;
    pushBackplot(geode, steps, true);
}

void rs274_backplot::_linear(const Position& pos)
{
	auto steps = cxxcam::path::expand_linear(convert(program_pos), convert(pos), {}, -1).path;
    pushBackplot(geode, steps, true);
}

rs274_backplot::rs274_backplot(osg::Group* parent)
 : rs274_base(), geode(new osg::Geode)
{
    parent->addChild(geode);
}
