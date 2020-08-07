#include "ofMain.h"

/*
 [課題2: 状態遷移プログラム]
 簡単な状態遷移プログラムを書いて下さい

*/

using StateRef = std::shared_ptr<class BaseWriteState>;

class BaseWriteState {
public:
    virtual ~BaseWriteState()=default;
    virtual void write(std::string str) = 0;
    
};

class UpperCase : public BaseWriteState {
public:
    UpperCase(std::stringstream& ss): BaseWriteState{}, ss_{ss}{
        
    }
    void write(std::string str) override {
        std::transform(str.cbegin(), str.cend(), str.begin(), ::toupper);
        ss_ << str;
    }
    
private:
    std::stringstream& ss_;
};

class LowerCase : public BaseWriteState {
public:
    LowerCase(std::stringstream& ss): BaseWriteState{}, ss_{ss}{
        
    }
    void write(std::string str) override {
        std::transform(str.cbegin(), str.cend(), str.begin(), ::tolower);
        ss_ << str;
    }
    
private:
    std::stringstream& ss_;

};


class Default : public BaseWriteState {
public:
    Default(std::stringstream& ss): BaseWriteState{}, ss_{ss}{
        
    }
    void write(std::string str) override {
        ss_ << str;
    }
    
private:
    std::stringstream& ss_;
    
};


class Editor {
public:
    Editor(StateRef state) : state_{state}{
        
    }
    
    void state(StateRef state) {
        state_ = state;
    }
    
    void write(const std::string& str) {
        state_ -> write(str);
    }
    
private:
    StateRef state_;
};

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
private:
    std::shared_ptr<Editor> editor;
    std::stringstream input;
    std::stringstream output;
    std::string state_str;
};


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    editor = std::make_shared<Editor>(std::make_shared<Default>(output));
    state_str = "Default";
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapStringHighlight("Input", 20, 20, ofColor::blue);
    ofDrawBitmapString(input.str(), 20, 40);

    ofDrawBitmapStringHighlight("Output : " + state_str, 20, ofGetHeight()/2. + 20, ofColor::blue);
    ofDrawBitmapString(output.str(), 20, ofGetHeight()/2. + 40);

}

void ofApp::keyPressed(int key) {
    switch (key) {
        case '1':
            if(editor) editor->state(std::make_shared<Default>(output));
            state_str = "Default";
            break;
            
        case '2':
            if(editor) editor->state(std::make_shared<UpperCase>(output));
            state_str = "UpperCase";
            break;
            
        case '3':
            if(editor) editor->state(std::make_shared<LowerCase>(output));
            state_str = "LowerCase";
            break;
            
        case OF_KEY_SHIFT:
            break;
            
        case OF_KEY_RETURN:
            input << '\n';
            editor->write(std::string{'\n'});
            break;
            
        default:
            if(key == 3680) break;
            input << static_cast<char>(key);
            editor->write(std::string{static_cast<char>(key)});
    }
}

int main( ){
    ofSetupOpenGL(640,320,OF_WINDOW);
    ofRunApp(new ofApp());
}
