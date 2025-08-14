#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#define TFT_CS 2      
#define TFT_RST 3     
#define TFT_DC 4  
#define TFT_LED 5

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <Arduino.h>

#include <Adafruit_ILI9341.h> 


#include <stdtype.h>

#include <vector>
#include <string>

enum ElementType {
    ElementTypeContainer, // A container that can hold other elements
    ElementTypeText, // A text element
    ElementTypePolygon // A polygon element
};

enum Positioning {
    PositionFree, // Free positioning, element can be placed anywhere using x, y coordinates
    PositionLayout // Layout positioning, element is positioned based on the layout rules of its container
};

enum Alignment {
    AlignStretch, // Stretch the element's box to fill the available space 
    AlignStart, // Align the element's box to the start of the available space
    AlignEnd, // Align the element's box to the end of the available space
    AlignCenter, // Center the element's box in the available space
    AlignSpaceBetween // Distribute the element's box with space between them in the container
};

enum Justification {
    JustifyStart, // Justify content to the start of the container
    JustifyEnd, // Justify content to the end of the container
    JustifyCenter // Center content in the container
};

enum LayoutDirection {
    LayoutRow, // Layout items in a row (horizontal)
    LayoutColumn // Layout items in a column (vertical)
};

enum OverflowMode {
    OverflowGrow, // Grow the container to fit content
    OverflowHide, // Hide content that overflows the container
    OverflowScroll // Allow scrolling for overflowing content
};

enum ComputedLength {
    LengthAuto = -1, 
    LengthNone = -2, 
    LengthFull = -3,
};

enum TextAlignment {
    TextAlignLeft, 
    TextAlignRight, 
    TextAlignCenter, 
    TextAlignJustify
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ComputedLayout {
    int16_t x; // Computed x position of the element
    int16_t y; // Computed y position of the element
    int16_t width; // Computed width of the element
    int16_t height; // Computed height of the element
}; 

class BaseElement {
public:
    ComputedLayout layout; // Computed layout of the element

    ElementType elementType; // Type of the element (container, text, polygon)
    Positioning positioning; // Positioning of the element in the layout§
    Alignment selfAlign; // Self-alignment of the element within its container
    int16_t x; // X position of the element
    int16_t y; // Y position of the element
    int16_t width; // Width of the element
    int16_t height; // Height of the element
    int16_t maxWidth; // Maximum width of the element
    int16_t maxHeight; // Maximum height of the element
    int16_t minWidth; // Minimum width of the element
    int16_t minHeight; // Minimum height of the element
    int16_t zIndex; // Z-index for stacking order of the element
    int16_t grow; // Grow factor for the element in the layout (for flexbox-like behavior)
    int16_t basis; // Base size for the element in the layout (for flexbox-like behavior)
    int16_t marginLeft; // Margin around the element
    int16_t marginRight; // Margin around the element
    int16_t marginTop; // Margin around the element
    int16_t marginBottom; // Margin around the element

    bool visible; // Whether the element is visible (still layouted but not drawn)
    bool displayed; // Whether the element is layouted and takes space in the layout

    BaseElement() {

        positioning = PositionFree;
        selfAlign = AlignStretch;
        x = 0;
        y = 0;
        width = LengthAuto;
        height = LengthAuto;
        maxWidth = LengthNone;
        maxHeight = LengthNone;
        minWidth = LengthNone;
        minHeight = LengthNone;
        zIndex = 0;
        grow = 0; 
        basis = LengthAuto;
        marginLeft = 0;
        marginRight = 0;
        marginTop = 0;
        marginBottom = 0;

        visible = true; 
        displayed = true; 
    }
}; 


class Container: public BaseElement {
public:
    int16_t paddingLeft; // Padding around the content
    int16_t paddingRight;
    int16_t paddingTop;
    int16_t paddingBottom;

    Color backgroundColor; // Background color of the container
    bool fillBackground; // Whether to fill the background with the color
    Color borderColor; // Border color of the container
    int16_t borderWidth; // Border width of the container
    int16_t borderRadius; // Border radius for rounded corners
    OverflowMode overflow; // Overflow behavior of the container

    LayoutDirection layoutDirection; // Direction of the layout (row or column)
    Justification contentJustify; // Justification of the content within the container
    Alignment contentAlign; // Alignment of the content within the container
    int16_t contentGap; // Gap between content items in the container

    std::vector<BaseElement*> children; // List of child elements in the container

    Container(){
        elementType = ElementTypeContainer;

        paddingLeft = 0;
        paddingRight = 0;
        paddingTop = 0;
        paddingBottom = 0;

        backgroundColor = {255, 255, 255}; // Default white
        fillBackground = false; 
        borderColor = {0, 0, 0}; // Default black
        borderWidth = 0;
        borderRadius = 0;
        overflow = OverflowGrow;

        layoutDirection = LayoutRow;
        contentJustify = JustifyStart;
        contentAlign = AlignStretch;
        contentGap = 0; 
    }
}; 

class Text: public BaseElement {
public:
    std::string text; // The text content of the element
    TextAlignment textAlign; // Text alignment within it's container
    Color textColor; // Color of the text
    uint8_t textSize; // Size of the text in pixels
    bool bold; // Whether the text is bold
    bool italic; // Whether the text is italic

    Text() {
        elementType = ElementTypeText;

        text = "";
        textAlign = TextAlignLeft;
        textColor = {0, 0, 0}; // Default black
        textSize = 1; // Default size
        bold = false; 
        italic = false; 
    }
}; 

class Polygon: public BaseElement {
public:
    std::vector<int16_t> points; // List of x, y pairs
    Color fillColor; // Fill color of the polygon
    Color strokeColor; // Stroke color of the polygon
    int16_t strokeWidth; // Width of the stroke
    bool fill; // Whether to fill the polygon
    bool stroke; // Whether to draw the stroke

    Polygon() {
        elementType = ElementTypePolygon;

        fillColor = {0, 0, 0}; // Default black
        strokeColor = {255, 255, 255}; // Default white
        strokeWidth = 1;
        fill = false; 
        stroke = true; 
    }
};

class BaseGraphicsContext {
public:
    virtual void begin() = 0; // Initialize the graphics context
    virtual void drawText(std::string str, int16_t x, int16_t y, Color color, int16_t size, bool isBold, bool isItalic) = 0; // Draw text on the screen
    virtual void drawRectangle(int16_t x, int16_t y, int16_t width, int16_t height, Color color ) = 0; 
    virtual void drawRectangleOutline(int16_t x, int16_t y, int16_t width, int16_t height, Color color, int16_t lineWidth) = 0; 
    virtual void drawRectangleRounded(int16_t x, int16_t y, int16_t width, int16_t height, Color color) = 0;
    virtual void drawRectangleRoundedOutline(int16_t x, int16_t y, int16_t width, int16_t height, Color color, int16_t lineWidth) = 0;
};

class RenderEngine {
public:
    void begin() {

    }

    void layout(Container* rootContainer, int16_t availableWidth, int16_t availableHeight) {

        //start with the main axis pass 

        //decide the children's base size
        
    }
};

#endif // RENDER_ENGINE_HPP