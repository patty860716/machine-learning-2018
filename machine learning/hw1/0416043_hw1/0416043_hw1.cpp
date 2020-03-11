#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
#include<algorithm> 
#include<time.h>
#include<cstring>
#include<stdlib.h>
using namespace std;
int sortclass;
struct dat
{
	float iris[5];
};
vector<float> getuniqueset(vector<float> s)
{
	sort(s.begin(),s.end());
	vector<float> str;  
	str.resize(s.size());    
	str.resize(unique_copy(s.begin(), s.end(), str.begin()) - str.begin()); 
	return str;
}
bool cmp(const dat &a,const dat &b){
	return (a.iris[sortclass] < b.iris[sortclass]);
}
class ID3
{
	public:
		ID3(){s.erase(s.begin(),s.end());};
		~ID3(){};
		ID3(vector<dat> a);	
		double entropy(vector<float> data);	
		vector<vector<float> > getlabelvec();
		double getlabelgain(double bound,int n);
		ID3 getnewsub(ID3 ori,int label,double bound,int lr); 
		vector<dat> s;
		vector<float> calculatebound(int n);
};
ID3::ID3(vector<dat> data)
{
	s.erase(s.begin(),s.end());
	s.assign(data.begin(),data.end());
}
ID3 ID3::getnewsub(ID3 ori,int label,double bound,int lr)
{
	vector<vector<float> > datalabel = ori.getlabelvec();
	s.erase(s.begin(),s.end());
	dat tmp;
//	cout<<label<<i<<endl;
	switch(lr){
		case 0:
			for(int j=0;j<datalabel[label].size();j++)
		{
			if(bound>datalabel[label][j])
			{
				for(int i=0;i<5;i++)
				{
					tmp.iris[i] = datalabel[i][j];
				}
				this->s.push_back(tmp);	}}
			break;
		case 1:
				for(int j=0;j<datalabel[label].size();j++)
		{
			if(bound<=datalabel[label][j])
			{
				for(int i=0;i<5;i++)
				{
					tmp.iris[i] = datalabel[i][j];
				}
				this->s.push_back(tmp);	}}
			break;			
	}
	return *this;
}
vector<vector<float> > ID3::getlabelvec()
{
	vector<vector<float> > result;
	vector<float> result1;
	for(int j=0;j<5;j++)
	{
	for(int i=0;i<s.size();i++)
	{
		result1.push_back(s[i].iris[j]);
	}
	result.push_back(result1);result1.clear();
	}
	return result;
}
double ID3::entropy(vector<float> ss)
{
	//vector<float>::iterator i;
	vector<float> str = getuniqueset(ss);  
	int count[str.size()];
	memset(count,0,sizeof(int)*str.size());
 	for(int i=0;i<ss.size();i++)
 	{
 		for(int j=0;j<str.size();j++)
 		{
 			if(ss[i]==str[j])
 			count[j]++;
		 }
	 }
	 double entropy = 0.0;
	 //double entropytmp=0;
	 double p;
	 for(int j = 0; j < str.size(); j++)
  	{
  			p = (double)count[j]/(double)(ss.size());
  			entropy -= p*log2(p);
  	}
  	return entropy;
}
vector<float> ID3::calculatebound(int n)
{
	//vector<float> tmp,tmp1;
	vector<float> bound;
	float boundtmp;
	sortclass = n;
	sort(s.begin(),s.end(),cmp);
	vector<vector<float> >  tmp = getlabelvec();
	for(int i=0;i<tmp[n].size()-1;i++)
	{
		if(tmp[4][i]!=tmp[4][i+1])
		{
			boundtmp = (tmp[n][i]+tmp[n][i+1])/(float)2;
			bound.push_back(boundtmp);
		}
	}
	return bound;
}
double ID3::getlabelgain(double bound,int n)       //count infogain
{	
	vector<vector<float> > tmp = this->getlabelvec();
	double baseentropy = entropy(tmp[4]);
	vector<float> leftset,rightset;
 	for(int i=0;i<tmp[n].size();i++)
 	{
 		if(tmp[n][i]<bound)
 		{
 			leftset.push_back(tmp[4][i]);
		 }
 		else
 		{
 			rightset.push_back(tmp[4][i]);
		 }
	 }
	 double rem = 0.0;
	 double reml ,remr;
	 double totalsize = leftset.size()+rightset.size();
	 if(leftset.size()==0)
	 reml = 0;
	 else
	 reml = (double)leftset.size()*entropy(leftset)/totalsize;
	 remr = (rightset.size()==0)?0:(double)rightset.size()*entropy(rightset)/totalsize;
	 leftset.clear();
	 rightset.clear();
	 rem = reml+remr;
	 double Igain;
  	Igain = baseentropy - rem;
  	return Igain;
}
class dctree
{
	public:
		dctree(){value=-1;node = -1;};
		~dctree(){};
		dctree* buildtree(dctree *tree,ID3 s);
		int node;
		float value;
		dctree* left;
		dctree* right;
		dctree* bro;
		float testdata(dat test,dctree* tree);
		dctree &operator =  (const dctree &m);
};
dctree &dctree::operator =  (const dctree &m)
{
	value = m.value;
	node = m.node;
}
float dctree::testdata(dat test,dctree* tree)                    //test instance
{
	if(tree->node==4) //leaf
	{
		return tree->value;
	}
	if(tree->value<0) //null change to its brother
	return testdata(test,tree->bro);
		if(test.iris[tree->node]>=tree->value)
		{
		return testdata(test,tree->right);
		}
		else
		{
		return testdata(test,tree->left);	
		}	
}
dctree* dctree::buildtree(dctree *tree,ID3 set)
{
	if(set.s.size()==0)                      //subtree is null (usually not happen)
	{
		tree = new dctree();
		return tree;
	}
	else if(set.s.size()==1)                            //only left one instance =>leaf
	{
		tree->node=4;
		tree->value = set.s[0].iris[4];
		return tree;
	}
	vector<vector<float> > dataclass1 = set.getlabelvec();
	
	if(getuniqueset(dataclass1[4]).size()==1)              //if class only has one kind in set =>don't need to classify again =>leaf
	{
		tree->node = 4;
		tree->value = set.s[0].iris[4];
		return tree;
	}
	int bestlabel;
	vector<float> bound;
	double bestgain[5];
	memset(bestgain,0,sizeof(double)*5);
	float bestbound[5];
	double thebestgain = 0;
	float thebestbound = 0;
	double gaintmp;
	for(int i=0;i<4;i++)                                  //get best attribute,bound
	{
		bound = set.calculatebound(i);
		for(int j=0;j<bound.size();j++)
		{
			double gaintmp = set.getlabelgain(bound[j],i);
			if(gaintmp-bestgain[i]>1.e-8)
			{
				bestbound[i] = bound[j];
				bestgain[i] = gaintmp;
			}
		}
		if(bestgain[i]-thebestgain>1.e-8)
		{
			thebestgain = bestgain[i];
			thebestbound = bestbound[i];
			bestlabel = i;
			}	
	bound.clear();	
	}
	
	tree->node = bestlabel;
	tree->value = thebestbound;
	//cout<<tree->node<<" "<<tree->value<<endl;
	ID3 subsetl,subsetr;
    tree->left = new dctree();
    tree->right = new dctree();
    tree->left->bro = tree->right;
    tree->right->bro = tree->left;
    subsetl.getnewsub(set,bestlabel,thebestbound,0);               //split set to leftsubset,rightsubset
    subsetr.getnewsub(set,bestlabel,thebestbound,1);
    buildtree(tree->left,subsetl);                         //build subtreel return to tree left
    buildtree(tree->right,subsetr);
  return tree;
}

int main(int argc,char **argv)
{
	char* filename = argv[1];
	vector<dat> dataset,randdata;
	dat a;
	float class2;
	FILE *fp;
	fp = fopen(filename,"r");
	//fp = fopen("testdata.txt","r");
	if(!fp) {cout<<"error";
	return 0;
	}
	int cnt=1;
	char class1[20];
	while(!feof(fp))
	{
		fscanf(fp,"%f,%f,%f,%f,%s",&a.iris[0],&a.iris[1],&a.iris[2],&a.iris[3],class1);
		switch(class1[8])
		{
			case 'o':
				class2 = 0;
				break;
			case 's':
				class2 = 1;
				break;
			case 'g':
				class2 = 2;
				break;
		}
		
	a.iris[4] = class2;
		dataset.push_back(a);
	}
	int i,j;
	 srand( (int)time(NULL));
	 while(dataset.size()>0)                         //shuffle
    {
        int temp=rand()%dataset.size(); 
   		randdata.push_back(*(dataset.begin()+temp)) ;    
        dataset.erase(dataset.begin()+temp);   
    }
	ID3 data(randdata);
	int foldnum = randdata.size()/5;
	
	float classcorrect[3];
	int classtotal[3];
	float accuracy = 0;
	float precision[3];
	float predict[3];
	float recall[3];
	memset(recall,0,3*sizeof(float));
	memset(precision,0,3*sizeof(float));
	vector<dat> test,train;
	for(int i=0;i<randdata.size();i+=foldnum)               //k-fold
	{	
		
		float correct=0;
		memset(predict,0,3*sizeof(float));
		memset(classcorrect,0,3*sizeof(float));
		memset(classtotal,0,3*sizeof(int));
		test.insert(test.begin(),randdata.begin()+i,randdata.begin()+i+foldnum-1);
		//cout<<test.size();
		
		if(i-1>0)
		train.insert(train.begin(),randdata.begin(),randdata.begin()+i-1);
		train.insert(train.end(),randdata.begin()+i+foldnum,randdata.end());
		//cout<<train.size();
		ID3 traindata(train);
		dctree *tree = new dctree();
		tree = tree->buildtree(tree,traindata);                //build training tree
		for(int j=0;j<test.size();j++)
		{
			float p = tree->testdata(test[j],tree) ;       //test instance in test vector on training tree
			float r = test[j].iris[4] ;
			classtotal[(int)r]++;
			predict[(int)p]++;
			if(r==p)
			{
				correct++;
				classcorrect[(int)r]++;
			}
		//	cout<<test[j].iris[4]<<" "<<tree->testdata(test[j],tree)<<endl;
		}
		//cout<<endl;
		accuracy+=correct/(float)foldnum;
		for(int o=0;o<3;o++)
	{
	if(classcorrect[o]==0)continue;
	precision[o] += classcorrect[o]/(float)classtotal[o];
	recall[o]+= classcorrect[o]/predict[o];
	}
	test.clear();
	train.clear();
	//cout<<endl;
	}	
	cout<<accuracy/(float)5<<endl;
	for(int o=0;o<3;o++)
	{
	cout<<precision[o]/(float)5<<" ";
	cout<<recall[o]/(float)5<<endl;
	
	}
	return 0;
}
