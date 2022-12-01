//
// Created by danil on 23.11.2022.
//

#ifndef MEDGRAPHICS_WIDGET_ATTRIBUTES_H
#define MEDGRAPHICS_WIDGET_ATTRIBUTES_H

#define VIEW_ATTRS \
int id = 0;        \
Dimension width = WRAP_CONTENT;   \
Dimension height = WRAP_CONTENT;  \
Padding padding;   \
BackgroundWrapper background;       \
Visibility visibility = VISIBLE;    \
Style style;

#define VIEW_ATTRS_SET(src) \
id = src.id;                \
width = src.width;          \
height = src.height;        \
padding = src.padding;      \
background = src.background;\
visibility = src.visibility;\
style = src.style;

#define TEXT_VIEW_ATTRS                         \
String text;                                    \
std::filesystem::path font;                     \
rgba fontColor = rgba{255, 255, 255, 255};      \
int fontSize = 14;

#define TEXT_VIEW_ATTRS_SET(src)    \
text = src.text;                    \
font = src.font;                    \
fontColor = src.fontColor;          \
fontSize = src.fontSize;

#define EDIT_TEXT_VIEW_ATTRS                    \
InputType inputType = TEXT;

#define EDIT_TEXT_VIEW_ATTRS_SET(src)    \
inputType = src.inputType;

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
