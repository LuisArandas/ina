#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/app/Window.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Surface.h"
#include "cinder/Timeline.h"
#include <list>

#include "Warp.h"
#include "CinderImGui.h"

#include "cinder/qtime/QuickTimeGl.h"

#include <CoreGraphics/CGEventSource.h>
#include <Carbon/Carbon.h>
#include "InputSimulation.h"

//Change top bar color
//Load Images and videos in vec<>
//Im gui keypress
//Limpar isto
//Prints da consola no ecrã
//Quando editar retira a UI e
//cria outra pequenita a direita
//capture basic
//basic app window <- check the drawing

using namespace ci;
using namespace ci::app;
using namespace ph::warping;
using namespace std;

class InaApp : public App {
  public:
    
    static void prepareSettings( Settings *settings );
    void setup() override;
    void cleanup() override;
    void update() override;
    void draw() override;
    void mouseMove( MouseEvent event ) override;
    void mouseDown( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    void mouseUp( MouseEvent event ) override;
    void keyDown( KeyEvent event ) override;
    void keyUp( KeyEvent event ) override;
    void updateWindowTitle();
    
    void fileDrop( FileDropEvent event ) override;
    void loadMovieFile( const fs::path &path );
    void loadVideo();
    
private:
    
    
    gl::TextureRef mImage;
    gl::TextureRef mFrameTexture;
    qtime::MovieGlRef mMovie;

    
    fs::path mSettings;
    WarpList mWarps;
    
    bool mUseBeginEnd;
    bool loaded = false;

    Area mSrcArea;
    Area mSrcAreaVideo;
    Area mSrcAreaShader;
    
    
    //Shader
    gl::TextureRef mTexture0;
    gl::TextureRef mTexture1;
    
    gl::GlslProgRef mShader;
    
    WarpBilinear bilinear;
        
};

//------------------------------------------------------------------------------------------------


void InaApp::prepareSettings( Settings *settings ) {
    
    settings->setWindowSize(1200, 768);
    settings->disableFrameRate();
    settings->setResizable(true);
    settings->setFullScreen(false);
    settings->setMultiTouchEnabled(false);
    //settings->setBorderless(true);
    //settings->setWindowPos(0,0);
}

//------------------------------------------------------------------------------------------------


void InaApp::setup() {
    
    ui::initialize();
    ci::gl::enableAlphaBlending();

    mUseBeginEnd = false;
    updateWindowTitle();
    
    mSettings = getAssetPath( "" ) / "warps.xml";
    if (fs::exists(mSettings)) {
        mWarps = Warp::readSettings(loadFile(mSettings));
    }
    
    try {
        mImage = gl::Texture::create(loadImage(loadAsset("test.jpg")),
                                     gl::Texture2d::Format().loadTopDown().mipmap(true).minFilter(GL_LINEAR_MIPMAP_LINEAR));
        mSrcArea = mImage->getBounds();

        mTexture0 = gl::Texture::create(loadImage(loadAsset("bottom.jpg")));
        mTexture1 = gl::Texture::create(loadImage(loadAsset("top.jpg")));
            
        mShader = gl::GlslProg::create(loadAsset("shader.vert"), loadAsset("shader.frag"));
        
    }
    
    catch( const std::exception &e ) {
        console() << e.what() << std::endl;
    }
    
    

}
    
//------------------------------------------------------------------------------------------------

void InaApp::cleanup(){
    //Warp::writeSettings( mWarps, writeFile( mSettings ) );
}

//------------------------------------------------------------------------------------------------

void InaApp::update() {
    
    static bool showDemoWindow = false;
    
    ui::SetNextWindowPos(ImVec2(0, 0));
    ui::SetNextWindowSize(ImVec2(getWindowWidth()/4, getWindowHeight()));
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.f;
    //The colors here


    //ui::SetNextWindowPos(imVec2(0,0), ImGuiCond_FirstUseEver);
    ui::ScopedWindow window("Interface", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    // setup the window menu bar
    //ui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
        
    if( ui::BeginMenuBar() ){
        if( ui::BeginMenu( "File" ) ){
            ui::MenuItem( "Copy" , nullptr, &showDemoWindow);
            ui::MenuItem( "Cut" );
            ui::MenuItem( "Paste" );
            ui::EndMenu();
        }
        if( ui::BeginMenu( "Edit" ) ){
            ui::MenuItem( "Copy" , nullptr, &showDemoWindow);
            ui::EndMenu();
        }
        if( ui::BeginMenu( "Specs" ) ){
            ui::MenuItem( "Copy" , nullptr, &showDemoWindow);
            ui::EndMenu();
        }
        ui::EndMenuBar();
    }
    if (showDemoWindow){
        ui::ShowDemoWindow();
    }
    

    ui::Text( "Window Content" );
    ui::BulletText("Double-click on title bar to collapse window.");
    
    if (ui::Button("Enable Edit"))
        Warp::enableEditMode(!Warp::isEditModeEnabled());
    
    if (ui::Button("Vertical Sync"))
        gl::enableVerticalSync( !gl::isVerticalSyncEnabled() );
    ui::SameLine();

    ui::TextDisabled("(?)");
     if (ui::IsItemHovered()){
         ui::BeginTooltip();
         ui::PushTextWrapPos(ui::GetFontSize() * 35.0f);
         ui::TextUnformatted("okok");
         ui::PopTextWrapPos();
         ui::EndTooltip();
     }
    
    if (ui::Button("Add Gamma"))
        Warp::enableGammaMode(true);
    ui::SameLine();
    if (ui::Button("Remove Gamma"))
        Warp::enableGammaMode(false);


    if (ui::Button("Reset Corners"))
        if (fs::exists(mSettings)) {
            mWarps = Warp::readSettings(loadFile(mSettings));
        }
    
    if (ui::Button("Bounds"))
        //With or without bounds
        mUseBeginEnd = !mUseBeginEnd;
        updateWindowTitle();
    
    
    
    if (ui::Button("Add Horiz Edges"))
    
    ui::AlignTextToFramePadding();
    ui::Text("Hold to repeat:");
    ui::SameLine();
    
    // Arrow buttons with Repeater
    static int counter = 0;
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ui::PushButtonRepeat(true);
    if (ui::ArrowButton("##left", ImGuiDir_Left)) {
        cout << "1";
    
    }
    ui::SameLine(0.0f, spacing);
    if (ui::ArrowButton("##right", ImGuiDir_Right)) {
        cout << "2";
    }
    ui::PopButtonRepeat();
    ui::SameLine();
    ui::Text("%d", counter);
    
    if (ui::Button("Load Video"))
        InaApp::loadVideo();
    
    if (ui::Button("Stop Video"))
        mMovie->stop();
    
    if (ui::Button("Play Video"))
        mMovie->play();
    
    if (ui::Button("Step Backward Frame"))
        mMovie->stepBackward();
    
    if (ui::Button("Step Forward Frame"))
        mMovie->stepForward();
    
    if (ui::Button("Video Rate"))
        mMovie->setRate(0.5);
        // If condition on the draw system

    
    ui::SetNextWindowPos(ImVec2(getWindowWidth()-(getWindowWidth()/4), 0));
    ui::SetNextWindowSize(ImVec2(getWindowWidth()/4, getWindowHeight()));
    ui::ScopedWindow secondWindow("Mapping", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    
    
    static ImVec4 color = ImColor(114, 144, 154, 200);
    
    ImGui::ColorPicker4("", (float*)&color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX);
    
    // Popup
    if (ui::Button("Toggle.."))
        ui::OpenPopup("my_toggle_popup");
    if (ui::BeginPopup("my_toggle_popup"))
    {
        if (ui::BeginMenu("Sub-menu")) {
            ui::MenuItem("Click me");
            ui::EndMenu();
        }
        
        ui::Separator();
        ui::Text("Tooltip here");
        if (ui::IsItemHovered())
            ui::SetTooltip("I am a tooltip over a popup");
        
        if (ui::Button("Stacked Popup"))
            ui::OpenPopup("another popup");
        
        ui::EndPopup();
    }
    // End Popup start Modal
    
    if (ui::Button("Delete.."))
        ui::OpenPopup("Delete?");
    if (ui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
        ui::Separator();
        
        //static int dummy_i = 0;
        //ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");
        
        static bool dont_ask_me_next_time = false;
        ui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        ui::PopStyleVar();
        
        if (ui::Button("OK", ImVec2(120, 0))) { ui::CloseCurrentPopup(); }
        ui::SetItemDefaultFocus();
        ui::SameLine();
        if (ui::Button("Cancel", ImVec2(120, 0))) { ui::CloseCurrentPopup(); }
        ui::EndPopup();
    }
    if (ui::Button("Setting Edges"))
        for( auto &warp : mWarps ) {
            warp->setEdges(0.1, 0.1, 1., 1.); //LOOK AT THIS
        }

    if (ui::Button("DeselectControlPoint"))
        for( auto &warp : mWarps ) {
            //warp->keyDown(&event)
            warp->deselectControlPoint();
        }
    
    if (ui::Button("Size->Resolution"))
        for( auto &warp : mWarps ) {
            warp->setSize(500,500); // Resolution
        }
    if (ui::Button("Move"))
        for( auto &warp : mWarps ) {
            //warp->keyDown(&event)

            vec2 size( warp->getSize() );
            warp->setControlPoint(1, vec2(getWindowWidth()/4,getWindowWidth()-getWindowWidth()/4)/size);
            // The positions
        }
    if (ui::Button("Luminance"))
        for( auto &warp : mWarps ) {
            warp->setLuminance(5); //Funciona melhor com o setEdges
        }
    
    
    
    if (ui::Button("+10 em X"))

        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->setNumControlX(10);
        }
    if (ui::Button("+10 em Y"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->setNumControlY(10);
        }
    if (ui::Button("Tira em X -1"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F1();
        }
    if (ui::Button("Adiciona em X +1"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F2();
        }
    if (ui::Button("Tira em Y -1"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F3();
        }
    if (ui::Button("Adiciona em Y +1"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F4();
        }
    if (ui::Button("Tira Qualidade"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F5();
        }
    if (ui::Button("Adiciona Qualidade"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F6();
        }
    if (ui::Button("Resolução da meshADAPT"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F7();
        }
    if (ui::Button("Vira a Mesh Horiz"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F8();
        }
    if (ui::Button("Vira a Mesh Vert"))
        
        for (auto &warp : mWarps) {
            auto bilinear = std::dynamic_pointer_cast<WarpBilinear>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F9();
        }
    if (ui::Button("Roda Conteudo CW"))
        
        for (auto &warp : mWarps) {
            warp->PERSPECTIVE;
            auto bilinear = std::dynamic_pointer_cast<WarpPerspective>(warp);
            
            if(bilinear)
                bilinear->keyboardPress_F11();
        }
            
    if( mMovie )
        mFrameTexture = mMovie->getTexture();
    
    if (loaded == true){
        static bool sPrintedDone = false;
        if( ! sPrintedDone && mMovie->isDone() ) {
            console() << "Done Playing" << std::endl;
            sPrintedDone = true;
        }
    }
}


//------------------------------------------------------------------------------------------------


void InaApp::draw() {
    
    gl::clear(Color(0,0,0));
    gl::color(0.1,0.1,0.1);
    gl::drawSolidRect( Rectf( getWindowWidth()/2-getWindowWidth()/4,
                             getWindowHeight(),
                             getWindowWidth()/2+getWindowWidth()/4,
                             getWindowHeight()-getWindowHeight()/10 ) );
    
    gl::color(0.1,0.1,0.1); // set color to blue
    gl::drawSolidRect( Rectf( getWindowWidth()/2-getWindowWidth()/4,
                             getWindowHeight()/2-getWindowHeight(),
                             getWindowWidth()/2+getWindowWidth()/4,
                             getWindowHeight()/10) );
    gl::color(1,1,1);
    
    /* Warping */
    if( mImage ) {
        // iterate over the warps and draw their content
        for( auto &warp : mWarps ) {
            // there are two ways you can use the warps:
            if( mUseBeginEnd ) {
                // a) issue your draw commands between begin() and end() statements
                warp->begin();
                // in this demo, we want to draw a specific area of our image,
                // but if you want to draw the whole image, you can simply use: gl::draw( mImage );
                gl::draw( mImage, mSrcArea, warp->getBounds() );
                warp->end();
            }
            else {
                // b) simply draw a texture on them (ideal for video)
                // in this demo, we want to draw a specific area of our image,
                // but if you want to draw the whole image, you can simply use: warp->draw( mImage );
                warp->draw( mImage, mSrcArea ); //mImage
            }
        }
    }
    if( mFrameTexture ) {
        //Rectf centeredRect = Rectf(mFrameTexture->getBounds()).getCenteredFit( getWindowBounds(), false );
        //gl::draw( mFrameTexture, centeredRect );
        mSrcAreaVideo = mFrameTexture->getBounds();
        for (auto &warp : mWarps){
            warp->begin();
            gl::draw( mFrameTexture, mSrcAreaVideo, warp->getBounds() );
            warp->end();
        }
    }
    
    /*if (mShader){
        
        mSrcAreaShader = mTexture1->getBounds();
        for (auto &warp : mWarps){
            warp->begin();
            gl::ScopedGlslProg shader( mShader );
            mShader->uniform( "tex0", 0 );
            mShader->uniform( "tex1", 1 );
            
            gl::ScopedTextureBind tex0( mTexture0, uint8_t( 0 ) );
            gl::ScopedTextureBind tex1( mTexture1, uint8_t( 1 ) );
            gl::drawSolidRect(warp->getBounds());
            
            warp->end();
        }
        
    }*/


}

//------------------------------------------------------------------------------------------------

void InaApp::mouseMove( MouseEvent event ) {
    
    // pass this mouse event to the warp editor first
    if( !Warp::handleMouseMove( mWarps, event ) ) {
        // let your application perform its mouseMove handling here
        
    }
}

//------------------------------------------------------------------------------------------------

void InaApp::mouseDrag( MouseEvent event ) {
    
    // pass this mouse event to the warp editor first
    if( !Warp::handleMouseDrag( mWarps, event ) ) {
        // let your application perform its mouseDrag handling here
    }
}

//------------------------------------------------------------------------------------------------

void InaApp::mouseUp( MouseEvent event ) {
    
    // pass this mouse event to the warp editor first
    if( !Warp::handleMouseUp( mWarps, event ) ) {
        // let your application perform its mouseUp handling here
    }
}

//------------------------------------------------------------------------------------------------

void InaApp::keyDown( KeyEvent event ) {
    // pass this key event to the warp editor first
    if( !Warp::handleKeyDown( mWarps, event ) ) {
        // warp editor did not handle the key, so handle it here
        switch( event.getCode() ) {
            case KeyEvent::KEY_ESCAPE:
                // quit the application
                quit();
                break;
            case KeyEvent::KEY_f:
                // toggle full screen
                setFullScreen(!isFullScreen());
                break;
            case KeyEvent::KEY_v:
                // toggle vertical sync
                gl::enableVerticalSync( !gl::isVerticalSyncEnabled() );
                break;
            case KeyEvent::KEY_w:
                // toggle warp edit mode
                Warp::enableEditMode( !Warp::isEditModeEnabled() );
                break;
            case KeyEvent::KEY_a:
                // toggle drawing a random region of the image
                if( mSrcArea.getWidth() != mImage->getWidth() || mSrcArea.getHeight() != mImage->getHeight() )
                    mSrcArea = mImage->getBounds();
                else {
                    int x1 = Rand::randInt( 0, mImage->getWidth() - 150 );
                    int y1 = Rand::randInt( 0, mImage->getHeight() - 150 );
                    int x2 = Rand::randInt( x1 + 150, mImage->getWidth() );
                    int y2 = Rand::randInt( y1 + 150, mImage->getHeight() );
                    mSrcArea = Area( x1, y1, x2, y2 );
                }
                break;
            case KeyEvent::KEY_SPACE:
                // toggle drawing mode
                mUseBeginEnd = !mUseBeginEnd;
                updateWindowTitle();
                break;
        }
    }
    if (event.getChar() == 'g'){
        cout << "Pressed g";
    }
}
//------------------------------------------------------------------------------------------------

void InaApp::keyUp( KeyEvent event ) {
    
    // pass this key event to the warp editor first
    if( !Warp::handleKeyUp( mWarps, event ) ) {
        // let your application perform its keyUp handling here
    }
}

//------------------------------------------------------------------------------------------------

void InaApp::updateWindowTitle() {
    
    if( mUseBeginEnd )
        getWindow()->setTitle( "Warping Sample - Using begin() and end()" );
    else
        getWindow()->setTitle( "Warping Sample - Using draw()" );
}

//------------------------------------------------------------------------------------------------

void InaApp::loadMovieFile( const fs::path &moviePath ) {
    try {
        // load up the movie, set it to loop, and begin playing
        mMovie = qtime::MovieGl::create( moviePath );
        mMovie->setLoop(true, false);
        mMovie->setVolume(0.f);
        mMovie->play();
        console() << "Playing: " << mMovie->isPlaying() << std::endl;
    }
    catch( ci::Exception &exc ) {
        console() << "Exception caught trying to load the movie from path: " << moviePath << ", what: " << exc.what() << std::endl;
        mMovie.reset();
    }
    
    mFrameTexture.reset();
}

//------------------------------------------------------------------------------------------------

void InaApp::fileDrop( FileDropEvent event ) {
    loadMovieFile(event.getFile(0));
}

//------------------------------------------------------------------------------------------------


void InaApp::loadVideo(){
    loaded = true;
    fs::path moviePath = getOpenFilePath();
    console() << "moviePath: " << moviePath << std::endl;
    if( ! moviePath.empty() )
        loadMovieFile( moviePath );
}

//------------------------------------------------------------------------------------------------


void InaApp::mouseDown( MouseEvent event ) {
   
    //sb::InputSimulation::keyTap(KeyEvent::KEY_F2);
}

/*void SDAWarpingHydraApp::resize()
 {
 // tell the warps our window has been resized, so they properly scale up or down
 Warp::handleResize(mWarps);
 }*/

CINDER_APP(InaApp, RendererGl(RendererGl::Options().msaa(16)), &InaApp::prepareSettings)

