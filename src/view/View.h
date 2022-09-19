//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_VIEW_H
#define MEDGRAPHICS_VIEW_H

#include "Context.h"

namespace view {

    class View {
    public:
        View(Context* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        View(Context* context) : View(context, 0, 0, 0, 0) {}

        unsigned int getX() const;

        unsigned int getY() const;

        unsigned int getWidth() const;

        unsigned int getHeight() const;

        virtual void render() = 0;

        virtual ~View() = default;

    protected:
        unsigned int x, y, width, height;
        Context* context;
    };

} // view

#endif //MEDGRAPHICS_VIEW_H
