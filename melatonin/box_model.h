#pragma once
#include "helpers.h"

namespace melatonin
{
    class BoxModel : public Component, public Label::Listener
    {
    public:
        BoxModel()
        {
            addAndMakeVisible (componentLabel);
            componentLabel.setText ("Select a Component!", dontSendNotification);

            addAndMakeVisible (widthLabel);
            widthLabel.setEditable (true);
            widthLabel.addListener (this);
            widthLabel.setFont (20.0f);
            widthLabel.setJustificationType (Justification::centredRight);

            addAndMakeVisible (byLabel);
            byLabel.setText (L" × ", dontSendNotification);
            byLabel.setFont (20.f);
            byLabel.setJustificationType (Justification::centred);

            addAndMakeVisible (heightLabel);
            heightLabel.setEditable (true);
            heightLabel.addListener (this);
            heightLabel.setFont (20.0f);
            heightLabel.setJustificationType (Justification::centredLeft);

            Label* parentLabels[4] = { &topToParentLabel, &rightToParentLabel, &bottomToParentLabel, &leftToParentLabel };

            for (auto parentLabel : parentLabels)
            {
                addAndMakeVisible (parentLabel);
                parentLabel->setText ("-", dontSendNotification);
                parentLabel->setJustificationType (Justification::centred);
            }
        }

        void paint (Graphics& g) override
        {
            // dotted rectangles be hard yo
            g.setColour (color::redLineColor);
            float dashLengths[2] = { 3.f, 3.f };
            parentRectangle.clear();
            parentRectangle.addRectangle (getLocalBounds().reduced (padding));

            auto parentStroke = PathStrokeType (0.5);
            parentStroke.createDashedStroke (parentRectangle, parentRectangle, dashLengths, 2);
            g.strokePath (parentRectangle, parentStroke);

            auto paddingPerSide = padding + paddingToParent;
            g.setColour (color::blueLineColor);
            g.drawRect (getLocalBounds().reduced (paddingPerSide));
        }

        void resized() override
        {
            auto center = getLocalBounds().getCentre();
            auto labelWidth = 60;
            auto labelHeight = 30;
            widthLabel.setBounds (center.getX() - 70, center.getY() - 15, labelWidth, labelHeight);
            byLabel.setBounds (center.getX() - 10, center.getY() - 15, 20, labelHeight);
            heightLabel.setBounds (center.getX() + 10, center.getY() - 15, labelWidth, labelHeight);

            topToParentLabel.setBounds (center.getX() - labelWidth / 2, padding + paddingToParent / 2 - labelHeight / 2, labelWidth, labelHeight);
            rightToParentLabel.setBounds (getWidth() - padding - paddingToParent / 2 - labelWidth / 2, center.getY() - labelHeight / 2, labelWidth, labelHeight);
            bottomToParentLabel.setBounds (center.getX() - labelWidth / 2, getHeight() - padding - paddingToParent / 2 - labelHeight / 2, labelWidth, labelHeight);
            leftToParentLabel.setBounds (padding + paddingToParent / 2 - labelWidth / 2, center.getY() - labelHeight / 2, labelWidth, labelHeight);
        }

        void setComponent (Component* selectedComponent)
        {
            component = selectedComponent;
            auto boundsInParent = component->getBoundsInParent();
            componentLabel.setText (componentString (component), dontSendNotification);
            widthLabel.setText (String (component->getWidth()), dontSendNotification);
            heightLabel.setText (String (component->getHeight()), dontSendNotification);

            topToParentLabel.setText (String (boundsInParent.getY()), dontSendNotification);
            rightToParentLabel.setText (String (component->getParentWidth() - component->getWidth() - boundsInParent.getX()), dontSendNotification);
            bottomToParentLabel.setText (String (component->getParentHeight() - component->getHeight() - boundsInParent.getY()), dontSendNotification);
            leftToParentLabel.setText (String (boundsInParent.getX()), dontSendNotification);
            repaint();
        }

        void labelTextChanged (Label* changedLabel) override
        {
            if (changedLabel == &widthLabel || changedLabel == &heightLabel)
            {
                component->setSize (widthLabel.getText().getIntValue(), heightLabel.getText().getIntValue());
            }
        }

    private:
        Component::SafePointer<Component> component;
        Label componentLabel;
        Label widthLabel;
        Label byLabel;
        Label heightLabel;

        Label topToParentLabel;
        Label rightToParentLabel;
        Label bottomToParentLabel;
        Label leftToParentLabel;

        int padding = 50;
        int paddingToParent = 50;
        Path parentRectangle;
    };
}