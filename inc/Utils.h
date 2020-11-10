#pragma once
#include "ofMain.h"
#include <limits>
#include <algorithm>

namespace utils {
    static constexpr double pi = 3.14159265358979323846;

    std::pair<int, int> getOctavePitchPair(int midiNoteNumber);

    namespace note_grid {
        class PointNoteMap {
        public:
            PointNoteMap();
            
            void setup(const ofMesh& mesh, int width, int height, int widthGrid, int heightGrid);
            // point passed in must be from original mesh 
            // throws out_of_range exception if point does not exist in PointNoteMap
            int getNote(const ofPoint& point);
            
        private:
            float width, height;
            int widthGrid, heightGrid;

            float widthMin = std::numeric_limits<float>::max(), widthMax = std::numeric_limits<float>::min();
            float heightMin = std::numeric_limits<float>::max(), heightMax = std::numeric_limits<float>::min();
            
            std::map<std::pair<int, int>, int> pointNoteMap;


            void init(const ofMesh& mesh);


            int getNoteFromPoint(const ofVec3f& point);
            int getSegmentNumber(const float& coord, const float& dimLength, const int& nSegments, const float& minVal, const float& maxVal);
        };

       
    }
}
