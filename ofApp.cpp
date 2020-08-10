#include "ofApp.h"

void ofApp::setup()
{
    ofBackground(0);
    ofSetFrameRate(40);
    
    effect.init(ofGetWidth(), ofGetHeight());
    
    //Stage 10: Bleach ByPass: Also called silver retention, is a chemical effect on any image while printing a film 
    //Press Key '0' to activate and deactivate the shader
    effect.createPass<BleachBypassPass>()->setEnabled(false);
    
    //Stage 1: Bloom Pass: Produces fringes of bright light from sharp edges of 3d object
    //Press Key '1' to activate and deactivate the shader
    effect.createPass<BloomPass>()->setEnabled(false);
    
    //Stage 2: Dof Pass: Makes advanced depth rendering of the scene 
    //Press Key '2' to activate and deactivate the shader
    effect.createPass<DofPass>()->setEnabled(false);
    
    //Stage 3: KaleidoScope Pass: Is a symmetrical pattern produced by two reflecting surfaces tilted on an angle
    //Press Key '3' to activate and deactivate the shader
    effect.createPass<KaleidoscopePass>()->setEnabled(false);
    
    //Stage 4: Noise warp Pass: makes "deformed" or "warped" textures of non-specific forms
    //Press Key '4' to activate and deactivate the shader
    effect.createPass<NoiseWarpPass>()->setEnabled(false);
    
    //Stage 5: Pixelate Pass: Divides (an image) into pixels. caused by displaying a bitmap
    //Press Key '5' to activate and deactivate the shader
    effect.createPass<PixelatePass>()->setEnabled(false);
    
    //Stage 6: Edge Pass: This is a band-pass filter, allowing light of particular frequencies to pass through.
    //Press Key '6' to activate and deactivate the shader
    effect.createPass<EdgePass>()->setEnabled(false);
    
    //Stage 7: VerticalTiltShift: This effect is a change in camera position w.r.t to the sensors to add special effects.
    //Press Key '7' to activate and deactivate the shader
    effect.createPass<VerticalTiltShifPass>()->setEnabled(false);
    
    //Stage 8: RGB Shift Pass: Red, green or blue values of pixels in an image and applying those values to pixels in different positions on the image. 
    ////Press Key '8' to activate and deactivate the shader
    effect.createPass<RGBShiftPass>()->setEnabled(false);
    
    //Stage 9: God Rays Pass: This is called volumetric light scattering and is similar to the effect produced by sun light behind clouds.
    //////Press Key '9' to activate and deactivate the shader
    effect.createPass<GodRaysPass>()->setEnabled(false);
    
    
    for (unsigned i = 0; i < NUM_BOXES; ++i)
    {
        positions.push_back(ofVec3f(ofRandom(-600, 600), ofRandom(-600, 600), ofRandom(-600, 600)));
        colors.push_back(ofColor::fromHsb(255 * i / (float)NUM_BOXES, 255, 255, 255));
    }
    
    light.setPosition(1000, 1000, 2000);
    
    
    boxes = ofMesh::box(40, 40, 40);
}

void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::draw()
{
    ofEnableDepthTest();
    light.enable();
    
    effect.begin(cam);
    
    for (unsigned i = 0; i < positions.size(); ++i)
    {
        ofSetColor(colors[i]);
        ofPushMatrix();
        ofTranslate(positions[i]);
        boxes.draw();
        ofPopMatrix();
    }
    
    ofDrawAxis(0);
    
    effect.end();
    
    
}

void ofApp::keyPressed(int key)
{
    unsigned int id = key - 48;
    if (id < effect.size()) effect[id]->setEnabled(!effect[id]->getEnabled());
}

