#include "ofApp.h"

using namespace ofxCv;
//--------------------------------------------------------------
void ofApp::setup(){
    
    //
    gaussianSize = 9;
    kernelSize = 3;
    
    filePath = "/Users/kerbal/Desktop/defect.jpg";
    before.load(filePath);
}
//--------------------------------------------------------------
void ofApp::update(){
    
    cv::Mat img = toCv(before);
    cv::cvtColor(img, img, CV_BGR2GRAY);
    // resize
    float ratio = 256.0 / img.cols;
    cv::resize(img, img, cv::Size(img.cols*ratio, img.rows*ratio));
    
    // Invert ----------
    //cv::Mat out2;
    //bitwise_not(img, out2);
    
    cv::Mat planes[] = {cv::Mat_<float>(img), cv::Mat::zeros(img.size(), CV_32F)};
    cv::Mat complexImg;
    cv::merge(planes, 2, complexImg);
    cv::dft(complexImg, complexImg);
    
    cv::split(complexImg, planes);
    
    cv::Mat mag, logmag, smooth, spectralResidual;
    cv::Mat mag1;
    
    cv::magnitude(planes[0], planes[1], mag);
    cv::log(mag, logmag);
    
    //cv::normalize(logmag, mag1, 255, 0, NORM_MINMAX, CV_8U);
    //toOf(mag1, original);
    //original.update();
    
    cv::boxFilter(logmag, smooth, -1, cv::Size(kernelSize, kernelSize));
    cv::subtract(logmag, smooth, spectralResidual);
    cv::exp(spectralResidual, spectralResidual);
    
    planes[0] = planes[0].mul(spectralResidual) / mag;
    planes[1] = planes[1].mul(spectralResidual) / mag;
    
    cv::merge(planes, 2, complexImg);
    cv::dft(complexImg, complexImg, cv::DFT_INVERSE | cv::DFT_SCALE);
    cv::split(complexImg, planes);
    cv::magnitude(planes[0], planes[1], mag);
    
    //cv::normalize(mag, mag1, 255, 0, NORM_MINMAX, CV_8U);
    //toOf(mag1, original);
    //original.update();
    
    cv::multiply(mag, mag, mag);
    cv::GaussianBlur(mag, mag, cv::Size(gaussianSize, gaussianSize), 2.5, 2.5);
    
    // normalize & show
    cv::normalize(mag, mag1, 255, 0, cv::NORM_MINMAX, CV_8U);
    toOf(mag1, after);
    after.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    before.draw(0, 0, 512, 512);
    after.draw(513, 0, 512, 512);
    
        // filled box
    ofSetColor(10, 10, 10);
    ofDrawRectangle(5, 5, 200, 60);
    
    // text
    ofSetColor(255);
    ofDrawBitmapString("Gaussian Size= " + ofToString(gaussianSize), 20, 30);
    ofDrawBitmapString("Kernel Size= " + ofToString(kernelSize), 20, 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key=='s') {
        string filename = ofGetTimestampString()+".png";
        std::cout << "save to "<< filename <<std::endl;
        
        //std::cout << "original size: " << original.getWidth()
        //<< " " << original.getHeight() <<std::endl;
        after.update();
        after.save(filename);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

    gaussianSize= (int)(x / 1024.0 * 40);
    if (gaussianSize % 2 == 0){
        gaussianSize += 1;
    }
    
    kernelSize= (int)(y / 512.0 * 10);
    if (kernelSize % 2 == 0){
        kernelSize += 1;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    if( dragInfo.files.size() > 0 ){
        before.load(dragInfo.files[0]);
        before.setImageType(OF_IMAGE_COLOR);
    }
    //
    ofSetWindowTitle(dragInfo.files[0]);

}
