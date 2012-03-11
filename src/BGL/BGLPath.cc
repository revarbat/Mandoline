//
//  BGLPath.cc
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/13/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//

#include "config.h"
#include <iostream>
#include <iomanip>

#include "BGLPath.hh"

using namespace std;
using namespace BGL;

namespace BGL {


Path::Path(int cnt, const Point* pts)
{
    Point prev = pts[0];
    flags = 0;
    for (int i = 1; i < cnt; i++) {
        Line ln(prev,pts[i]);
        attach(ln);
        prev = pts[i];
    }
}



// Comparison operators
bool Path::operator==(const Path &rhs) const
{
    if (size() != rhs.size()) {
        return false;
    }
    Lines::const_iterator itera = segments.begin();
    Lines::const_iterator iterb = rhs.segments.begin();
    while(itera != segments.end() && iterb != rhs.segments.end()) {
        if (*itera != *iterb) {
            return false;
        }
        itera++, iterb++;
    }
    return true;
}



// Compound assignment operators
Path& Path::operator+=(const Point &rhs) {
    Lines::iterator it;
    for (it = segments.begin(); it != segments.end(); it++) {
        *it += rhs;
    }
    return *this;
}



Path& Path::operator-=(const Point &rhs) {
    Lines::iterator it;
    for (it = segments.begin(); it != segments.end(); it++) {
        *it -= rhs;
    }
    return *this;
}



Path& Path::operator*=(double rhs) {
    Lines::iterator it;
    for (it = segments.begin(); it != segments.end(); it++) {
        *it *= rhs;
    }
    return *this;
}



Path& Path::operator*=(const Point &rhs) {
    Lines::iterator it;
    for (it = segments.begin(); it != segments.end(); it++) {
        *it *= rhs;
    }
    return *this;
}



Path& Path::operator/=(double rhs) {
    Lines::iterator it;
    for (it = segments.begin(); it != segments.end(); it++) {
        *it /= rhs;
    }
    return *this;
}



Path& Path::operator/=(const Point &rhs) {
    Lines::iterator it;
    for (it = segments.begin(); it != segments.end(); it++) {
        *it /= rhs;
    }
    return *this;
}



void Path::quantize(double quanta) {
    Lines::iterator it;
    Lines::iterator previt = segments.end();
    Point delta = endPoint() - startPoint();
    if (fabs(delta.x) <= quanta && fabs(delta.y) <= quanta) {
        // Path is close enough to closed.
        previt--;
    }
    for (it = segments.begin(); it != segments.end(); it++) {
        it->startPt.quantize(quanta);
        if (previt != segments.end()) {
            previt->endPt = it->startPt;
        }
        previt = it;
    }
}



void Path::quantize() {
    Lines::iterator it;
    Lines::iterator previt = segments.end();
    Point delta = endPoint() - startPoint();
    if (fabs(delta.x) <= CLOSEENOUGH && fabs(delta.y) <= CLOSEENOUGH) {
        // Path is close enough to closed.
        previt--;
    }
    for (it = segments.begin(); it != segments.end(); it++) {
        it->startPt.quantize();
        if (previt != segments.end()) {
            previt->endPt = it->startPt;
        }
        previt = it;
    }
}



double Path::length() const
{
    double totlen = 0.0;
    Lines::const_iterator itera = segments.begin();
    for(; itera != segments.end(); itera++) {
        totlen += itera->length();
    }
    return totlen;
}



double Path::windingArea() const
{
    double totarea = 0.0;
    Lines::const_iterator itera = segments.begin();
    for(; itera != segments.end(); itera++) {
        totarea += itera->startPt.x * itera->endPt.y;
        totarea -= itera->endPt.x * itera->startPt.y;
    }
    return (totarea/2.0);
}



bool Path::isClockwise() const
{
    return (windingArea() > 0);
}



double Path::area() const
{
    return fabs(windingArea());
}



void Path::setTemperature(double val)
{
    Lines::iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        itera->temperature = val;
    }
}



void Path::setWidth(double val)
{
    Lines::iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        itera->extrusionWidth = val;
    }
}



Bounds Path::bounds() const
{
    Bounds bnds;
    Lines::const_iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        bnds.expand(itera->startPt);
        bnds.expand(itera->endPt);
    }
    return bnds;
}



bool Path::couldAttach(const Line& ln) const
{
    if (size() < 1) return true;
    if (hasEndPoint(ln.startPt)) return true;
    if (hasEndPoint(ln.endPt)) return true;
    return false;
}



bool Path::couldAttach(const Path& path) const
{
    if (size() < 1) return true;
    if (hasEndPoint(path.startPoint())) return true;
    if (hasEndPoint(path.endPoint())) return true;
    return false;
}



bool Path::attach(const Line& ln)
{
    if (size() <= 0) {
        segments.push_back(ln);
        return true;
    }
    if (endPoint() == ln.startPt) {
        segments.push_back(ln);
        return true;
    }
    if (startPoint() == ln.endPt) {
        segments.push_front(ln);
        return true;
    }
    if (endPoint() == ln.endPt) {
        Line lnrev(ln);
        lnrev.reverse();
        segments.push_back(lnrev);
        return true;
    }
    if (startPoint() == ln.startPt) {
        Line lnrev(ln);
        lnrev.reverse();
        segments.push_front(lnrev);
        return true;
    }
    return false;
}



bool Path::attach(const Path& path)
{
    if (hasEndPoint(path.startPoint())) {
        Lines::const_iterator itera = path.segments.begin();
        for(; itera != path.segments.end(); itera++) {
            attach(*itera);
        }
        return true;
    }
    if (hasEndPoint(path.endPoint())) {
        Lines::const_reverse_iterator itera = path.segments.rbegin();
        for(; itera != path.segments.rend(); itera++) {
            attach(*itera);
        }
        return true;
    }
    return false;
}



Path& Path::rotate(double angle) {
    Lines::iterator it;
    for (it = segments.begin(); it != segments.end(); it++) {
        it->rotate(angle);
    }
    return *this;
}



string Path::svgPathWithOffset(double dx, double dy) const
{
    char buf[80];
    string out;
    if (size() == 0) {
        return out;
    }
    Line prev;
    Point start;
    double mult = 1.0;
    Lines::const_iterator itera = segments.begin();
    bool isfirst = true;
    for (; itera != segments.end(); itera++) {
        if (!isfirst) {
            out.append(" ");
        }
        if (isfirst || prev.endPt != itera->startPt) {
            start = itera->startPt;
            snprintf(buf,sizeof(buf),"M%5.3f,%5.3f ",
                (itera->startPt.x+dx)*mult,
                (itera->startPt.y+dy)*mult);
            out.append(buf);
            isfirst = false;
        }
        if (itera->endPt == start) {
            snprintf(buf,sizeof(buf),"Z");
            isfirst = true;
        } else {
            snprintf(buf,sizeof(buf),"L%5.3f,%5.3f",
                (itera->endPt.x+dx)*mult,
                (itera->endPt.y+dy)*mult);
        }
        out.append(buf);
        prev = *itera;
    }
    return out;
}




ostream &Path::svgPathDataWithOffset(ostream& os, double dx, double dy) const
{
    if (size() == 0) {
        return os;
    }
    os.setf(ios::fixed);
    os.precision(3);
    double mult = 1.0;
    Line prev;
    Point start;
    Lines::const_iterator itera = segments.begin();
    bool isfirst = true;
    for (; itera != segments.end(); itera++) {
        if (!isfirst) {
            os << endl << "    ";
        }
        if (isfirst || prev.endPt != itera->startPt) {
            start = itera->startPt;
            os << "M" << setw(8) << ((itera->startPt.x+dx)*mult);
            os << "," << setw(8) << ((itera->startPt.y+dy)*mult);
            os << endl << "    ";
            isfirst = false;
        }
        if (itera->endPt == start) {
            os << "Z";
            isfirst = true;
        } else {
            os << "L" << setw(8) << ((itera->endPt.x+dx)*mult);
            os << "," << setw(8) << ((itera->endPt.y+dy)*mult);
        }
        prev = *itera;
    }
    return os;
}



ostream &Path::svgPathWithOffset(ostream& os, double dx, double dy) const
{
    os << "<path d=\"";
    svgPathDataWithOffset(os, dx, dy);
    os << "\" />" << endl;
    return os;
}



bool Path::intersects(const Line &ln) const
{
    Lines::const_iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        Intersection isect = itera->intersectionWithSegment(ln);
        if (isect.type != NONE) {
            return true;
        }
    }
    return false;
}



bool Path::intersects(const Path &path) const
{
    Lines::const_iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        Lines::const_iterator iterb = path.segments.begin();
        for (; iterb != path.segments.end(); iterb++) {
            Intersection isect = itera->intersectionWithSegment(*iterb);
            if (isect.type != NONE) {
                return true;
            }
        }
    }
    return false;
}



Intersections &Path::intersectionsWith(const Line &ln, Intersections &outISects) const
{
    int segnum = 0;
    bool isclosed = isClosed();
    Lines::const_iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        Intersection isect = itera->intersectionWithSegment(ln);
        // Ignore point intersections with the startpoint of a segment.
        // It should have already been caught as the endpoint of the
        //  previous segment.
        if (isect.type != NONE) {
            if (isect.type != POINT ||
                isect.p1 != itera->startPt ||
                (segnum == 0 && !isclosed)
                ) {
                isect.segment = segnum;
                outISects.push_back(isect);
            }
        }
        segnum++;
    }
    return outISects;
}



bool Path::hasEdgeWithPoint(const Point &pt, Lines::const_iterator &outSeg) const
{
    Lines::const_iterator itera;
    for (itera = segments.begin(); itera != segments.end(); itera++) {
        if (itera->contains(pt)) {
            outSeg = itera;
            return true;
        }
    }
    return false;
}



bool Path::contains(const Point &pt) const
{
    if (!isClosed()) {
        return false;
    }
    Line longLine(pt,Point(1.0e9,pt.y));
    int icount = 0;
    Lines::const_iterator itera;
    for (itera = segments.begin(); itera != segments.end(); itera++) {
        Line testLine(itera->startPt, itera->endPt);
        if (fabs(testLine.startPt.y-pt.y) <= EPSILON) {
            testLine.startPt.y += 2.0*EPSILON;
        }
        if (fabs(testLine.endPt.y-pt.y) <= EPSILON) {
            testLine.endPt.y += 2.0*EPSILON;
        }
        Intersection isect = testLine.intersectionWithSegment(longLine);
        if (isect.type != NONE) {
            icount++;
        }
    }
    return ((icount & 0x1) != 0);
}



Point Path::closestPointTo(const Point &pt, int *outSeg) const
{
    double minDist = 999e99;
    Point minPt;
    Lines::const_iterator seg;
    int segNum = 0;
    for (seg = segments.begin(); seg != segments.end(); seg++) {
        Point testPt = seg->closestSegmentPointTo(pt);
        double testDist = pt.distanceFrom(testPt);
        if (testDist < minDist) {
            minDist = testDist;
            minPt = testPt;
            if (outSeg) {
                *outSeg = segNum;
            }
        }
        segNum++;
    }
    return minPt;
}



void Path::reverse()
{
    Lines tmpLines = segments;
    segments.clear();
    Lines::reverse_iterator itera;
    for (itera = tmpLines.rbegin(); itera != tmpLines.rend(); itera++) {
        Line ln(*itera);
        ln.reverse();
        segments.push_back(ln);
    }
}



// Strips out segments that are shorter than the given length.
void Path::stripSegmentsShorterThan(double minlen)
{
    Lines::iterator itera = segments.begin();
    while (itera != segments.end()) {
        if (itera->length() < minlen) {
            Line ln = *itera;
            itera = segments.erase(itera);
            if (itera == segments.end()) {
                if (segments.size() > 0) {
                    segments.back().endPt = ln.endPt;
                }
            } else {
                itera->startPt = ln.startPt;
            }
        } else {
            itera++;
        }
    }
}



// Strips out segments that are redundantly linear, or very short.
void Path::simplify(double minErr)
{
    if (segments.size() < 2) {
        return;
    }
    Lines::iterator itera = segments.begin();
    Lines::iterator iterb = itera;
    for (iterb++; iterb != segments.end(); itera++, iterb++) {
        Line ln(itera->startPt, iterb->endPt);
        while (ln.minimumExtendedLineDistanceFromPoint(itera->endPt) <= minErr) {
            itera->endPt = iterb->endPt;
            iterb = segments.erase(iterb);
            if (iterb == segments.end()) {
                return;
            }
            ln.endPt = iterb->endPt;
        }
    }
    stripSegmentsShorterThan(minErr);
}



// Takes a bunch of unattached line segments and assembles them into contiguous paths.
Paths &Path::assemblePathsFromSegments(const Lines &segs, Paths &outPaths)
{
    Lines unhandled(segs);
    Path currPath;
    bool foundLink = false;
    while (unhandled.size() > 0) {
        if (currPath.size() == 0) {
            Line ln = unhandled.front();
            currPath.attach(ln);
            unhandled.pop_front();
        }
        foundLink = false;
        Lines::iterator itera = unhandled.begin();
        while (itera != unhandled.end()) {
            if (currPath.attach(*itera)) {
                itera = unhandled.erase(itera);
                foundLink = true;
            } else {
                itera++;
            }
        }
        if (!foundLink || unhandled.size() == 0) {
            outPaths.push_back(currPath);
            currPath = Path();
        }
    }
    return outPaths;
}



// Attempts to close paths by joining them together.
Paths &Path::repairUnclosedPaths(const Paths &paths, Paths &outPaths)
{
    Paths unhandled(paths);
    
    // filter out all completed paths.
    Paths::iterator itera = unhandled.begin();
    while (itera != unhandled.end()) {
        if (itera->isClosed()) {
            outPaths.push_back(*itera);
            itera = unhandled.erase(itera);
        } else {
            itera++;
        }
    }
    
    // Now we just have incomplete paths left.
    while (unhandled.size() > 0) {
        Path path = unhandled.front();
        unhandled.pop_front();
        for (;;) {
            // Find closest remaining incomplete path
            Paths::iterator closestIter;
            double closestDist = 9.0e9;
            double closingDist = path.startPoint().distanceFrom(path.endPoint());
            for (itera = unhandled.begin(); itera != unhandled.end(); itera++) {
                Path &path2 = *itera;
                double dist1 = path.endPoint().distanceFrom(path2.startPoint());
                double dist2 = path.endPoint().distanceFrom(path2.endPoint());
                if (dist1 < closestDist) {
                    closestDist = dist1;
                    closestIter = itera;
                }
                if (dist2 < closestDist) {
                    closestDist = dist2;
                    closestIter = itera;
                }
            }
            // If closest found incomplete path is closer than just closing the path, then attach it.
            if (closestDist < closingDist) {
                Path &path2 = *closestIter;
                path.attach(Line(path.endPoint(),path2.startPoint()));
                path.attach(path2);
                closestIter = unhandled.erase(closestIter);
            } else {
                // Closest found match is if we just close the path.
                if (path.size() < 2) {
                    break;
                }
                Line ln(path.endPoint(), path.startPoint());
                path.attach(ln);
            }
            if (path.isClosed()) {
                // Path has been closed.  On to the next path!
                outPaths.push_back(path);
                break;
            }
        }
    }
    return outPaths;
}



// Aligns this path more exactly to match up with the other given path.
// Will jigger segment endpoints to align with other path's endpoints.
// Will split segments where the other path crosses them.
// Will remove any resulting zero-length segments.
void Path::alignTo(const Path &path)
{
    Lines::iterator itera;
    Lines::iterator piter;
    Lines::const_iterator iterb;

    piter = segments.end();
    piter--;
    for (itera = segments.begin(); itera != segments.end(); itera++) {
        for (iterb = path.segments.begin(); iterb != path.segments.end(); iterb++) {

            if (itera->startPt.distanceFrom(iterb->endPt) <= CLOSEENOUGH) {
                // start point is close to other line's end point, jigger it to make it exact.
                itera->startPt = iterb->endPt;
                piter->endPt = itera->startPt;
            } else if (itera->startPt.distanceFrom(iterb->startPt) <= CLOSEENOUGH) {
                // start point is close to other line's start point, jigger it to make it exact.
                itera->startPt = iterb->startPt;
                piter->endPt = itera->startPt;
            } else if (iterb->minimumSegmentDistanceFromPoint(itera->startPt) <= CLOSEENOUGH) {
                // start point is close to the line, shift it onto the line.
                itera->startPt = iterb->closestSegmentPointTo(itera->startPt);
                piter->endPt = itera->startPt;
            }
        }

        if (piter->length() < EPSILON) {
            piter = segments.erase(piter);
        }
        piter = itera;
    }

    for (itera = segments.begin(); itera != segments.end(); itera++) {
        for (iterb = path.segments.begin(); iterb != path.segments.end(); iterb++) {

            if (itera->minimumSegmentDistanceFromPoint(iterb->startPt) <= CLOSEENOUGH) {
                // other segment's start point is on the current segment
                if (iterb->startPt.distanceFrom(itera->startPt) > CLOSEENOUGH &&
                    iterb->startPt.distanceFrom(itera->endPt) > CLOSEENOUGH
                ) {
                    // But is not at one end of the segment or the other. Split segment.
                    Point midPt = itera->closestSegmentPointTo(iterb->startPt);
                    Point firstPt = itera->startPt;
                    itera->startPt = midPt;
                    itera = segments.insert(itera, Line(firstPt, midPt));
                }
            }

            if (itera->minimumSegmentDistanceFromPoint(iterb->endPt) <= CLOSEENOUGH) {
                // other segment's end point is on the current segment
                if (iterb->endPt.distanceFrom(itera->startPt) > CLOSEENOUGH &&
                    iterb->endPt.distanceFrom(itera->endPt) > CLOSEENOUGH
                ) {
                    // But is not at one end of the segment or the other. Split segment.
                    Point midPt = itera->closestSegmentPointTo(iterb->endPt);
                    Point firstPt = itera->startPt;
                    itera->startPt = midPt;
                    itera = segments.insert(itera, Line(firstPt, midPt));
                }
            }

            Intersection isect = itera->intersectionWithSegment(*iterb);
            if (isect.type == POINT) {
                if (isect.p1.distanceFrom(itera->startPt) > CLOSEENOUGH &&
                    isect.p1.distanceFrom(itera->endPt) > CLOSEENOUGH
                ) {
                    Point firstPt = itera->startPt;
                    itera->startPt = isect.p1;
                    itera = segments.insert(itera, Line(firstPt, isect.p1));
                }
            }

        }
    }
}



void Path::splitSegmentsAtIntersectionsWithPath(const Path &path)
{
    alignTo(path);
    return;

    bool dodebug = false;

    Lines::iterator itera;
    Lines::const_iterator iterb;

    for (itera = segments.begin(); itera != segments.end(); itera++) {
        for (iterb = path.segments.begin(); iterb != path.segments.end(); iterb++) {
            if (dodebug) {
                cerr << "  " << *itera << " vs " << *iterb << "!  FIGHT!" << endl;
            }
            Points isects;
            Intersection isect = itera->intersectionWithSegment(*iterb);
            if (isect.type != NONE) {
                if (!itera->hasEndPoint(isect.p1)) {
                    isects.push_back(isect.p1);
                    if (dodebug) {
                        cerr << "    isect1 split itera at " << isect.p1 << endl;
                    }
                }
                if (isect.type == SEGMENT) {
                    if (!itera->hasEndPoint(isect.p2)) {
                        if (isect.p1 != isect.p2) {
                            double dist1 = itera->startPt.distanceFrom(isect.p1);
                            double dist2 = itera->startPt.distanceFrom(isect.p2);
                            if (dist2 > dist1) {
                                isects.push_front(isect.p2);
                            } else {
                                isects.push_back(isect.p2);
                            }
                            if (dodebug) {
                                cerr << "    isect2 split itera at " << isect.p2 << endl;
                            }
                        }
                    }
                }
            }
            Points::iterator iterc;
            for (iterc = isects.begin(); iterc != isects.end(); iterc++) {
                Point tempPt = itera->startPt;
                itera->startPt = *iterc;
                itera = segments.insert(itera, Line(tempPt, *iterc));
                if (dodebug) {
                    cerr << "    Actually split at " << *iterc << endl;
                }
            }
        }
    }
}



Paths &Path::separateSelfIntersectingSubpaths(Paths &outPaths)
{
    splitSegmentsAtIntersectionsWithPath(*this);

    Path subpath1;
    Path subpath2;
    Lines::iterator it1;
    Lines::iterator it2;
    Lines::iterator it3;
    bool found = false;
    for (it1 = segments.begin(); !found && it1 != segments.end(); it1++) {
        subpath1.segments.push_back(*it1);
        for (it2 = it1; it2 != segments.end(); it2++) {
            if (it1 != it2 && it1->endPt == it2->endPt) {
                it3 = it1;
                for (it3++; it3 != segments.end(); it3++) {
                    subpath2.segments.push_back(*it3);
                    if (it3 == it2) {
                        break;
                    }
                }
                it3 = it2;
                for (it3++; it3 != segments.end(); it3++) {
                    subpath1.segments.push_back(*it3);
                }
                found = true;
                break;
            }
        }
    }
    if (!found) {
        outPaths.push_back(*this);
        return outPaths;
    }

    subpath1.separateSelfIntersectingSubpaths(outPaths);
    subpath2.separateSelfIntersectingSubpaths(outPaths);
    return outPaths;
}



void Path::reorderByPoint(const Point &pt)
{
    int limit = segments.size();
    while (limit-->0) {
        Line &ln = segments.front();
        if (ln.startPt == pt) {
            return;
        }
        if (ln.endPt != pt && ln.contains(pt)) {
            ln.startPt = pt;
            segments.push_back(Line(endPoint(),pt));
            return;
        }
        segments.push_back(segments.front());
        segments.pop_front();
    }
}



void Path::untag()
{
    flags = OUTSIDE;
    Lines::iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        itera->flags = USED;
    }
}



void Path::tagSegmentsRelativeToClosedPath(const Path &path)
{
    bool invert = (flags == INSIDE);
    bool dodebug = false; // CURRDEBUG: set true

    Point midpt;
    Lines::iterator itera = segments.begin();
    for (; itera != segments.end(); itera++) {
        Line &seg = *itera;
        midpt = (seg.startPt + seg.endPt) / 2.0;
        Lines::const_iterator foundSeg = path.segments.end();
        bool isTagged = false;
        if (path.hasEdgeWithPoint(midpt, foundSeg)) {
            // Either shared or unshared segment.
            if (dodebug) {
                cerr << "SorU? ";
            }

            // Check if the matching segments point the same way.
            double dang = seg.angleDelta(*foundSeg);
            if (fabs(dang) < EPSILON || fabs(fabs(dang)-M_PI) < EPSILON) {
                // Segments are parallel, or very close to being so.

                // Figure out if segments are shared or unshared edges.
                bool isShared = (fabs(dang) < M_PI_2);

                // If the paths wind in opposite directions, invert sharedness test.
                if (isClockwise() != path.isClockwise()) {
                    isShared = !isShared;
                }

                if (invert) {
                    isShared = !isShared;
                }
                seg.flags = isShared? SHARED : UNSHARED;
                if (dodebug) {
                    cerr << (isShared? "S " : "U ");
                }
                isTagged = true;
            }
        }
        if (!isTagged) {
            if (dodebug) {
                cerr << "IorO? ";
            }
            bool isinside = path.contains(midpt);
            if (invert) {
                isinside = !isinside;
                if (dodebug) {
                    cerr << "Invert ";
                }
            }
            if (dodebug) {
                cerr << (isinside? "I " : "O ");
            }
            if (seg.flags == USED) {
                seg.flags = isinside? INSIDE : OUTSIDE;
            }
        }
        if (dodebug) {
            cerr << "Tagged: " << seg << "  ";
            if (seg.flags == INSIDE)   { cerr << "I"; }
            if (seg.flags == OUTSIDE)  { cerr << "O"; }
            if (seg.flags == SHARED)   { cerr << "S"; }
            if (seg.flags == UNSHARED) { cerr << "U"; }
            cerr << endl;
        }
    }
}



bool Path::insertVertexAttachedPath(const Path &inPath)
{
    Lines::iterator it1;
    Lines::iterator it2;
    Lines::iterator it3;

    if (inPath.size() < 1) {
        return false;
    }
    Path path(inPath);

    bool doFwd = (isClockwise() == path.isClockwise());
    it1 = path.begin();
    Point midPt = (it1->startPt + it1->endPt)/2.0;
    if (contains(midPt)) {
        doFwd = !doFwd;
    }
    if (!doFwd) {
        //cerr << "revit" << endl;
        path.reverse();
    }
    for (it1 = segments.begin(); it1 != segments.end(); it1++) {
        for (it2 = path.segments.begin(); it2 != path.segments.end(); it2++) {
            if (it2->startPt == it1->startPt) {
                //cerr << "joinit" << endl;
                for (it3 = it2; it3 != path.segments.end(); it3++) {
                    it1 = segments.insert(it1, *it3);
                    it1++;
                }
                for (it3 = path.segments.begin(); it3 != it2; it3++) {
                    it1 = segments.insert(it1, *it3);
                    it1++;
                }
                return true;
            }
        }
    }
    return false;
}



void Path::joinVertexAttachedPaths(Paths &paths)
{
    Paths::iterator it1;
    Paths::iterator it2;
    for (it1 = paths.begin(); it1 != paths.end(); it1++) {
        for (it2 = it1; it2 != paths.end(); it2++) {
            if (it1 != it2) {
                if (it1->insertVertexAttachedPath(*it2)) {
                    it2 = paths.erase(it2);
                }
            }
        }
    }
}




Paths& Path::assembleTaggedPaths(const Path &inPath1, int flags1, const Path &inPath2, int flags2, Paths &outPaths)
{
    bool dodebug = false; // CURRDEBUG: set true

    Path path1(inPath1);
    Path path2(inPath2);
    Paths finishedPaths;
    Paths tmpPaths;

    if (dodebug) {
        cerr << endl << endl;
        cerr << "inPath1=" << path1 << endl;
        cerr << "inPath2=" << path2 << endl;
    }

    path1.separateSelfIntersectingSubpaths(tmpPaths);
    joinVertexAttachedPaths(tmpPaths);
    path1 = tmpPaths.front();
    tmpPaths.clear();

    path2.separateSelfIntersectingSubpaths(tmpPaths);
    joinVertexAttachedPaths(tmpPaths);
    path2 = tmpPaths.front();
    tmpPaths.clear();

    if (dodebug) {
        cerr << endl;
        cerr << "untwisted inPath1=" << path1 << endl;
        cerr << "untwisted inPath2=" << path2 << endl;
    }

    path1.quantize(CLOSEENOUGH/2.0);
    path2.quantize(CLOSEENOUGH/2.0);

    if (dodebug) {
        cerr << endl;
        cerr << "quantized inPath1=" << path1 << endl;
        cerr << "quantized inPath2=" << path2 << endl;
    }

    path1.simplify(CLOSEENOUGH);
    path2.simplify(CLOSEENOUGH);

    if (dodebug) {
        cerr << endl;
        cerr << "simplified inPath1=" << path1 << endl;
        cerr << "simplified inPath2=" << path2 << endl;
    }

    // Align paths more exactly to each other.
    // Also split segments where other path intersects with us.
    path1.alignTo(path2);
    path2.alignTo(path1);

    if (dodebug) {
        cerr << endl;
        cerr << "aligned inPath1=" << path1 << endl;
        cerr << "aligned inPath2=" << path2 << endl;
    }

    path1.quantize(CLOSEENOUGH/2.0);
    path2.quantize(CLOSEENOUGH/2.0);

    if (dodebug) {
        cerr << endl;
        cerr << "quantize2 inPath1=" << path1 << endl;
        cerr << "quantize2 inPath2=" << path2 << endl;
    }

    // Align paths more exactly to each other.
    path1.alignTo(path2);
    path2.alignTo(path1);

    if (dodebug) {
        cerr << endl;
        cerr << "aligned2 inPath1=" << path1 << endl;
        cerr << "aligned2 inPath2=" << path2 << endl;
    }

    if (dodebug) {
        cerr << "-----------------Poly1---" << endl;
        cerr << (path1.isClockwise()? "CW" : "CCW") << endl;
    }

    // Tag segments for insideness, outsideness, or sharedness.
    path1.untag();
    path1.tagSegmentsRelativeToClosedPath(path2);

    if (dodebug) {
        cerr << "-----------------Poly2---" << endl;
        cerr << (path2.isClockwise()? "CW" : "CCW") << endl;
    }

    // Tag segments for insideness, outsideness, or sharedness.
    path2.untag();
    path2.tagSegmentsRelativeToClosedPath(path1);

    if (dodebug) {
        cerr << "-----------------End-----" << endl;
    }
    
    int remaining = path1.size() + path2.size();
    
    // Mark all unwanted segments in path1 as used.
    Lines::iterator itera = path1.segments.begin();
    for (; itera != path1.segments.end(); itera++) {
        if ((itera->flags & flags1) == 0) {
            itera->flags = USED;
            remaining--;
        }
    }
    
    // Mark all unwanted segments in path2 as used.
    Lines::iterator iterb = path2.segments.begin();
    for (; iterb != path2.segments.end(); iterb++) {
        if ((iterb->flags & flags2) == 0) {
            iterb->flags = USED;
            remaining--;
        }
    }
    
    // Try assembling path from unused segments.
    Lines::iterator currseg;
    Lines::iterator otherseg;
    currseg = path1.segments.begin();
    otherseg = path2.segments.begin();
    int pathLimit = 0;
    Path* patha = &path1;
    Path* pathb = &path2;

    finishedPaths.push_back(Path());
    Path* outPath = &finishedPaths.back();
    while (remaining > 0) {
        Line &seg = *currseg;
        if (seg.flags != USED && outPath->couldAttach(seg)) {
            // Found a connected unused segment.
            // Attach it to the current path.
            seg.flags = USED;
            outPath->attach(seg);
            remaining--;
            pathLimit = 0;
            currseg++;
            if (currseg == patha->segments.end()) {
                currseg = patha->segments.begin();
            }
            
            // If path was closed by this segment, remember it and start a new path.
            if (outPath->isClosed()) {
                if (dodebug) {
                    cerr << (outPath->isClockwise()? "CW" : "CCW") << endl;
                }
                finishedPaths.push_back(Path());
                outPath = &finishedPaths.back();
                pathLimit = 0;
            }
        } else {
            pathLimit++;
            
            // Swap over to the other path.
            Path* tmppath = patha;
            patha = pathb;
            pathb = tmppath;
            
            Lines::iterator tmpseg = currseg;
            currseg = otherseg;
            otherseg = tmpseg;
            
            // Find a connected unused segment in the new path.
            // Stop looking if we completely circumnavigate the path.
            int limit = 0;
            for (limit = patha->size(); limit > 0; limit--) {
                currseg++;
                if (currseg == patha->segments.end()) {
                    currseg = patha->segments.begin();
                }
                if (currseg->flags != USED && outPath->couldAttach(*currseg)) {
                    break;
                }
            }
            
            if (limit == 0 && remaining > 0 && pathLimit >= 2) {
                // Failed to find another connected segment in either path.
                // Remember this path, and start a new one.
                if (dodebug) {
                    cerr << (outPath->isClockwise()? "CW" : "CCW") << endl;
                }
                if (dodebug && !outPath->isClosed()) {
                    cerr << endl;
                    cerr << "******************" << endl;
                    cerr << "RESULT NOT CLOSED1" << endl;
                    cerr << "******************" << endl;
                    cerr << endl;
                }
                finishedPaths.push_back(Path());
                outPath = &finishedPaths.back();
                pathLimit = 0;
            }
        }
    }
    if (outPath->size() == 0) {
        // Drop final path if empty.
        finishedPaths.pop_back();
        cerr << "pop it" << endl;
    } else {
        if (dodebug) {
            cerr << (outPath->isClockwise()? "CW" : "CCW") << endl;
        }
        if (dodebug && !outPath->isClosed()) {
            cerr << endl;
            cerr << "******************" << endl;
            cerr << "RESULT NOT CLOSED2" << endl;
            cerr << "******************" << endl;
            cerr << endl;
        }
    }
    Paths::iterator pit;
    for (pit = finishedPaths.begin(); pit != finishedPaths.end(); pit++) {
        pit->separateSelfIntersectingSubpaths(outPaths);
    }
    joinVertexAttachedPaths(outPaths);

    for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
        pit->quantize(CLOSEENOUGH/2.0);
        pit->simplify(CLOSEENOUGH);
        if (dodebug) {
            cerr << "resultPath=" << *pit << endl;
        }
    }
    
    return outPaths;
}



Paths &Path::unionOf(const Path &path1, const Path &path2, Paths &outPaths)
{
    return assembleTaggedPaths(path1, (OUTSIDE|SHARED), path2, OUTSIDE, outPaths);
}



Paths &Path::differenceOf(const Path &path1, const Path &path2, Paths &outPaths)
{
    return assembleTaggedPaths(path1, (OUTSIDE|UNSHARED), path2, INSIDE, outPaths);
}



Paths &Path::intersectionOf(const Path &path1, const Path &path2, Paths &outPaths)
{
    return assembleTaggedPaths(path1, (INSIDE|SHARED), path2, INSIDE, outPaths);
}



Paths &Path::unionOf(Paths &inPaths, Paths &outPaths)
{
    Paths::iterator it1;
    Paths::iterator it2;
    Paths::iterator it3;

    Paths paths;
    for (it1 = inPaths.begin(); it1 != inPaths.end(); it1++) {
        paths.push_back(*it1);
    }
    
    for (it1 = paths.begin(); it1 != paths.end(); ) {
        bool found = false;
        for (it2 = it1; it2 != paths.end(); it2++) {
            if (it1 != it2) {
                Paths tempPaths;
                Path::unionOf(*it1, *it2, tempPaths);
                if (tempPaths.size() == 1) {
                    paths.push_back(tempPaths.front());
                    it2 = paths.erase(it2);
                    it1 = paths.erase(it1);
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            it1++;
        }
    }
    for (it1 = paths.begin(); it1 != paths.end(); it1++) {
        outPaths.push_back(*it1);
    }
    return outPaths;
}



// BUG: Not quite a correct algorithm.
// Some diffs return multiple paths, where one is inside the other.
// Diffing a set of paths from another set of paths should return a CompoundRegion.
Paths &Path::differenceOf  (Paths &paths1, Paths &paths2, Paths &outPaths)
{
    Paths::iterator it1;
    Paths::iterator it2;

    Paths paths;
    paths = paths1;
    for (it2 = paths2.begin(); it2 != paths2.end(); it2++) {
        Paths tempPaths;
        for (it1 = paths.begin(); it1 != paths.end(); it1++) {
            Path::differenceOf(*it1, *it2, tempPaths);
        }
        paths = tempPaths;
    }
    for (it1 = paths.begin(); it1 != paths.end(); it1++) {
        outPaths.push_back(*it1);
    }
    return outPaths;
}



Lines &Path::containedSegments(const Line &line, Lines &outSegs) const
{
    Path linePath;
    linePath.attach(line);

    linePath.quantize(CLOSEENOUGH/2.0);

    // Align paths more exactly to each other.
    // Also split segments where other path intersects with us.
    linePath.alignTo(*this);

    linePath.untag();
    linePath.tagSegmentsRelativeToClosedPath(*this);
    
    Lines::iterator itera = linePath.segments.begin();
    for (; itera != linePath.segments.end(); itera++) {
        Line &seg = *itera;
        if (seg.flags == INSIDE || seg.flags == SHARED || seg.flags == UNSHARED) {
            outSegs.push_back(seg);
        }
    }
    return outSegs;
}



Paths &Path::containedSubpathsOfPath(Path &path, Paths outPaths) const
{
    path.quantize(CLOSEENOUGH/2.0);

    // Align paths more exactly to each other.
    // Also split segments where other path intersects with us.
    path.alignTo(*this);

    path.untag();
    path.tagSegmentsRelativeToClosedPath(*this);
    
    Lines outSegs;
    Lines::iterator itera = path.segments.begin();
    for (; itera != path.segments.end(); itera++) {
        Line &seg = *itera;
        if (seg.flags == INSIDE || seg.flags == SHARED || seg.flags == UNSHARED) {
            outSegs.push_back(seg);
        }
    }
    
    return assemblePathsFromSegments(outSegs, outPaths);
}



Paths &Path::leftOffset(double offsetby, Paths& outPaths)
{
    Path offsetPath;
    Lines offsetLines;
    Lines::iterator itera;
    Lines::iterator iterb;
    Lines::iterator iterc;
    Lines::iterator prevValid;
    int lastValidity = VALID;
    Point prevPt;
    bool closed = isClosed();

    if (segments.size() == 0) {
        return outPaths;
    }
    if (segments.size() == 1) {
        Line ln(segments.front());
        ln.leftOffset(offsetby);
        Path outPath;
        outPath.attach(ln);
        outPaths.push_back(outPath);
        return outPaths;
    }

    // Make raw insets lines.
    if (closed) {
        // TODO: handle closure
    } else {
        prevPt = segments.front().startPt;
    }
    for (iterb = segments.begin(), itera = iterb++, iterc = itera;
         iterb != segments.end();
         itera++, iterb++
        ) {
        double ang = itera->angle();
        double deltaAng = itera->angleDelta(*iterb);
        double bisectAng = (M_PI-deltaAng)/2.0 + ang;

        Point bisectPt(itera->endPt);
        bisectPt.polarOffset(bisectAng, offsetby);

        Line ln(prevPt, bisectPt);
        if (fabs(itera->angleDelta(ln)) > M_PI_2) {
            // Line got reversed by insetting.  Mark it invalid.
            if (lastValidity != VALID) {
                ln.flags = CONSECUTIVELY_INVALID;
                if (iterc->flags == INVALID) {
                    iterc->flags = CONSECUTIVELY_INVALID;
                }
            } else {
                ln.flags = INVALID;
            }
        } else {
            ln.flags = VALID;
            prevValid = itera;
        }
        lastValidity = ln.flags;
        offsetLines.push_back(ln);

        prevPt = bisectPt;
        iterc = itera;
    }
    if (closed) {
        // TODO: handle closure
    } else {
        Line ln(prevPt, itera->endPt);
        if (fabs(itera->angleDelta(ln)) > M_PI_2) {
            // Line got reversed by insetting.  Mark it invalid.
            if (lastValidity == INVALID) {
                ln.flags = CONSECUTIVELY_INVALID;
            } else {
                ln.flags = INVALID;
            }
        } else {
            ln.flags = VALID;
        }
        lastValidity = ln.flags;
        offsetLines.push_back(ln);
    }

    // Start pruning invalid segments.
    Lines::iterator nextValid = offsetLines.end();
    for (itera = offsetLines.begin(), iterb = segments.begin();
         itera != offsetLines.end();
         itera++, iterb++
        ) {
        if (itera->flags != VALID) {
            bool didUpdate = false;

            // Find next valid line.
            nextValid = itera;
            for (int limit = 2; limit-->0;) {
                while (nextValid != offsetLines.end() && nextValid->flags != VALID) {
                    nextValid++;
                }
                if (nextValid == offsetLines.end()) {
                    if (limit>0) {
                        nextValid = offsetLines.begin();
                        continue;
                    }
                }
                break;
            }
            if (nextValid == offsetLines.end()) {
                // No valid segments!  We have a null path.
                offsetLines.clear();
                break;
            }

            if (itera->flags == INVALID) {
                // case I
                Intersection isect = prevValid->intersectionWithExtendedLine(*nextValid);
                if (isect.type != NONE) {
                    prevValid->endPt = isect.p1;
                    nextValid->startPt = isect.p1;
                    itera = offsetLines.erase(itera);
                    iterb++;
                    didUpdate = true;
                }
            } else {
                // case II
                // TODO: find matching non-offset segments
                // TODO: do pairwise intersections of offsets from segments
                // set aside list of invalid segment's original segments.
                // for each original segment, trim off intersections from offset path
            }
            if (!didUpdate) {
                // TODO: Connect forward and backwards edge.
            }
        } else {
            prevValid = itera;
        }
    }
    // TODO: finish this.
    return outPaths;
}



// Currently implemented as brute force using boolean geometry.
// Outset is a negative inset.
Paths &Path::inset(double insetBy, Paths& outPaths)
{
    bool dodebug = false;
    const double minimum_arc_segment_length = 1.0;
    const double minimum_arc_angle = M_PI_4 / 8.0;
    const double maximum_arc_angle = M_PI / 3.0;

    if (dodebug) {
        cerr << "Inset by " << insetBy << endl;
    }

    if (!isClosed()) {
        if (dodebug) {
            cerr << "NOT CLOSED." << endl;
        }
        return outPaths;
    }

    double leftOffset = insetBy;
    // Translate inset into a left offset.
    if (!isClockwise()) {
        if (dodebug) {
            cerr << "Invert offset." << endl;
        }
        leftOffset = -leftOffset;
    }
    if (dodebug) {
        cerr << "Left Offset by " << leftOffset << endl;
    }

    //simplify(CLOSEENOUGH/2.0);

    Paths trimPaths;
    Paths::iterator pit;
    Line offsetLine;

    // Check start-of-path segment against end-of-path segment
    Lines::iterator lit = segments.begin();
    Lines::iterator prevlit = segments.end();

    for (prevlit--; lit != segments.end(); lit++) {
        bool isConvex = (prevlit->angleDelta(*lit) < 0.0);
        if (leftOffset < 0) {
            isConvex = !isConvex;
        }
        if (dodebug) {
            cerr << (isConvex? "Convex" : "Concave") << endl;
        }
        if (isConvex) {
            // Pointy angle.  Needs added segments to arc around the tip.
            offsetLine = *prevlit;
            offsetLine.leftOffset(leftOffset);
            offsetLine.reverse();

            double sang = prevlit->angle() + M_PI_2;
            double eang = lit->angle() + M_PI_2;
            if (sang > M_PI) {
                sang -= 2.0 * M_PI;
            }
            if (eang > M_PI) {
                eang -= 2.0 * M_PI;
            }
            if (eang - sang > M_PI) {
                eang -= 2.0 * M_PI;
            }
            if (eang - sang < -M_PI) {
                eang += 2.0 * M_PI;
            }

            // Calculate a good arc angle.
            // We don't want too many tiny arc steps.
            // Try to keep them about 1mm apart.
            // We need to circumscribe the ideal arc, not inscribe.
            // y = r * sin(a)
            // a = asin(y/r)
            double minstepang = maximum_arc_angle;
            if (fabs(insetBy) >= minimum_arc_segment_length) {
                minstepang = asin(minimum_arc_segment_length/fabs(insetBy));
                if (minstepang < minimum_arc_angle) {
                    minstepang = minimum_arc_angle; // Don't go overboard with facets.
                } else if (minstepang > maximum_arc_angle) {
                    minstepang = maximum_arc_angle;
                }
            }
            // Actually add arc segment boxes.
            double step = (eang-sang) / (floor(fabs(eang-sang) / minstepang)+1);
            for (double ang = sang + step/2.0; fabs(ang-step/2.0-(eang+step)) > fabs(step/2); ang += step) {
                offsetLine.endPt = offsetLine.startPt;
                offsetLine.startPt = lit->startPt;
                offsetLine.startPt.polarOffset(ang, leftOffset/cos(step/2.0));

                trimPaths.push_back(Path());
                Path &trimPath2 = trimPaths.back();
                trimPath2.segments.push_back(Line(lit->startPt, offsetLine.startPt));
                trimPath2.segments.push_back(offsetLine);
                trimPath2.segments.push_back(Line(offsetLine.endPt, lit->startPt));
                if (dodebug) {
                    cerr << "arcTrim" << endl;
                    trimPath2.svgPathWithOffset(cerr, 10, 10);
                }
            }
        }

        // Add the box between the current segment and the offset line.
        offsetLine = *lit;
        offsetLine.leftOffset(leftOffset);
        offsetLine.reverse();

        trimPaths.push_back(Path());
        Path &trimPath = trimPaths.back();
        trimPath.segments.push_back(*lit);
        trimPath.segments.push_back(Line(lit->endPt, offsetLine.startPt));
        trimPath.segments.push_back(offsetLine);
        trimPath.segments.push_back(Line(offsetLine.endPt, lit->startPt));

        if (dodebug) {
            cerr << "rectTrim" << endl;
            trimPath.svgPathWithOffset(cerr, 10, 10);
        }

        prevlit = lit;
    }

    if (true || dodebug) {
        for (pit = trimPaths.begin(); pit != trimPaths.end(); pit++) {
            cerr << "Trimpath=" << *pit << endl;
        }
        cerr << "PathToTrim=" << *this << endl;
    }

    if (insetBy < 0.0) {
        // Outsetting. Union to existing path.
        Paths pathsToExpand;
        pathsToExpand.push_back(*this);
        for (pit = trimPaths.begin(); pit != trimPaths.end(); pit++) {
            pathsToExpand.push_back(*pit);
        }
        unionOf(pathsToExpand, outPaths);
    } else {
        // Insetting. Diff from existing path.
        Paths pathsToTrim;
        pathsToTrim.push_back(*this);
        differenceOf(pathsToTrim, trimPaths, outPaths);
    }

    if (dodebug) {
        cerr << endl << endl;
    }

    return outPaths;
    //return leftOffset(leftOffset, outPaths);
}



ostream& operator <<(ostream &os, const Path &path)
{
    os << "{";
    Lines::const_iterator it = path.segments.begin();
    for ( ; it != path.segments.end(); it++) {
        os << *it;
    }
    os << "}";
    return os;
}


}
// vim: set ts=4 sw=4 nowrap expandtab: settings

