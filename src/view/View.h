//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_VIEW_H
#define MEDGRAPHICS_VIEW_H

namespace view {

    class View {
    public:
        View(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        unsigned int getX() const;

        unsigned int getY() const;

        unsigned int getWidth() const;

        unsigned int getHeight() const;

        virtual void render() {
        }

    private:
        unsigned int x, y, width, height;
    };

} // view

#endif //MEDGRAPHICS_VIEW_H
