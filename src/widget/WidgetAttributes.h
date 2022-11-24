//
// Created by danil on 23.11.2022.
//

#ifndef MEDGRAPHICS_WIDGET_ATTRIBUTES_H
#define MEDGRAPHICS_WIDGET_ATTRIBUTES_H

#define VIEW_ATTRS \
int id = 0;        \
Dimension width;   \
Dimension height;  \
Padding padding;   \
BackgroundWrapper background;  \
Style style;

#define VIEW_ATTRS_SET(src) \
id = src.id;                \
width = src.width;          \
height = src.height;        \
padding = src.padding;      \
background = src.background; \
style = src.style;

#define TEXT_VIEW_ATTRS                         \
String text;                                    \
std::filesystem::path font;                     \
rgba fontColor{COLOR_FONT_PRIMARY};             \
int fontSize = 14;

#define TEXT_VIEW_ATTRS_SET(src)    \
text = src.text;                    \
font = src.font;                    \
fontColor = src.fontColor;          \
fontSize = src.fontSize;

#define VIEW_GROUP_ATTRS            \
std::vector<View*> children;

#define VIEW_GROUP_ATTRS_SET(src)   \
for (auto* child : src.children)    \
    addChild(child);

#define LINEAR_LAYOUT_ATTRS \
Orientation orientation = VERTICAL;\
Gravity gravity = CENTER;

#define LINEAR_LAYOUT_ATTRS_SET(src) \
orientation = src.orientation;\
gravity = src.gravity;\

#endif //MEDGRAPHICS_WIDGET_ATTRIBUTES_H
