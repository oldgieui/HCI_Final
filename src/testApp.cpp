#include "testApp.h"
#include <math.h>
//#define PI 3.14

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.setSkeletonProfile(XN_SKEL_PROFILE_ALL);

    openNIDevice.setLogLevel(OF_LOG_VERBOSE);
    openNIDevice.setResolution(640, 480, 30);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(1);



    
    // setup the hand generator
//    openNIDevice.addHandsGenerator();
    
    // add all focus gestures (ie., wave, click, raise arm)
//    openNIDevice.addAllHandFocusGestures();
    
    // or you can add them one at a time
    //vector<string> gestureNames = openNIDevice.getAvailableGestures(); // you can use this to get a list of gestures
                                                                         // prints to console and/or you can use the returned vector
    //openNIDevice.addHandFocusGesture("Wave");
    
    openNIDevice.setMaxNumHands(4);
    
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true); 
        // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
        // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true, 
        // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false, 
        // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    
    openNIDevice.start();
    openNIDevice.setFrame(30);
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 18);
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    
    // reset all depthThresholds to 0,0,0
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i);
        ofPoint leftBottomNearWorld = ofPoint(0,0,0);
        ofPoint rightTopFarWorld = ofPoint(0,0,0);
        ofxOpenNIROI roi = ofxOpenNIROI(leftBottomNearWorld, rightTopFarWorld);
        depthThreshold.setROI(roi);
    }
    
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handWorldPosition = hand.getWorldPosition(); // remember to use world position for setting ROIs!!!
        
        // set depthThresholds based on handPosition
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
        ofPoint leftBottomNearWorld = handWorldPosition - 100; // ofPoint has operator overloading so it'll subtract/add 50 to x, y, z
        ofPoint rightTopFarWorld = handWorldPosition + 100;
        
        ofxOpenNIROI roi = ofxOpenNIROI(leftBottomNearWorld, rightTopFarWorld);
        depthThreshold.setROI(roi);
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 255, 255);
    
    ofPushMatrix();
    // draw debug (ie., image, depth, skeleton)
    openNIDevice.drawDebug();
    ofPopMatrix();
    
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
//        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
//        ofPoint & handPosition = hand.getPosition();
        
        // draw a rect at the position
//        ofSetColor(255,0,0);
//        ofRect(handPosition.x, handPosition.y, 2, 2);
        
        // set depthThresholds based on handPosition
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
        
        // draw ROI over the depth image
//        ofSetColor(255,255,255);
//        depthThreshold.drawROI();
        
        // draw depth and mask textures below the depth image at 0.5 scale
        // you could instead just do pixel maths here for finger tracking etc
        // by using depthThreshold.getDepthPixels() and/or depthThreshold.getMaskPixels()
        // and turn off the textures in the initial setup/addDepthTexture calls
        
        ofPushMatrix();
        ofTranslate(320 * i, 480);
        ofScale(0.5, 0.5);
        depthThreshold.drawDepth();
        depthThreshold.drawMask();
        ofPopMatrix();
        
        // i think this is pretty good but might be a frame behind???
        
    }
    
    ofDisableBlendMode();
    ofPopMatrix();
    
    RenderSkeleton();
    
    // draw some info regarding frame counts etc
//	ofSetColor(0, 255, 0);
//	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
//	verdana.drawString(msg, 20, 480 - 20);
}

//--------------------------------------------------------------
void testApp::handEvent(ofxOpenNIHandEvent & event){
    // show hand event messages in the console
    ofLogNotice() << getHandStatusAsString(event.handStatus) << "for hand" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

void testApp::RenderSkeleton(){
    int numUsers = openNIDevice.getNumTrackedUsers();
    for (int i = 0; i<numUsers; i++) {
        openNIDevice.drawSkeleton(0, 0, WINDOW_WIDTH/640, WINDOW_HEIGHT/480, i);
        ofxOpenNIUser &user = openNIDevice.getTrackedUser(i);
        
        if (!user.isSkeleton())
        {
            break;
        }
        
        ofxOpenNIJoint leftHand = user.getJoint(JOINT_LEFT_HAND);
        ofxOpenNIJoint rightHand = user.getJoint(JOINT_RIGHT_HAND);
        
        ofxOpenNIJoint leftElbow = user.getJoint(JOINT_LEFT_ELBOW);
        ofxOpenNIJoint rightElbow = user.getJoint(JOINT_RIGHT_ELBOW);
        
        ofxOpenNIJoint leftShoulder = user.getJoint(JOINT_LEFT_SHOULDER);
        ofxOpenNIJoint rightShoulder = user.getJoint(JOINT_RIGHT_SHOULDER);
        
        ofxOpenNIJoint head = user.getJoint(JOINT_HEAD);
        ofxOpenNIJoint neck = user.getJoint(JOINT_NECK);
    
//        float leftHandAndShoulder = getAngleBetweenJoints(leftHand, leftShoulder);
//        float rightHandAndShoulder = getAngleBetweenJoints(rightHand, rightShoulder);
//        
////        leftHandAndShoulder = static_cast<int>(leftHandAndShoulder);
//        
//        if(leftHandAndShoulder - rightHandAndShoulder > 0 && leftHandAndShoulder - rightHandAndShoulder <= 1 ){
//            ofSetColor(128, 255, 0);
//            verdana.drawString("0 < left - right <= 1", 20, 400);
//        }
//        
//        if(rightHandAndShoulder - leftHandAndShoulder > 0 && rightHandAndShoulder - leftHandAndShoulder <= 1 ){
//            ofSetColor(128, 255, 0);
//            verdana.drawString("0 < right - left <= 1", 20, 400);
//        }
        
        
        float lHandHead = getAngleBetweenJoints(leftHand, head);
        float rHandHead = getAngleBetweenJoints(rightHand, head);
        float lElbowHead = getAngleBetweenJoints(leftElbow, head);
        float rElbowHead = getAngleBetweenJoints(rightElbow, head);
        
        ofSetColor(0, 255, 0);
        verdana.drawString("leftHand : " + ofToString(lHandHead), 20, 380);
        verdana.drawString("rightHand : " + ofToString(rHandHead), 20, 400);
        verdana.drawString("leftElbow : " + ofToString(lElbowHead), 20, 420);
        verdana.drawString("rightElbow : " + ofToString(rElbowHead), 20, 440);
//        verdana.drawString("Left : " + ofToString(leftHandAndShoulder), 20, 420);
//        verdana.drawString("Right : " + ofToString(rightHandAndShoulder), 20, 440);
    }
    
}

float testApp::getAngleBetweenJoints(ofxOpenNIJoint j1, ofxOpenNIJoint j2){
    ofPoint& j1P = j1.getProjectivePosition();
    ofPoint& j2P = j2.getProjectivePosition();
    
    return getAngleBetweenPoints(j1P.x, j1P.y, j2P.x, j2P.y);
}


float testApp::getAngleBetweenPoints(float x1, float y1, float x2, float y2){

    float a = x2 - x1;
    if(a < 0 ){
        a *= -1;
    }
    float b = y2 - y1;
    if(b < 0){
        b *= -1;
    }
    float c = sqrt(pow(a, 2) + pow(b, 2));
    
    float angle = acosf(a / c);
    
    if(y1 > y2) {
        angle = (2*PI) - angle;
    }
    return angle;
}