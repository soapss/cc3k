#ifndef TEXTOBSERVER_H
#define TEXTOBSERVER_H

#include "observer.h"
#include "cc3k.h"
#include <iostream>
#include <memory>

class CC3K;

class TextObserver : public Observer
{
    // We do not use a smart pointer here because Observer does not own CC3K
    CC3K* subject;
    int width;
    int height;
    std::ostream &out = std::cout;

public:
    TextObserver(CC3K *subject, int width, int height);
    void notify() override;
    ~TextObserver();
};

#endif
