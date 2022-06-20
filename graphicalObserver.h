#ifndef GRAPHICALOBSERVER
#define GRAPHICALOBSERVER

#include "observer.h"
#include "cc3k.h"
#include "window.h"
#include <iostream>
#include <memory>

class CC3K;

class GraphicalObserver : public Observer
{
    // We do not use a smart pointer here because Observer does not own CC3K
    CC3K* subject;
    int width;
    int height;
    const int SCALE = 16;
    std::shared_ptr<Xwindow> theWindow;
public:
    GraphicalObserver(CC3K* subject, int width, int height);
    void notify() override;
    ~GraphicalObserver();
};

#endif
