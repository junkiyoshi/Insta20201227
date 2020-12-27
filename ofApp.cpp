#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	ofIcoSpherePrimitive ico_sphere = ofIcoSpherePrimitive(100, 3);
	vector<ofMeshFace> triangles = ico_sphere.getMesh().getUniqueFaces();
	for (int i = 0; i < triangles.size(); i++) {

		ofPoint average = (triangles[i].getVertex(0) + triangles[i].getVertex(1) + triangles[i].getVertex(2)) / 3;
		this->location_list.push_back(average);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 3;

	while (this->log_list.size() < 450) {

		int location_index = ofRandom(this->location_list.size());
		vector<glm::vec3> log;
		log.push_back(this->location_list[location_index]);
		this->log_list.push_back(log);
		this->color_list.push_back(ofColor(ofRandom(39, 139)));
		this->life_list.push_back(270);
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto x = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0065, this->noise_seed.x + ofGetFrameNum() * 0.005)), 0, 1, -15, 15);
		auto y = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0065, this->noise_seed.y + ofGetFrameNum() * 0.005)), 0, 1, -15, 15);
		auto z = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0065, this->noise_seed.z + ofGetFrameNum() * 0.005)), 0, 1, -15, 15);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec3(x, y, z));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofRotateY(ofGetFrameNum() * 0.75);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);
		ofFill();
		ofDrawSphere(this->log_list[i].front(), 2);

		auto alpha = this->life_list[i] > 60 ? 255 : ofMap(this->life_list[i], 0, 60, 0, 255);
		ofSetColor(this->color_list[i], alpha);

		ofNoFill();
		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
