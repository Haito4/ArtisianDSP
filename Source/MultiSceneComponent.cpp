#include "MultiSceneComponent.h"


MultiSceneComponent::MultiSceneComponent(ArtisianDSPAudioProcessor& processor)
    : audioProcessor(processor),
      scene1(std::make_unique<Gate1Component>(processor)),
      scene2(std::make_unique<Comp2Component>(processor)),
      scene3(std::make_unique<Drive3Component>(processor)),
      scene4(std::make_unique<Amp4Component>(processor)),
      scene5(std::make_unique<Reverb5Component>(processor)),
      scene6(std::make_unique<Impulse6Component>(processor))
{
    addAndMakeVisible(scene1.get());
    addAndMakeVisible(scene2.get());
    addAndMakeVisible(scene3.get());
    addAndMakeVisible(scene4.get());
    addAndMakeVisible(scene5.get());
    addAndMakeVisible(scene6.get());
    
    // Initially hide scenes other than scene1
    scene2->setVisible(false);
    scene3->setVisible(false);
    scene4->setVisible(false);
    scene5->setVisible(false);
    scene6->setVisible(false);

    sceneButton1.onClick = [this] { showScene(1); };
    sceneButton2.onClick = [this] { showScene(2); };
    sceneButton3.onClick = [this] { showScene(3); };
    sceneButton4.onClick = [this] { showScene(4); };
    sceneButton5.onClick = [this] { showScene(5); };
    sceneButton6.onClick = [this] { showScene(6); };

    sceneButton1.setButtonText("GATE");
    sceneButton2.setButtonText("COMP");
    sceneButton3.setButtonText("OD");
    sceneButton4.setButtonText("AMP");
    sceneButton5.setButtonText("RVB");
    sceneButton6.setButtonText("CAB");
    
    addAndMakeVisible(sceneButton1);
    addAndMakeVisible(sceneButton2);
    addAndMakeVisible(sceneButton3);
    addAndMakeVisible(sceneButton4);
    addAndMakeVisible(sceneButton5);
    addAndMakeVisible(sceneButton6);
}

MultiSceneComponent::~MultiSceneComponent()
{
}

void MultiSceneComponent::resized()
{
    scene1->setBounds(getLocalBounds());
    scene2->setBounds(getLocalBounds());
    scene3->setBounds(getLocalBounds());
    scene4->setBounds(getLocalBounds());
    scene5->setBounds(getLocalBounds());
    scene6->setBounds(getLocalBounds());

    sceneButton1.setBounds(110, 10, 80, 50);
    sceneButton2.setBounds(194, 10, 80, 50);
    sceneButton3.setBounds(278, 10, 80, 50);
    sceneButton4.setBounds(362, 10, 80, 50);
    sceneButton5.setBounds(446, 10, 80, 50);
    sceneButton6.setBounds(530, 10, 80, 50);
}

void MultiSceneComponent::showScene(int sceneIndex)
{
    scene1->setVisible(sceneIndex == 1);
    scene2->setVisible(sceneIndex == 2);
    scene3->setVisible(sceneIndex == 3);
    scene4->setVisible(sceneIndex == 4);
    scene5->setVisible(sceneIndex == 5);
    scene6->setVisible(sceneIndex == 6);
}

