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
    pics[0].loadImage("../../img/000.jpg");
    pics[1].loadImage("../../img/001.jpg");
    pics[2].loadImage("../../img/002.jpg");
    pics[3].loadImage("../../img/003.jpg");
    pics[4].loadImage("../../img/004.jpg");
    picIndex = 0;
    
    good.loadImage("../../img/good.jpg");
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
    
    pics[picIndex].draw(0, 0);
    ofPushMatrix();
    // draw debug (ie., image, depth, skeleton)
//    openNIDevice.drawDebug();
    openNIDevice.drawImage(WINDOW_WIDTH/2, 0);
    openNIDevice.drawSkeletons(WINDOW_WIDTH/2, 0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
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
        
        ofxOpenNIJoint torso = user.getJoint(JOINT_TORSO);
    
        float lHandHead = getAngleBetweenJoints(leftHand, head);
        float rHandHead = getAngleBetweenJoints(rightHand, head);
        float lElbowHead = getAngleBetweenJoints(leftElbow, head);
        float rElbowHead = getAngleBetweenJoints(rightElbow, head);
        float lHandTorso = getAngleBetweenJoints(leftHand, torso);
        float rHandTorso = getAngleBetweenJoints(rightHand, torso);
        
        ofSetColor(0, 0, 0);
        verdana.drawString("lHandHead : " + ofToString(lHandHead), 20, 480);
        verdana.drawString("rHandHead : " + ofToString(rHandHead), 20, 500);
        verdana.drawString("lElbowHead : " + ofToString(lElbowHead), 20, 520);
        verdana.drawString("rElbowHead : " + ofToString(rElbowHead), 20, 540);
        verdana.drawString("lHandTorso : " + ofToString(lHandTorso), 20, 560);
        verdana.drawString("rHandTorso : " + ofToString(rHandTorso), 20, 580);
        
        switch(picIndex){
            case 0:
                if (lHandHead >= 0.045 && lHandHead <= 0.2 &&
                    rHandHead >= 5.2 && rHandHead <= 5.4 &&
//                    lElbowHead >= 5.7 && lElbowHead <= 5.9 &&
//                    rElbowHead >= 5.2 && rElbowHead <= 5.4 &&
                    lHandTorso >= 0.8 && lHandTorso <= 1.0 &&
                    rHandTorso >= 5.5 && rHandTorso <= 5.7)
                    {
                        ofSetColor(0, 0, 0);
                        verdana.drawString("BINGO!", 20, 400);
                        sleep(1);
                        picIndex++;
                    }
                break;
            case 1:
                if (((lHandHead >= 5.2 && lHandHead <= 6.1) || (lHandHead >= 0.3 && lHandHead <= 0.45)) &&
                    rHandHead >= 4.7 && rHandHead <= 5.2 &&
                    lHandTorso >= 1.15 && lHandTorso <= 1.55 &&
                    rHandTorso >= 0.9 && rHandTorso <= 1.5
                    )
                {
                    good.draw(50, 200);
                    sleep(1);
                    picIndex++;
                }
                break;
            case 2:
                if (lHandHead >= 5.1 && lHandHead <= 5.35 &&
                    rHandHead >= 0.1 && rHandHead <= 0.5 &&
                    lHandTorso >= 5.5 && lHandTorso <= 5.8 &&
                    rHandTorso >= 0.8 && rHandTorso <= 1.2)
                {
                    good.draw(50, 200);
                    sleep(1);
                    picIndex++;
                }
                break;
            case 3:
                if (lHandHead >= 1.1 && lHandHead <= 1.6 &&
                    rHandHead >= 5.1 && rHandHead <= 5.4 &&
                    lHandTorso >= 1.4 && lHandTorso <= 1.6 &&
                    rHandTorso >= 5.4 && rHandTorso <= 5.6)
                {
                    good.draw(50, 200);
                    sleep(1);
                    picIndex++;
                }
                break;
            case 4:
                if (lHandHead >= 0.15 && lHandHead <= 0.35 &&
                    rHandHead >= 0.15 && rHandHead <= 0.35 &&
                    lHandTorso >= 0.8 && lHandTorso <= 1.2 &&
                    rHandTorso >= 0.8 && rHandTorso <= 1.2)
                {
                    good.draw(50, 200);
                    sleep(1);
                    picIndex = 0;
                }
                break;
            default:
                break;
        }
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