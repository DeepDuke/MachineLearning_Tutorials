// using vector to store a bounding box: [x1, y1, x2, y2, score]
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

double IOU(vector<double> a, vector<double> b){
	double iw = min(a[2], b[2]) - max(a[0], b[0]) + 1;
	if(iw < 0) return 0.0;
	double ih = min(a[3], b[3]) - max(a[1], b[1]) + 1;
	if(ih < 0) return 0.0;
	double interArea = iw * ih;
	double unionArea = (a[2]-a[0]+1)*(a[3]-a[1]+1) + (b[2]-b[0]+1)*(b[3]-b[1]+1) - interArea;
	return interArea / unionArea;
}
vector<vector<double> > NMS(vector<vector<double> > bboxes, double thresh){
    vector<vector<double> > result;
    int cnt = bboxes.size();  // point to the last bbox which is not deleted
    while(cnt){
        // find the bbox with highest score
        int idx = 0;
        for(int i = 1; i < bboxes.size(); i++){
            if(bboxes[i][4] && bboxes[i][4] > bboxes[idx][4]){
                idx = i;
            }
        }
        vector<double> tmpBox = bboxes[idx];
        result.push_back(tmpBox);  // store into result 
        bboxes[idx][4] = 0;
        cnt--;
        // delete those with score < thresh by set their score to zero
        for(int i = 0; i < bboxes.size(); i++){
            if(bboxes[i][4] && IOU(tmpBox, bboxes[i]) > thresh){
                bboxes[i][4] = 0;
                cnt--;
            }
        }
    }
    return result;
}
int main(){
	vector<vector<double> > bboxes =  {
			 {690, 720, 800, 820, 0.5}, // x1, y1, x2, y2, score
             {102, 204, 250, 358, 0.5},
             {118, 257, 250, 380, 0.8}, 
             {135, 280, 250, 400, 0.7},
             {118, 255, 235, 360, 0.7}
			}; 
	double thresh = 0.4;
    vector<vector<double> > result = NMS(bboxes, thresh);
    // print result
	cout << "result with thresh=" << thresh << ":" << endl;
	for(int i = 0; i < result.size(); i++){
		for(int j = 0; j <result[i].size(); j++){
			cout << result[i][j] << " ";
		}
		cout << endl;
	}
	system("pause");
	return 0;	
}