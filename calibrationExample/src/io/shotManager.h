

#include "ofMain.h"
#include "frameDataManager.h"

typedef struct
{
	string name;
	string footage2K;
	string footageHD;
	string footageSD;
	
	bool aCam;
	vector<string> faceData;
} Shot;

class shotManager {

  public:
	void setup();
	string footageBasePath;
	void loadShot(string name, frameDataManager& frameManager, bool lowRes = true);
	map<string, Shot> shots;
	
};
