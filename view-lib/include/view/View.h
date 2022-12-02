//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_VIEW_H
#define MEDGRAPHICS_VIEW_H

#include <functional>
#include <view/core/Context.h>
#include <utils/Texture.h>
#include <img/rgba.h>
#include <view/Style.h>
#include "MouseEvent.h"
#include "../../../src/widget/WidgetAttributes.h"

namespace view {

    enum State {
        DEFAULT,
        HOVERED,
        PRESSED
    };

    enum Visibility {
        VISIBLE,
        GONE,
        INVISIBLE
    };

    struct SpaceRequirement {
        union {
            struct {
                float width = 0;
                float height = 0;
                float parentPartW = 0;
                float parentPartH = 0;
                float parentSparePartW = 0;
                float parentSparePartH = 0;
            };
            struct {
                float size[2];
                float parentPart[2];
                float parentSparePart[2];
            };
        };
    };

    typedef std::function<void(view::View* view, const MouseEvent& event)> OnMouseEventListener;
    typedef std::function<void(view::View* view)> OnClickListener;

    struct ViewAttributes {
        VIEW_ATTRS
    };

    class View {
    public:
        View(Context* context, const ViewAttributes& attr);

        View(const View& other) = delete;

        View(const View&& other) = delete;

        View& operator=(const View& other) = delete;

        View& operator=(const View&& other) = delete;

        virtual ~View() = default;

        Style& getStyle();

        virtual bool onMouseEvent(const MouseEvent& event);

        /// @return whether event was consumed
        virtual bool onClick(const MouseEvent& event);

        virtual bool onDrag(double x, double y, double dx, double dy);

        View& setOnClickListener(const std::function<void()>& _onClickListener);

        void setOnMouseEventListener(const OnMouseEventListener& listener);

        View& setOnClickListener(const OnClickListener& listener);

        virtual void onWindowResize(unsigned int width, unsigned int height);

        View& setOnWindowResizeListener(std::function<void(View& v, unsigned int w, unsigned int h)>&& _onWindowResize);

        virtual void onMouseEnter();

        virtual void onMouseLeave();

        virtual void onMouseMove(double x, double y);

        virtual bool onScroll(double xOffset, double yOffset, double x, double y);

        virtual bool onKey(int key, int scancode, int action, int mods);

        virtual bool onChar(unsigned int codepoint);

        void draw();

        virtual void onDraw();

        virtual bool isInside(double x, double y);

        const Context* getContext() const;

        int getId() const;

        View* setId(int _id);

        virtual View* findViewById(int _id);

        State getState() const;

        void setState(State _state);

        SpaceRequirement measure();

        virtual float getContentWidth();

        virtual float getContentHeight();

        virtual SpaceRequirement onMeasure();

        void layout(float left, float top, float right, float bottom);

        virtual void onLayout(float left, float top, float right, float bottom) {}

        void setParent(View* parent);

        View* getParent();

        bool isDirty() const;

        void invalidate();

        void setBackground(const BackgroundWrapper& background);

        void setVisibility(Visibility visibility);

        Visibility getVisibility();

    protected:
        Context* context;
        State state = DEFAULT;
        OnMouseEventListener onMouseEventListener = nullptr;
        OnClickListener onClickListener = nullptr;
        std::function<void(View& view, unsigned int width, unsigned int height)> onWindowResizeListener = nullptr;

        VIEW_ATTRS

        struct edges {
            float left = 0;
            float top = 0;
            float right = 0;
            float bottom = 0;

            float width() const {
                return right - left;
            }

            float height() const {
                return bottom - top;
            }
        } edges, innerEdges;

        View* parent = nullptr;
    private:
        // Layout staff
        SpaceRequirement measuredSpaceRequirement{};
        bool isMeasured = false;
        bool needRerender = true;
    };

} // view

#endif //MEDGRAPHICS_VIEW_H
