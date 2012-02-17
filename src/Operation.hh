#ifndef OPERATION_H
#define OPERATION_H

class Operation {
public:
    bool isCancelled;

    Operation() : isCancelled(false) {}
    virtual ~Operation() {};
    virtual void main() = 0;
};

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

