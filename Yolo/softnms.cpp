#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

double IOU(vector<double> a, vector<double> b){
	double iw = min(a[2], b[2]) - max(a[0], b[0]) + 1;
	if(iw < 0) return 0.0;
	double ih = min(a[3], b[3]) - max(a[1], b[1]) + 1;
	if(ih < 0) return 0.0;
	double interArea = iw * ih * 1.0;
	double unionArea = (a[2]-a[0]+1)*(a[3]-a[1]+1) + (b[2]-b[0]+1)*(b[3]-b[1]+1) - interArea;
	// cout << "interArea:\t" << interArea << "\tunionArea:\t" << unionArea << endl;
	return interArea / unionArea * 1.0;
}

double func(double score, double iou, double thresh){
	if(score < thresh) return score;
	else return score*(1-iou);
}

vector<vector<double> > softNMS(vector<vector<double> > bboxes, double thresh){
	vector<vector<double> >  buffer, result;
	while(bboxes.size() > 0){
		// find bbox with maximum score
		vector<double> tmp = bboxes[0];
		int tmp_idx = 0;
		for(int i = 1; i < bboxes.size(); i++){
			if(bboxes[i][4] > tmp[4]){
				tmp = bboxes[i];
				tmp_idx = i;
			}
		}
		for(int i = 1; i < bboxes.size(); i++){
			double iou = IOU(bboxes[i], tmp);
			bboxes[i][4] = func(bboxes[i][4], iou, thresh);
		}
		buffer.push_back(tmp);
		bboxes.erase(bboxes.begin()+tmp_idx);
	}
	// print bboxes
	cout << "After softNMS, bboxes:\n";
	for(int i = 0; i < buffer.size(); i++){
		for(int j = 0; j < buffer[i].size(); j++){
			cout << buffer[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	//
	for(int i = 0; i < buffer.size(); i++){
		if(buffer[i][4] > thresh){
			result.push_back(buffer[i]);
		}
	}
	return result;
}
int main()
{
	vector<double> v1 = {100, 100, 200, 200, 0.5}, v2 = {150, 150, 250, 250, 0.6}, v3 = {200, 200, 300, 300, 0.6};
	// cout << "iou:\t" << IOU(v1,  v1) << endl;
	// cout << 1/2.0*1.0 << endl;
	vector<vector<double> > bboxes =  {
			 {690, 720, 800, 820, 0.5}, // x1, y1, x2, y2, score
             {102, 204, 250, 358, 0.5},
             {118, 257, 250, 380, 0.8}, 
             {135, 280, 250, 400, 0.7},
             {118, 255, 235, 360, 0.7}
			}; 
	double thresh = 0.4;
	vector<vector<double> > result = softNMS(bboxes, thresh);
	// print result
	cout << "result with thesh=" << thresh << ":" << endl;
	for(int i = 0; i < result.size(); i++){
		for(int j = 0; j <result[i].size(); j++){
			cout << result[i][j] << " ";
		}
		cout << endl;
	}
	system("pause");
	return 0;	
}

