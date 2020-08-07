#include "ofMain.h"

/*
 [課題1: 三角形問題]
 1cm, 2cm, ...10cmの10本の棒が入った箱があります
 この箱からランダムに3本の棒を取り出したとき、その3本で三角形を作ることができる確率を計算するプログラムを書いてください
 たとえば3cm, 4cm, 5cmの棒だと三角形を作れますが、2cm, 5cm, 8cmだと三角形を作ることができません
 乱数でシミュレーションしてもかまいませんし、あらゆる可能性を数えあげてもかまいません
 
 */

bool is_triangle(const unsigned int length1,
                 const unsigned int length2,
                 const unsigned int length3)
{
    std::vector<unsigned int> len = { length1, length2, length3 };
    auto max_length = *std::max_element(len.begin(), len.end());
    auto result = std::remove(len.begin(), len.end(), max_length);
    len.erase(result, len.end());
    return max_length < len[0] + len[1];
}

std::vector<int> get_random_length_combination() {
    int l1 = static_cast<int>(ofRandom(1, 11));
    int l2 = l1;
    int l3 = l1;
    
    while(l1 == l2) {
        l2 = static_cast<int>(ofRandom(1, 11));
    }
    
    while(l1 == l3 || l2 == l3) {
        l3 = static_cast<int>(ofRandom(1, 11));
    }

    return { l1, l2, l3};
}

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
private:
    uint64_t sum_ { 0 };
    uint64_t counter_ { 0 };
    double probability_;
    std::deque<double> probability_history_;
    int num_attempts_per_frame_ { 5 };
};

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetWindowTitle("triangle-inequality-simulation");
}

//--------------------------------------------------------------
void ofApp::update(){
    for(auto i = 0; i < num_attempts_per_frame_; ++i) {
        auto length = get_random_length_combination();
        bool result = is_triangle(length[0], length[1], length[2]);
        ++counter_;
        sum_ += static_cast<int>(result);
        probability_ = static_cast<double>(sum_) / static_cast<double>(counter_);
        probability_history_.push_back(probability_);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    std::stringstream ss;
    ss << std::fixed << std::setprecision(4) << probability_;
    ofDrawBitmapStringHighlight("Probability: " + ofToString(ss.str()), 10, 20, ofColor::blue);
    ofDrawBitmapStringHighlight("Attempts: " + ofToString(counter_), 10, 40, ofColor::blue);

    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(0, ofGetHeight());
    ofNoFill();
    ofBeginShape();
    {
        for(auto i = 0; i < probability_history_.size(); ++i){
            ofVertex( i * (static_cast<float>(ofGetWidth()) / (probability_history_.size()-1)),
                     -1 * probability_history_[i] * static_cast<float>(ofGetHeight()) );
        }
    }
    ofEndShape();
    ofPopMatrix();
    ofPopStyle();
}

int main( ){
    ofSetupOpenGL(1200,400,OF_WINDOW);
    ofRunApp(new ofApp());
}

