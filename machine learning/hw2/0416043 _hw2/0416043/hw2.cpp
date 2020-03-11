#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<time.h>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<sstream>
#include<fstream>
using namespace std;
int sortclass = 0;
class dat
{
	public:
	double dimension[9];
	string name,type;
	int index;
	double dist;
	dat &operator =  (const dat &m);
};
vector<dat> knn;
dat &dat::operator =  (const dat &m)
{
	this->name = m.name;
	this->type = m.type;
	this->name = m.name;
	this->index = m.index;
	this->dist = m.dist;
	for(int i=0;i<9;i++)
	{
		this->dimension[i] = m.dimension[i];
	}
	return *this;
}
bool cmp(const dat &a,const dat &b){
	return (a.dimension[sortclass] < b.dimension[sortclass]);
}
bool cmp1(const dat &a,const dat &b){
	return (a.dist < b.dist);
}
double distance1(dat a, dat b)
{
	double sum = 0.0;
	double x = 1.0/2.0;
	for(int i=0;i<9;i++)
	{
		sum += pow(fabs(a.dimension[i] - b.dimension[i]),2);
	}
	return pow(sum,x);

}
class node
{
	public:
		node(){s.erase(s.begin(),s.end());};
		~node(){};
		node(vector<dat> a);
		node *getnewsub(node ori,int label,double med,int lr);
		vector<dat> s;
		dat calculatemedium(int label);
		//vector<double> getuniqueset(int i);
		//int getbestlabel();
};
node::node(vector<dat> data)
{
	s.erase(s.begin(),s.end());
	s.assign(data.begin(),data.end());
}
dat node::calculatemedium(int n)
{
	vector<dat> tmp;
	tmp.assign(s.begin(),s.end());
	sortclass = n;
	sort(tmp.begin(),tmp.end(),cmp);
	return tmp[tmp.size()/2];
}
node *node::getnewsub(node ori,int label,double med,int lr)
{
	//double med = ori.calculatemedium(label);
	switch(lr)
	{
		case 0:         //left
	for(int i=0;i<ori.s.size();i++)
	{
		if(ori.s[i].dimension[label]<med)
		this->s.push_back(ori.s[i]);
	}
	break;
	case 1:  //right
		for(int i=0;i<ori.s.size();i++)
	{
		if(ori.s[i].dimension[label]>=med)
		this->s.push_back(ori.s[i]);
	}
	break;
	default:
		cout<<"error";
		break;
	}
	//cout<<this->s.size();
	return this;

}
class dctree
{
	public:
		dctree(){attr = -1;med.index = -1;parent = 0;};
		~dctree(){};
		dctree* buildtree(dctree *tree,node s);
		dat med;
		string type;
		dctree* left;
		dctree* right;
		dctree* bro;
		dctree* parent;
		dctree* searchtree(dat q,dctree* tree);
		int attr;
};
dctree* dctree::searchtree(dat q,dctree* tree)
{
	//cout<<tree->attr;
	if(tree->left->attr == -1&&tree->right->attr == -1)//leaf
	{
		//cout<<tree->med.index;
		if(tree->attr>=0)
		return tree;
		else return searchtree(q,tree->bro);
	}
	if(tree->right->attr<0)
        return searchtree(q,tree->left);
    else if(tree->left->attr<0) return searchtree(q,tree->right);
	else if(q.dimension[tree->attr]<tree->med.dimension[tree->attr])
	{
	return searchtree(q,tree->left);
	}
	else if(q.dimension[tree->attr]>=tree->med.dimension[tree->attr])
	{
		return searchtree(q,tree->right);
	}
}
dctree* dctree::buildtree(dctree *tree,node set)
{
	if(set.s.size()==0)                      //subtree is null (usually not happen)
	{
	    tree->attr = -1;
	tree->left = new dctree();
	tree->right = new dctree();
		return tree;
	}
	if(set.s.size()==1)                            //only left one instance =>leaf
	{
		tree->attr=9;
		tree->left = new dctree();
		tree->right = new dctree();
		tree->med = set.s[0];
		return tree;
	}
	if(tree->parent==0)
	tree->attr = 0;
	else
	{
		tree->attr = tree->parent->attr + 1;
		if(tree->attr>8) tree->attr = 0;
	}
	tree->med = set.calculatemedium(tree->attr);
	node subsetl,subsetr;
    tree->left = new dctree();
    tree->right = new dctree();
    tree->left->parent = tree;
    tree->right->parent = tree;
    int i;
    for(i=0;i<set.s.size();i++)
    {
    	if(tree->med.index == set.s[i].index)
    	break;
	}
    set.s.erase(set.s.begin()+i);
    tree->left->bro = tree->right;
    tree->right->bro = tree->left;
    double bound = tree->med.dimension[tree->attr];
    subsetl.getnewsub(set,tree->attr,bound,0);               //split set to leftsubset,rightsubset
    subsetr.getnewsub(set,tree->attr,bound,1);
   //cout<<subsetl.s.size()<<" "<<subsetr.s.size()<<" "<<bound<<" "<<tree->attr<<endl;
    buildtree(tree->left,subsetl);                     //build subtreel return to tree left
    buildtree(tree->right,subsetr);
  return tree;
}
void getktmp(dctree *best,int k)
{
	if(knn.size()<k)   //未滿k個
	{
	knn.push_back(best->med);
	sort(knn.begin(),knn.end(),cmp1);
	}
	else if(best->med.dist<knn[knn.size()-1].dist)   //已經滿k個 但出現更近的
	{
		knn.erase(knn.end());
		knn.push_back(best->med);
		sort(knn.begin(),knn.end(),cmp1);
	}
}
dctree* knnfunc(dctree *tree,int k,dat q)
{
	dctree *besttemp = tree->searchtree(q,tree);   //search to leaf
	while(besttemp!=tree->parent)
	{
	dat besttmp = besttemp->med;
	besttemp->med.dist = distance1(q,besttmp);
	if(besttemp->attr!=-1||besttemp->parent == 0)
	{
		getktmp(besttemp,k);
		besttemp->attr = -1;
	}
	else
    {besttemp = besttemp->parent; continue;
    }
	if(besttemp->parent!=tree->parent)
	{
		dat pp = besttemp->parent->med;
	int att = besttemp->parent->attr;
		if(besttemp->bro->attr>=0&&distance1(q,tree->med)>fabs(pp.dimension[att]-q.dimension[att])) //另一邊可能有更近ㄉ
		{
            besttemp = besttemp->bro;
            besttemp = knnfunc(besttemp,k,q);
		}
		else
		{
	//	    besttemp->attr = -1;
		    besttemp->bro->attr = -1;
            besttemp = besttemp->parent;
		}
	}
	else
	{
            besttemp = besttemp->parent;
		}
	}
	return besttemp;
	//cout<<"end";
}
int main(int argc,char **argv)
{
	char* filename = argv[1];
	char* filename1 = argv[2];
	vector<dat> dataset;
	vector<vector<dat> > knnresult;
	fstream fp,fp1,fp2;
	fp.open(filename,ios::in);
	fp1.open(filename1,ios::in);
	//fp2.open(argv[3],ios::out);
	if(!fp)
	cout<<"error";
	string line;
	vector<string> t;
  	while (getline( fp, line,'\n'))
	{
	  istringstream tmp(line);
	  string data;
	  dat temp;
	  while (getline( tmp, data,','))
	  {
	  	t.push_back(data);
	  }
	stringstream ss(t[0]);
	  ss>>temp.index;
	  ss.clear();
	  temp.name = t[1];
	  for(int i=2;i<=10;i++)
	  {
	  	temp.dimension[i-2] = atof(t[i].c_str());
	   }
	   //cout<<temp.dimension[0]<<endl;
	   temp.type = t[11];
	   dataset.push_back(temp);
	  t.clear();
	}
	dataset.erase(dataset.begin());
	int x=0;
	node set(dataset);
	dataset.clear();
	dctree *tree = new dctree();
	while (getline( fp1, line,'\n'))
	{
	    if(x==0)
        {x++;
            continue;

        }
	  istringstream tmp(line);
	  string data;
	  dat temp;
	  while (getline( tmp, data,','))
	  {
	  	t.push_back(data);
	  }
temp.index = atoi(t[0].c_str());
	  temp.name = t[1];
	  for(int i=2;i<=10;i++)
	  {
	  	temp.dimension[i-2] = atof(t[i].c_str());
	   }
	   temp.type = t[11];
	   dataset.push_back(temp);t.clear();
	}
   int classmap[8],maxi;
	int a[4] = {1,5,10,100};
	for(int p=0;p<4;p++)
	{double sum = 0.0;
	int k=0;
	for(int o=0;o<dataset.size();o++)
	{memset(classmap,0,sizeof(int)*8);
	tree->buildtree(tree,set);
    knnfunc(tree,a[p],dataset[o]);
    for(int i=0;i<knn.size();i++)
    {
    	//cout<<knn[i].type<<endl;
    	if(knn[i].type[0]=='c'&&knn[i].type[1]=='p')   //cp
    	{
    		classmap[0]++;
    		continue;
		}
		else if(knn[i].type[0]=='i'&&knn[i].type[1]=='m'&&knn[i].type[2] == 'U') //imU
    	{
    		classmap[1]++;
    		continue;
		}
		else if(knn[i].type[1]=='p'&&knn[i].type[0] == 'p')
    	{
    		classmap[2]++;
    		continue;
		}
		else if(knn[i].type[2] == 'S'&&knn[i].type[0] == 'i'&&knn[i].type[1] == 'm')
    	{
    		classmap[3]++;
    		continue;
		}
		else if(knn[i].type[2] == 'L'&&knn[i].type[1] == 'm'&&knn[i].type[0]=='o')
    	{
    		classmap[4]++;
    		continue;
		}
		else if(knn[i].type[0] == 'o'&&knn[i].type[1]=='m')
    	{
    		classmap[5]++;
    		continue;
		}
		else if(knn[i].type[1] == 'n'&&knn[i].type[0]=='i'&&knn[i].type[0]=='L')
    	{
    		classmap[6]++;
    		continue;
		}
		else if(knn[i].type[0]=='i'&&knn[i].type[1]=='m')
    	{
    		classmap[7]++;
    		continue;
		}
    }
    maxi = 0;
    int predict = 0;
    string predicttype;
    for(int i=0;i<8;i++)
    {
    	if(maxi<classmap[i])
    	{maxi = classmap[i];
    	predict = i;
		}
	}
	int test;
	if(dataset[o].type[0]=='c'&&dataset[o].type[1]=='p')   //cp
    	{
    		test = 0;
		}
		else if(dataset[o].type[0]=='i'&&dataset[o].type[1]=='m'&&dataset[o].type[2] == 'U') //imU
    	{
    		test = 1;
		}
		else if(dataset[o].type[1]=='p'&&dataset[o].type[0] == 'p')
    	{
    		test = 2;
		}
		else if(dataset[o].type[2] == 'S'&&dataset[o].type[0] == 'i'&&dataset[o].type[1] == 'm')
    	{
    		test = 3;
		}
		else if(dataset[o].type[2] == 'L'&&dataset[o].type[1] == 'm'&&dataset[o].type[0]=='o')
    	{
    		test = 4;
		}
		else if(dataset[o].type[0] == 'o'&&dataset[o].type[1]=='m')
    	{
    		test = 5;
		}
		else if(dataset[o].type[1] == 'n'&&dataset[o].type[0]=='i'&&dataset[o].type[0]=='L')
    	{
    		test = 6;
		}
		else if(dataset[o].type[0]=='i'&&dataset[o].type[1]=='m')
    	{
    		test = 7;
		}
	if(test == predict) k++;
    if(knnresult.size()<3)
	 knnresult.push_back(knn);
    knn.clear();
    delete tree;
    tree = new dctree();
	}
	cout<<"knn accuracy: "<<(double)k/(double)dataset.size()<<endl;
	for(int i=0;i<knnresult.size();i++)
	{
		for(int j=0;j<knnresult[i].size();j++)
		cout<<knnresult[i][j].index<<" ";
		cout<<endl;
	}
	cout<<endl;
	knnresult.clear();
	}
	return 0;
}
