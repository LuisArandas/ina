#include "ofApp.h"

bool doSetTheme = false;
/* Add warp of output and warp of mesh */

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    gui.setup();
    /*ImGui::GetIO().MouseDrawCursor = false;
    show_test_window = true;*/

    backgroundColor = ofColor(114, 144, 154);
    show_another_window = false;
    floatValue = 0.0f;
    
    /*imageButtonSource.load("of.png");
    imageButtonID = gui.loadImage(imageButtonSource);
    ofLoadImage(pixelsButtonSource, "of_upside_down.png");
    pixelsButtonID = gui.loadPixels(pixelsButtonSource);
    textureSourceID = gui.loadTexture(textureSource, "of_upside_down.png");
    ofLogVerbose() << "textureSourceID: " << textureSourceID;
    ofDirectory dataDirectory(ofToDataPath("", true));
    files = dataDirectory.getFiles();
    for(size_t i=0; i<files.size(); i++) {
        fileNames.push_back(files[i].getFileName());
    }*/
    
    // 60 fps!
    warper.setup();
    warper.activate();
    recenterQuad();
    warper.deactivate();
    
    ofSetVerticalSync(true);
    
    ofLoadImage(img, "image.png");
    img.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    //img.load("image.jpg");
    
    for(int i = 0; i < 80; i++){
        balls[i].setup(ofRandom(10, ofGetWidth() - 10), ofRandom(10, ofGetHeight()-10), ofRandom(5, 25));
        balls[i].vel.x = ofRandom(1.5, 2.8);
        balls[i].vel.y = ofRandom(1.5, 2.8);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Bolas
    for(int i = 0; i < 80; i++) {
        balls[i].update(ofGetWidth(), ofGetHeight());
    }
    
    if(doSetTheme) {
        doSetTheme = false;
        gui.setTheme(new ThemeTest());
    }
}
bool doThemeColorsWindow = false;
//--------------------------------------------------------------
void ofApp::draw(){
    
    //This might not be needed
    ofSetBackgroundColor(backgroundColor);
    
    gui.begin();
    ImGui::SetNextWindowSize(ofVec2f(ofGetWidth()/4, ofGetHeight()-30));
    ImGui::SetNextWindowPos(ofVec2f(15,15));
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    window_flags |= ImGuiWindowFlags_MenuBar;
    
    bool showControlsWindow;
    bool p_open = true;
    bool flag;
    bool show_app_main_menu_bar;
    
    ImGui::Begin("gui", &p_open, window_flags);  // Menubar Window
    ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);

    if (ImGui::BeginMenuBar()) {
        // View menu
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("File", NULL, &show_app_main_menu_bar);
            ImGui::MenuItem("Save", NULL, &show_app_main_menu_bar);
            ImGui::MenuItem("Exit", NULL, &show_app_main_menu_bar);
            ImGui::Checkbox("Flag", &flag);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Test")) {
            ImGui::MenuItem("File", NULL, &show_app_main_menu_bar);
            ImGui::MenuItem("Save", NULL, &show_app_main_menu_bar);
            ImGui::MenuItem("Exit", NULL, &show_app_main_menu_bar);
            ImGui::Checkbox("Flag", &flag);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::SetNextWindowSize(ofVec2f(ofGetWidth()/4, ofGetHeight()-30));
    ImGui::SetNextWindowPos(ofVec2f(ofGetWidth()-((ofGetWidth()/4)+15),ofGetHeight()-(ofGetHeight()-15)));
    ImGui::Begin("Interface", &p_open, window_flags);  // Menubar Window
    
    //ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui::SliderFloat("Float", &floatValue, 0.0f, 1.0f);
    ImGui::ColorEdit3("Background Color", (float*)&backgroundColor);
        
    if (ImGui::Button("Demo Window")) {
        show_test_window = !show_test_window;
    }
        
    if (ImGui::Button("Another Window")) {
        cout << "test";
            
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
    if (show_test_window){
        ImGui::SetNextWindowPos(ofVec2f(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowDemoWindow(&show_test_window);
    }
    
    ImGui::End();
    gui.end();
    
    
    /* Mapping stuff again */
    
    warper.begin();
    
    ofSetHexColor(0xAAAAAA);
    
    if(img.isAllocated()){
        img.draw(0,0,1);
    }
    
    ofNoFill();
    ofSetHexColor(0xffffff);
    ofDrawRectangle(1, 1, ofGetWidth()-2, ofGetHeight()-2);

    ofEnableAlphaBlending();
    ofSetColor(255, 0, 255, 130);
    ofFill();
    for(int i = 0; i < 40; i++)balls[i].draw();
    ofDisableAlphaBlending();
    
    warper.end();// feel free to move this to another position within de draw chain to see how this affects the drawing.
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //ofLogVerbose(__FUNCTION__) << key;
    switch (key) {
        case ' ': {
            if (warper.isActive()) {
                warper.deactivate();
            } else {
                warper.activate();
            }
            break;
        }
        case '9': {
            recenterQuad();
        }
        case '0': {
            cout << "ok \n";
            //warper.selectCorner(ofxGLWarper::TOP_RIGHT); // Select
            //warper.setCorner(ofxGLWarper::TOP_RIGHT, 50., 50.); // Move
            //cout << warper.getCorner(ofxGLWarper::TOP_LEFT);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //ofLogVerbose(__FUNCTION__) << key;
    
}


void ofApp::mouseScrolled(float x, float y)
{
    //ofLogVerbose(__FUNCTION__) << "x: " << x << " y: " << y;
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
//--------------------------------------------------------------
void ofApp::recenterQuad(){
    warper.setCorner(ofxGLWarper::TOP_LEFT, ofGetWidth()/4+30, ofGetHeight()/8);
    warper.setCorner(ofxGLWarper::TOP_RIGHT, ofGetWidth()-(ofGetWidth()/4+30), ofGetHeight()/8);
    warper.setCorner(ofxGLWarper::BOTTOM_LEFT, ofGetWidth()/4+30, ofGetHeight()-(ofGetHeight()/8)-15);
    warper.setCorner(ofxGLWarper::BOTTOM_RIGHT, ofGetWidth()-(ofGetWidth()/4+30), ofGetHeight()-(ofGetHeight()/8)-15);
}

