//
//  Knob3D.hpp
//  gradientColor - VST3
//
//  Created by Sheila Gonzalez on 2020-09-13.
//

#ifndef Knob3D_hpp
#define Knob3D_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include <stdio.h>

//==============================================================================
/**
*/
class JTLookAndFeel : public LookAndFeel_V4
{
public:
    JTLookAndFeel(){}
    ~JTLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                              float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
       {
           // width = heigth .... take minimum length
            if (width != height)
            {
                width = width < height ? width : height;
                height = width;
            }
            auto diameter = (float) jmin (width, height) - 4.0f;
            auto radius = (diameter / 2.0f) * std::cos (MathConstants<float>::pi / 4.0f);
            auto centreX = (float) x + (float) width  * 0.5f;
            auto centreY = (float) y + (float) height * 0.5f;
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

            Rectangle<float> r (rx, ry, rw, rw);
            auto transform = AffineTransform::rotation (angle, r.getCentreX(), r.getCentreY());

            auto x1 = r.getTopLeft()   .getX();
            auto y1 = r.getTopLeft()   .getY();
           auto x2 = r.getBottomLeft().getX();
            auto y2 = r.getBottomLeft().getY();

            transform.transformPoints (x1, y1, x2, y2);
           
           double reduce = width*0.25;
           
            // Shadow
            g.setGradientFill(juce::ColourGradient(juce::Colours::transparentBlack, width - reduce,
                                                   height/2, juce::Colour(0xFF000000).withAlpha(0.05f),
                                                   width*0.625, height-reduce, false));
            Path circ;
           double rs1 = (width - reduce)/2.f;
           double rArcS = width - reduce*0.3f;
           double rs2 = rArcS/2.f;
            circ.startNewSubPath(r.getCentreX() + rs1*cos(190*MathConstants<float>::pi/180.f),
                                r.getCentreY() + rs1*sin(190*MathConstants<float>::pi/180.f));
           
            circ.lineTo(r.getCentreX() + rs1*cos(370*MathConstants<float>::pi/180.f),
                        r.getCentreY() + rs1*sin(370*MathConstants<float>::pi/180.f));
           
            circ.lineTo(r.getCentreX() + rs2*cos(60*MathConstants<float>::pi/180.f),
                        r.getCentreY() + rs2*sin(60*MathConstants<float>::pi/180.f));
            
            circ.addArc(r.getCentreX() - rs2, r.getCentreY() - rs2, rArcS, rArcS, 150*MathConstants<float>::pi/180.f, 240*MathConstants<float>::pi/180.f);
           
            circ.lineTo(r.getCentreX() + rs2*cos(160*MathConstants<float>::pi/180.f),
                        r.getCentreY() + rs2*sin(160*MathConstants<float>::pi/180.f));
           
            circ.closeSubPath();
            g.fillPath(circ);
           
            // MAIN KNOB
            g.setColour(juce::Colour(0xff808080));
            Path mainKnob;
            float startAng = 0*MathConstants<float>::pi/180.f; //30
            float endAng = 360*MathConstants<float>::pi/180.f; //330
            mainKnob.addArc(reduce / 2.0f, reduce / 2.0f, width - reduce, height - reduce, endAng + angle, startAng + angle, true);
            g.fillPath (mainKnob);
           
           // light reflect
           g.setGradientFill(juce::ColourGradient(juce::Colour(0xffffffff).withAlpha(0.8f),
                                                  r.getCentreX() + rs1*cos(305*MathConstants<float>::pi/180.f),
                                                  r.getCentreY() + rs1*sin(305*MathConstants<float>::pi/180.f),
                                                  juce::Colour(0xffaaaaaa).withAlpha(0.1f),
                                                  reduce,
                                                  reduce, true));
            Path whiteLight;
            whiteLight.addArc(reduce / 2.0f, reduce / 2.0f, width - reduce, height - reduce, endAng, startAng, true);
            g.fillPath(whiteLight);

           // inner lighted ring
           Path innerRing;
           innerRing.addArc(r.getCentreX() - width*0.715*0.5, r.getCentreY() - width*0.715*0.5, width*0.715, width*0.715, 0, 2*MathConstants<float>::pi, true);
           g.strokePath(innerRing, PathStrokeType (width <= 200 ? 2.0f : 3.0f));

           // Finger Hole
           g.setColour(juce::Colour(0xff303030).withAlpha(0.5f));
           float cX = r.getCentreX() + width*0.25*cos(angle - 1.555);
            float cY = r.getCentreY() + width*0.25*sin(angle - 1.555);
            Path circleKnob;
           circleKnob.addEllipse(cX-width*0.175*0.5, cY-width*0.175*0.5, width*0.175, width*0.175);
//           g.setGradientFill(juce::ColourGradient(juce::Colour(0xff000000),
//                                                  r.getCentreX() + 50*cos(3.7699 - 1.555),
//                                                  r.getCentreY() + 50*sin(3.7699 - 1.555) - 300.0*sliderPos,
//                                                  juce::Colour(0xffaaaaaa).withAlpha(0.8f),
//                                                  r.getCentreX() + 135*cos(3.7699 - 1.555),
//                                                  r.getCentreY() + 135*sin(3.7699 - 1.555) - 300.0*sliderPos, false));
            g.fillPath(circleKnob);
            g.setColour(juce::Colours::lightgrey);
            g.strokePath(circleKnob, PathStrokeType (0.1f));

       }
};


#endif /* Knob3D_hpp */
