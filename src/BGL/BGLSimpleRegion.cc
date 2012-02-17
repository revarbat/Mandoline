//
//  BGLRegion.cc
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/14/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//

#include "BGLCommon.hh"
#include "BGLBounds.hh"
#include "BGLPoint.hh"
#include "BGLLine.hh"
#include "BGLPath.hh"
#include "BGLSimpleRegion.hh"
#include "BGLCompoundRegion.hh"



namespace BGL {


// Compound assignment operators
SimpleRegion& SimpleRegion::operator+=(const Point &rhs) {
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        *it += rhs;
    }
    outerPath += rhs;
    return *this;
}



SimpleRegion& SimpleRegion::operator-=(const Point &rhs) {
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        *it -= rhs;
    }
    outerPath -= rhs;
    return *this;
}



SimpleRegion& SimpleRegion::operator*=(double rhs) {
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        *it *= rhs;
    }
    outerPath *= rhs;
    return *this;
}



SimpleRegion& SimpleRegion::operator*=(const Point &rhs) {
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        *it *= rhs;
    }
    outerPath *= rhs;
    return *this;
}



SimpleRegion& SimpleRegion::operator/=(double rhs) {
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        *it /= rhs;
    }
    outerPath /= rhs;
    return *this;
}



SimpleRegion& SimpleRegion::operator/=(const Point &rhs) {
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        *it /= rhs;
    }
    outerPath /= rhs;
    return *this;
}




int SimpleRegion::size()
{
    return subpaths.size();
}



double SimpleRegion::perimeterLength() const
{
    double length = outerPath.length();
    Paths::const_iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        length += it->length();
    }
    return length;
}



bool SimpleRegion::contains(const Point &pt) const
{
    int count = 0;
    if (outerPath.contains(pt)) {
        count++;
    }
    Paths::const_iterator it1;
    for (it1 = subpaths.begin(); it1 != subpaths.end(); it1++) {
        if (it1->contains(pt)) {
            count++;
        }
    }
    return ((count & 0x1) == 0x1);
}



bool SimpleRegion::intersectsPerimeter(const Line &ln) const
{
    if (outerPath.intersects(ln)) {
        return true;
    }
    Paths::const_iterator it1;
    for (it1 = subpaths.begin(); it1 != subpaths.end(); it1++) {
        if (it1->intersects(ln)) {
            return true;
        }
    }
    return false;
}



bool SimpleRegion::intersects(const Line &ln) const
{
    if (outerPath.intersects(ln)) {
        return true;
    }
    Paths::const_iterator it1;
    for (it1 = subpaths.begin(); it1 != subpaths.end(); it1++) {
        if (it1->intersects(ln)) {
            return true;
        }
        if (it1->contains(ln.startPt) && it1->contains(ln.endPt)) {
            return false;
        }
    }
    if (outerPath.contains(ln.startPt)) {
        return true;
    }
    return false;
}



bool SimpleRegion::intersects(const Path &path) const
{
    if (path.contains(outerPath.startPoint())) {
        return true;
    }
    if (contains(path.startPoint())) {
        return true;
    }
    Paths::const_iterator it1;
    for (it1 = subpaths.begin(); it1 != subpaths.end(); it1++) {
        if (it1->intersects(path)) {
            return true;
        }
    }
    return false;
}



bool SimpleRegion::intersects(const SimpleRegion &reg) const
{
    Paths::const_iterator pit1;
    Paths::const_iterator pit2;
    if (contains(reg.outerPath.startPoint())) {
        return true;
    }
    if (reg.contains(outerPath.startPoint())) {
        return true;
    }
    if (outerPath.intersects(reg.outerPath)) {
        return true;
    }
    for (pit1 = subpaths.begin(); pit1 != subpaths.end(); pit1++) {
        if (pit1->intersects(reg.outerPath)) {
            return true;
        }
    }
    for (pit2 = reg.subpaths.begin(); pit2 != reg.subpaths.end(); pit2++) {
        if (pit2->intersects(outerPath)) {
            return true;
        }
    }
    for (pit1 = subpaths.begin(); pit1 != subpaths.end(); pit1++) {
        for (pit2 = reg.subpaths.begin(); pit2 != reg.subpaths.end(); pit2++) {
            if (pit1->intersects(*pit2)) {
                return true;
            }
        }
    }
    return false;
}



string SimpleRegion::svgPathWithOffset(double dx, double dy)
{
    string out;
    out.append(outerPath.svgPathWithOffset(dx, dy));
    Paths::iterator pit = subpaths.begin();
    for ( ; pit != subpaths.end(); pit++) {
        out.append(" ");
        out.append(pit->svgPathWithOffset(dx, dy));
    }
    return out;
}



ostream &SimpleRegion::svgPathDataWithOffset(ostream& os, double dx, double dy) const
{
    outerPath.svgPathDataWithOffset(os, dx, dy);
    Paths::const_iterator pit;
    for (pit = subpaths.begin(); pit != subpaths.end(); pit++) {
        os << " ";
        pit->svgPathDataWithOffset(os, dx, dy);
    }
    return os;
}


ostream &SimpleRegion::svgPathWithOffset(ostream& os, double dx, double dy) const
{
    os << "<path fill=\"inherit\" d=\"";
    svgPathDataWithOffset(os, dx, dy);
    os << "\" />" << endl;
    return os;
}





void SimpleRegion::setWidth(double extWidth)
{
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        it->setWidth(extWidth);
    }
    outerPath.setWidth(extWidth);
}



void SimpleRegion::simplify(double minErr)
{
    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        it->simplify(minErr);
    }
    outerPath.simplify(minErr);
}



SimpleRegions &SimpleRegion::assembleSimpleRegionsFrom(Paths &paths, SimpleRegions &outRegs)
{
    Paths::iterator it1;
    Paths::iterator it2;

    for (it1 = paths.begin(); it1 != paths.end(); it1++) {
        it1->flags = 0;
    }
    int count1, count2;
    for (count1 = 0, it1 = paths.begin(); it1 != paths.end(); count1++, it1++) {
        for (count2 = 0, it2 = paths.begin(); it2 != paths.end(); count2++, it2++) {
            if (it1 != it2) {
                if (it2->contains(it1->startPoint())) {
                    it1->flags++;
                }
            }
        }
    }
    for (it1 = paths.begin(); it1 != paths.end(); it1++) {
        if ((it1->flags & 0x1) == 0) {
            // An even contained count means outerpath.
            outRegs.push_back(SimpleRegion(*it1));
        }
    }
    SimpleRegions::iterator rit;
    for (it1 = paths.begin(); it1 != paths.end(); it1++) {
        if ((it1->flags & 0x1) == 1) {
            // Odd contained count means innerpath.
            for (rit = outRegs.begin(); rit != outRegs.end(); rit++) {
                // A SimpleRegion can be nested inside a hole in another
                // SimpleRegion.  Make sure we're adding the new hole to
                // the right SimpleRegion.
                if ((rit->outerPath.flags | 0x1) == (it1->flags | 0x1)) {
                    // Same level of containedness.
                    if (rit->outerPath.contains(it1->startPoint())) {
                        rit->subpaths.push_back(*it1);
                    }
                }
            }
        }
    }
    return outRegs;
}



SimpleRegions &SimpleRegion::assembleSimpleRegionsFrom(const Paths &outerPaths, const Paths &innerPaths, SimpleRegions &outRegs)
{
    Paths tempPaths(outerPaths);

    Paths::const_iterator it1;
    for (it1 = innerPaths.begin(); it1 != innerPaths.end(); it1++) {
        tempPaths.push_back(*it1);
    }
    return assembleSimpleRegionsFrom(tempPaths, outRegs);
}




SimpleRegions &SimpleRegion::differenceOf(SimpleRegion &reg, Path &path, SimpleRegions &outRegs)
{
    Path currPath(path);
    Paths innerPaths;
    Paths::iterator pit;
    for (pit = reg.subpaths.begin(); pit != reg.subpaths.end(); pit++) {
        Paths tempPaths;
        Path::unionOf(currPath, *pit, tempPaths);
        if (tempPaths.size() == 1) {
            currPath = tempPaths.front();
        } else {
            innerPaths.push_back(*pit);
        }
    }

    Paths outerPaths;
    Path::differenceOf(reg.outerPath, currPath, outerPaths);

    SimpleRegions newRegs;
    SimpleRegions::iterator rit;
    assembleSimpleRegionsFrom(outerPaths, innerPaths, newRegs);

    for (rit = newRegs.begin(); rit != newRegs.end(); rit++) {
        outRegs.push_back(*rit);
    }

    return outRegs;
}



SimpleRegions &SimpleRegion::unionOf(SimpleRegion &r1, SimpleRegion &r2, SimpleRegions &outRegs)
{
    Paths outerPaths;
    Path::unionOf(r1.outerPath, r2.outerPath, outerPaths);

    Paths::iterator it1;
    Paths::iterator it2;

    Paths newInnerPaths;
    // Calculate intersection of interior holes.
    for (it1 = r1.subpaths.begin(); it1 != r1.subpaths.end(); it1++) {
        for (it2 = r2.subpaths.begin(); it2 != r2.subpaths.end(); it2++) {
            Path::intersectionOf(*it1, *it2, newInnerPaths);
        }
    }
    // Calculate portions of holes outside the other region.
    for (it1 = r1.subpaths.begin(); it1 != r1.subpaths.end(); it1++) {
        Path::differenceOf(*it1, r2.outerPath, newInnerPaths);
    }
    for (it2 = r2.subpaths.begin(); it2 != r2.subpaths.end(); it2++) {
        Path::differenceOf(*it2, r1.outerPath, newInnerPaths);
    }

    Paths innerPaths;
    Path::unionOf(newInnerPaths, innerPaths);
    assembleSimpleRegionsFrom(outerPaths, innerPaths, outRegs);

    return outRegs;
}



SimpleRegions &SimpleRegion::differenceOf(SimpleRegion &r1, SimpleRegion &r2, SimpleRegions &outRegs)
{
    Paths outerPaths;
    Path::differenceOf(r1.outerPath, r2.outerPath, outerPaths);

    Paths::iterator it1;
    Paths::iterator it2;

    Paths newInnerPaths;
    for (it2 = r2.subpaths.begin(); it2 != r2.subpaths.end(); it2++) {
        Path::intersectionOf(*it2, r1.outerPath, newInnerPaths);
    }
    for (it2 = r1.subpaths.begin(); it2 != r1.subpaths.end(); it2++) {
        Paths tempPaths;
        for (it1 = newInnerPaths.begin(); it1 != newInnerPaths.end(); it1++) {
            Path::differenceOf(*it1, *it2, tempPaths);
        }
        newInnerPaths = tempPaths;
    }
    for (it2 = r1.subpaths.begin(); it2 != r1.subpaths.end(); it2++) {
        Paths tempPaths;
        for (it1 = outerPaths.begin(); it1 != outerPaths.end(); it1++) {
            Path::differenceOf(*it1, *it2, tempPaths);
        }
        outerPaths = tempPaths;
    }

    Paths innerPaths;
    Path::unionOf(newInnerPaths, innerPaths);
    assembleSimpleRegionsFrom(outerPaths, innerPaths, outRegs);

    return outRegs;
}



/*
 * Intersection:
 *   new.outerPaths = r1.outerPath.intersect(r2.outerPath)
 *   diff r1.outerPath from each r2.innerpath
 *   diff r2.outerPath from each r1.innerpath
 *   unionize all innerPaths
 *   foreach p1 innerPaths {
 *     if (p1 intersects a path in new.outerPaths) {
 *       diff p1 from new.outerPaths
 *     } else {
 *       add p1 to new.innerPaths
 *     }
 *   }
 *   sort new innerpaths into SimpleRegions with new outerPaths
 */
SimpleRegions &SimpleRegion::intersectionOf(SimpleRegion &r1, SimpleRegion &r2, SimpleRegions &outRegs)
{
    Paths outerPaths;
    Paths::iterator pit;
    Path::intersectionOf(r1.outerPath, r2.outerPath, outerPaths);

    SimpleRegions newRegs;
    SimpleRegions::iterator rit;
    for (pit = outerPaths.begin(); pit != outerPaths.end(); pit++) {
        newRegs.push_back(SimpleRegion(*pit));
    }

    for (pit = r1.subpaths.begin(); pit != r1.subpaths.end(); pit++) {
        SimpleRegions tempRegs;
        for (rit = newRegs.begin(); rit != newRegs.end(); rit++) {
            differenceOf(*rit, *pit, tempRegs);
        }
        newRegs = tempRegs;
    }
    for (pit = r2.subpaths.begin(); pit != r2.subpaths.end(); pit++) {
        SimpleRegions tempRegs;
        for (rit = newRegs.begin(); rit != newRegs.end(); rit++) {
            differenceOf(*rit, *pit, tempRegs);
        }
        newRegs = tempRegs;
    }

    for (rit = newRegs.begin(); rit != newRegs.end(); rit++) {
        outRegs.push_back(*rit);
    }
    return outRegs;
}



// Inset this region by the given offset. You can outset with a negative offset.
SimpleRegions &SimpleRegion::inset(double offsetby, SimpleRegions& outRegs)
{
    Paths outerPaths;
    outerPath.inset(offsetby, outerPaths);

    Paths innerPaths;
    Paths::iterator it1;
    for (it1 = subpaths.begin(); it1 != subpaths.end(); it1++) {
        it1->inset(-offsetby, innerPaths);
    }

    Paths innerPaths2;
    Path::unionOf(innerPaths, innerPaths2);

    Paths newPaths;
    Path::differenceOf(outerPaths, innerPaths2, newPaths);

    assembleSimpleRegionsFrom(newPaths, outRegs);
    return outRegs;
}



// Clip the given line to just the segments that are inside the region.
Lines &SimpleRegion::containedSegmentsOfLine(Line &line, Lines &outSegs)
{
    Path newpath;
    newpath.segments.push_back(line);
    newpath.splitSegmentsAtIntersectionsWithPath(outerPath);

    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        newpath.splitSegmentsAtIntersectionsWithPath(*it);
    }

    Lines::iterator lit;
    for (lit = newpath.segments.begin(); lit != newpath.segments.end(); lit++) {
        if (contains((lit->startPt + lit->endPt)/2.0)) {
            // Now inside
            outSegs.push_back(*lit);
        }
    }
    return outSegs;
}



// Clip the given path to just the subpaths that are inside the region.
Paths &SimpleRegion::containedSubpathsOfPath(const Path &path, Paths &outPaths)
{
    Path newpath(path);
    newpath.splitSegmentsAtIntersectionsWithPath(outerPath);

    Paths::iterator it;
    for (it = subpaths.begin(); it != subpaths.end(); it++) {
        newpath.splitSegmentsAtIntersectionsWithPath(*it);
    }

    Lines::iterator lit;
    bool wasOut = true;
    Path tempPath;
    for (lit = newpath.segments.begin(); lit != newpath.segments.end(); lit++) {
        Point midPt = (lit->startPt + lit->endPt) / 2.0;
        if (this->contains(midPt)) {
            // Now inside
            tempPath.segments.push_back(*lit);
            wasOut = false;
        } else {
            // Now outside
            if (!wasOut) {
                outPaths.push_back(tempPath);
                tempPath.segments.clear();
                tempPath.flags = INSIDE;
            }
            wasOut = true;
        }
    }
    if (tempPath.size() > 1) {
        outPaths.push_back(tempPath);
    }
    return outPaths;
}



// If p1 and p2 are both inside this simpleregion, find the shortest route
//   from p1 to p2 within this simpleregion.
// If p1 and p2 are both outside this simpleregion, find the shortest route
//   from p1 to p2 around the outside of this simpleregion.
// If p1 is inside this simpleregion, and p2 is outside, find the shortest
//   inside route from p1 to the closest perimeter point to p2, then go
//   straight to p2.
// If p1 is outside this simpleregion, and p2 is inside, go straight from p1
//   to the closest perimeter point to p1, then find the shortest inside route
//   from there to p2.

// BUG: Sadly, this implementation is incomplete.
Path &SimpleRegion::pathAround(Point p1, Point p2, Path &outPath) const
{
    bool inside1 = contains(p1);
    bool inside2 = contains(p2);
    bool doreverse = false;
    bool doinverse = false;
    if (inside1 && !inside2) {
        doreverse = true;
        Point tmp = p1;
        p1 = p2;
        p2 = tmp;
    }
    if (inside1 != inside2) {
        Point perimPt = outerPath.closestPointTo(p1, NULL);
        outPath.segments.push_back(Line(p1,perimPt));
        p1 = perimPt;
        inside1 = true;
        inside2 = true;
    }
    if (!inside1) {
        doinverse = true;
    }

    double lineAng = p1.angleToPoint(p2);
    double minAng = 0.0;
    double maxAng = 0.0;
    Point minPt;
    Point maxPt;

    // TODO: FIX THIS for insideness
    Paths::const_iterator itera;
    Lines::const_iterator lit;
    for (lit = outerPath.begin(); lit != outerPath.end(); lit++) {
        double ptAng = lineAng - p1.angleToPoint(lit->endPt);
        if (ptAng <= -M_PI) {
            ptAng += 2.0 * M_PI;
        }
        if (ptAng > M_PI) {
            ptAng -= 2.0 * M_PI;
        }
        if (ptAng < minAng) {
            minAng = ptAng;
            minPt = lit->endPt;
        }
        if (ptAng > maxAng) {
            maxAng = ptAng;
            maxPt = lit->endPt;
        }
    }
    if (minAng != 0.0 && maxAng != 0.0) {
        // TODO: umm, do something
    }

    bool isectFound = false;
    for (itera = subpaths.begin(); itera != subpaths.end(); itera++) {

        for (lit = itera->begin(); lit != itera->end(); lit++) {
            double ptAng = lineAng - p1.angleToPoint(lit->endPt);
            if (ptAng <= -M_PI) {
                ptAng += 2.0 * M_PI;
            }
            if (ptAng > M_PI) {
                ptAng -= 2.0 * M_PI;
            }
            if (ptAng < minAng) {
                minAng = ptAng;
                minPt = lit->endPt;
            }
            if (ptAng > maxAng) {
                maxAng = ptAng;
                maxPt = lit->endPt;
            }
        }
        if (minAng != 0.0 && maxAng != 0.0) {
            isectFound = true;
        }
    }
    // TODO: make recursive.

    if (doreverse) {
        outPath.reverse();
    }
    return outPath;
}



Paths &SimpleRegion::joinSubPathsInside(double maxDist, double extWidth, const Paths &inPaths, Paths &outPaths)
{
    double minDistFound;
    bool reversed1;
    bool reversed2;
    Path tempPath;
    Paths tempPaths;
    Paths::iterator pit, foundIter;
    Paths::const_iterator cpit;

    // Calculate a masking region just barely larger than the containing region.
    SimpleRegions maskRegs;
    inset(-extWidth, maskRegs);
    CompoundRegion maskReg(maskRegs);

    // Copy paths so we can mutate the list.
    for (cpit = inPaths.begin(); cpit != inPaths.end(); cpit++) {
        tempPaths.push_back(*cpit);
    }

    while (tempPaths.size() > 0) {
        // Grab the first path arbitrarily. This is our current path.
        tempPath = tempPaths.front();
        tempPaths.pop_front();

        bool foundOne;
        do {

            // Find closest path to the current one.
            foundOne = false;
            minDistFound = 99e99;
            foundIter = tempPaths.end();
            for (pit = tempPaths.begin(); pit != tempPaths.end(); pit++) {
                double dist1, dist2, dist;

                // Check distance from endpoint of current path to each other path.
                dist1 = tempPath.endPoint().distanceFrom(pit->startPoint());
                dist2 = tempPath.endPoint().distanceFrom(pit->endPoint());
                dist = min(dist1,dist2);
                if (dist <= maxDist && dist < minDistFound) {
                    minDistFound = dist;
                    reversed1 = false;
                    reversed2 = (dist2 < dist1);
                    foundIter = pit;
                }

                // Check distance from startpoint of current path to each other path.
                dist1 = tempPath.startPoint().distanceFrom(pit->startPoint());
                dist2 = tempPath.startPoint().distanceFrom(pit->endPoint());
                dist = min(dist1,dist2);
                if (dist <= maxDist && dist < minDistFound) {
                    minDistFound = dist;
                    reversed1 = true;
                    reversed2 = (dist2 < dist1);
                    foundIter = pit;
                }
            }
            
            if (foundIter != tempPaths.end()) {
                // We found a target path that is close enough
                // to the current path.

                // Figure out the start point of the joining line
                // between the two paths.
                double extWidth;
                Point tempPt1;
                if (reversed1) {
                    tempPt1 = tempPath.startPoint();
                    extWidth = tempPath.segments.front().extrusionWidth;
                } else {
                    tempPt1 = tempPath.endPoint();
                    extWidth = tempPath.segments.back().extrusionWidth;
                }

                // Figure out the end point of the joining line
                // between the two paths.
                Point tempPt2;
                if (reversed2) {
                    tempPt2 = foundIter->endPoint();
                } else {
                    tempPt2 = foundIter->startPoint();
                }

                // Construct the joining line.
                Line tempLn(tempPt1, tempPt2);
                tempLn.extrusionWidth = extWidth;

                // Construct a test line for intersection checks.
                // It's about 2% shorter than the joining line.
                Point testPt1 = ((tempPt1 * 99.0) + tempPt2) / 100.0;
                Point testPt2 = ((tempPt2 * 99.0) + tempPt1) / 100.0;
                Line testLn(testPt1, testPt2);

                // Test to see if the joining line would cross another path.
                bool crossFound = false;
                for (pit = tempPaths.begin(); pit != tempPaths.end(); pit++) {
                    if (pit->intersects(testLn)) {
                        crossFound = true;
                        break;
                    }
                }

                // If the joining line doesn't cross the perimeter of the
                // masking region, and it doesn't cross another paths,
                // we join the paths with the line.
                if (!maskReg.intersectsPerimeter(tempLn) &&
                    !tempPath.intersects(testLn) &&
                    !crossFound
                ) {
                    foundOne = true;
                    tempPath.attach(tempLn);
                    tempPath.attach(*foundIter);

                    // Remove target path from list.
                    foundIter = tempPaths.erase(foundIter);
                }
            }

        } while (foundOne);  // Keep joining until we can't find a close path.
        outPaths.push_back(tempPath);
    }
    return outPaths;
}



Paths &SimpleRegion::infillPathsForRegionWithDensity(double angle, InfillStyle style, double density, double extrusionWidth, CompoundRegion &solidMask, Paths &outPaths)
{
    Paths::iterator pit;
    Paths tempPaths;

    Bounds bounds = outerPath.bounds();
    if (bounds.minX == Bounds::NONE) {
        return outPaths;
    }
    if (density <= 0.001) {
        return outPaths;
    }
    
    // Figure out the zig-zag cell spacing for the given infill density.



    double spacing =  extrusionWidth;
    if (density < 0.99) {
        switch (style) {
            case INFILL_NONE:
                // Doesn't matter much what our spacing is when not infilling.
                // Just use the same spacing as Random Lines.
            case INFILL_LINES:
                // Straight line across each cell.
                // Not exact, but roughly correct on average.
                // density = spacing * extrusionWidth / (spacing*spacing)
                // density = extrusionWidth / spacing
                // spacing * density = extrusionWidth
                // spacing = extrusionWidth / density
                spacing = extrusionWidth / density;
                break;
            case INFILL_RECTANGULAR:
                // Diagonal slice across each cell.
                // density = sqrt(2.0) * spacing * extrusionWidth / (spacing*spacing)
                // density = sqrt(2.0) * extrusionWidth / spacing
                // spacing * density = sqrt(2.0) * extrusionWidth
                // spacing = sqrt(2.0) * extrusionWidth / density
                spacing = sqrt(2.0) * extrusionWidth / density;
                break;
            case INFILL_HEXAGONAL:
                // Average of three cell infill shapes.
                //    /     \     |
                //   |       |    |
                // Angled bits are at 60 degrees from vertical.
                // Cell is taller than it is wide.
                // spacingy = spacingx/cos(30deg)
                // density = (1.5 + 1.5 + 1)/3 * spacingy * extrusionWidth / (spacingx * spacingy)
                // density = 4/3 * spacingy * extrusionWidth / (spacingx * spacingy)
                // density = 4/3 * extrusionWidth / spacingx
                // spacingx * density = 4/3 * extrusionWidth
                // spacingx = 4/3 * extrusionWidth / density
                // spacingx = extrusionWidth * (4/3) / density
                spacing = extrusionWidth * (4.0/3.0) / density;
                break;
        }
    }

    // This next line limits our density options, but it ensures
    // that solid infill won't bunch up on spacing boundries.
    spacing = ceil(spacing/extrusionWidth)*extrusionWidth;


    double maxd = max(max(fabs(bounds.minX),fabs(bounds.maxX)),
                      max(fabs(bounds.minY),fabs(bounds.maxY)));
    maxd *= 2.0;

    int cells = floor(maxd/spacing+1);
    for (int xcell = -cells; xcell <= cells; xcell++) {
        Path path;
        Paths allPaths;
        double fillx = (xcell+0.5) * spacing;
        
        // Check if we need solid infill lines.
        // If any lines in the width of the cell cross the solid infill mask,
        // then they are included in the infill instead of the zia-zag fill.
        bool hasSolid = false;
        for (double subx = fillx - spacing * 0.5; subx < fillx + spacing * 0.499; subx += extrusionWidth) {
            Line tempLn(Point(subx,-maxd),Point(subx,maxd));
            tempLn.rotate(angle);
            if (solidMask.intersects(tempLn)) {
                Path tmpPath(tempLn);
                allPaths.push_back(tmpPath);
                hasSolid = true;
            }
        }

        // If no solid infill was done in this cell width,
        // then we fill with sparse zig-zag infill instead.
        if (!hasSolid) {
            if (style == INFILL_LINES) {
                double xoff1 = (((random() & 1023)/1024.0)-0.5) * spacing;
                double xoff2 = (((random() & 1023)/1024.0)-0.5) * spacing;
                Line tempLn2(Point(fillx+xoff1,-maxd),Point(fillx+xoff2,maxd));
                tempLn2.rotate(angle);
                path.segments.push_back(tempLn2);
            }
            if (style == INFILL_RECTANGULAR) {
                for (int ycell = -cells; ycell < cells; ycell++) {
                    double filly1 = ycell * spacing;
                    double filly2 = (ycell+1) * spacing;
                    bool alternate = (((xcell+ycell) & 0x1) == 0);
                    double zig = 0.0;
                    if (density < 0.99) {
                        zig = 0.5*spacing;
                        if (alternate) {
                            zig = -zig;
                        }
                    }
                    path.segments.push_back(Line(Point(fillx+zig,filly1),Point(fillx-zig,filly2)).rotate(angle));
                }
                allPaths.push_back(path);
            }
            if (style == INFILL_HEXAGONAL) {
                for (int ycell = -cells; ycell < cells; ycell++) {
                    double yscale = 1.0/cos(30.0 * M_PI / 180.0);
                    double filly1 = yscale * ycell * spacing;
                    double filly2 = yscale * (ycell+0.5) * spacing;
                    double filly3 = yscale * (ycell+1.0) * spacing;
                    bool alternate = ((xcell & 0x1) == 0);
                    double zig1 = 0.0;
                    double zig2 = 0.0;
                    double zig3 = 0.0;
                    if (density < 0.99) {
                        switch ((ycell+1000000) % 3) {
                          case 0:
                            zig1 =  0.5 * (spacing-extrusionWidth); /*   /   */
                            zig2 = -zig1;                           /*  |    */
                            zig3 =  zig2;
                            break;
                          case 1:
                            zig1 = -0.5 * (spacing-extrusionWidth); /*  |    */
                            zig2 =  zig1;                           /*   \   */
                            zig3 = -zig2;
                            break;
                          case 2:
                          default:
                            zig1 =  0.5 * (spacing-extrusionWidth); /*    |  */
                            zig2 = zig1;                            /*    |  */
                            zig3 = zig2;
                            break;
                        }
                        if (alternate) {
                            zig1 = -zig1;
                            zig2 = -zig2;
                            zig3 = -zig3;
                        }
                    }
                    path.segments.push_back(Line(Point(fillx+zig1,filly1),Point(fillx+zig2,filly2)).rotate(angle));
                    path.segments.push_back(Line(Point(fillx+zig2,filly2),Point(fillx+zig3,filly3)).rotate(angle));
                }
                allPaths.push_back(path);
            }
        }

        // Clip all the paths to the infill mask.
        for (pit = allPaths.begin(); pit != allPaths.end(); pit++) {
            containedSubpathsOfPath(*pit, tempPaths);
        }
    }

    // Set the extrusionWidth of all the subpaths.
    for (pit = tempPaths.begin(); pit != tempPaths.end(); pit++) {
        pit->setWidth(extrusionWidth);
        //outPaths.push_back(*pit);
    }

    // Join together nearby subpaths with short lines.
    joinSubPathsInside(spacing*2.1, extrusionWidth, tempPaths, outPaths);

    return outPaths;
}


} // namespace BGL
// vim: set ts=4 sw=4 nowrap expandtab: settings

